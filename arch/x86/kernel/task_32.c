/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <asm/fpu.h>
#include <asm/task.h>
#include <asm/tss.h>

#include <kernel/cpu.h>
#include <kernel/errno.h>
#include <kernel/interrupt.h>
#include <kernel/kmalloc.h>
#include <kernel/mm/page.h>
#include <kernel/string.h>
#include <kernel/task.h>

/*
 * task_fpu_init - allocate an aligned FXSAVE image for @task.
 *
 * kmalloc is not 16-byte aligned. The extra FPU_STATE_ALIGN bytes let
 * the image start at the next 16-byte boundary after a stored raw
 * pointer. ROUND_UP(raw + sizeof(void *)) keeps that pointer inside
 * the block even when raw is already aligned.
 *
 * Returns 0 on success, -ENOMEM if the buffer cannot be allocated.
 */
int task_fpu_init(struct task *task)
{
    void *raw;
    uintptr_t aligned;

    raw = kmalloc(FPU_STATE_SIZE + FPU_STATE_ALIGN);
    if (!raw)
        return -ENOMEM;

    aligned = ROUND_UP((uintptr_t)raw + sizeof(void *), FPU_STATE_ALIGN);
    ((void **)aligned)[-1] = raw;
    task->fpu_state = (void *)aligned;
    fpu_state_init(task->fpu_state);
    return 0;
}

void task_fpu_free(struct task *task)
{
    void *raw;
    if (!task->fpu_state)
        return;

    raw = ((void **)task->fpu_state)[-1];
    kfree(raw);
    task->fpu_state = NULL;
}

/*
 * trampoline - first user of a new kernel thread.
 *
 * context_switch lands here with @func and @arg already on the stack.
 * Schedule left in_sched set and IRQs off. Clear that and enable
 * interrupts before @func. If @func returns, exit with status 0.
 */
static void trampoline(void (*func)(void *), void *arg)
{
    cpu_current()->in_sched = false;
    interrupt_enable();
    func(arg);
    task_exit(0);
}

/*
 * task_context_switch - switch from @saved to @restored.
 *
 * Called by schedule() only. @saved's context is stored and @restored's
 * is loaded. Returns in @restored, as if its own switch had returned.
 */
void task_context_switch(struct task *saved, struct task *restored)
{
    tss_set_kstack(cpu_id(), restored->kstack + PAGE_SIZE);
    fpu_state_save(saved->fpu_state);
    fpu_state_restore(restored->fpu_state);
    context_switch(saved->context, restored->context);
}

/*
 * task_context_init - build @task's initial kernel stack.
 *
 * The stack is already allocated. The first switch to @task enters @func(@arg)
 * with interrupts enabled. If @func returns, the trampoline calls task_exit(0).
 */
void task_context_init(struct task *task, void (*func)(void *), void *arg)
{
    unsigned char *stackbase;

    stackbase = (unsigned char *)task->kstack + PAGE_SIZE;
    stackbase -= sizeof(struct task_context);
    task->context = (struct task_context *)stackbase;

    memset(task->context, 0, sizeof(struct task_context));

    stackbase -= 4;
    *(uint32_t *)stackbase = (uint32_t)arg;
    stackbase -= 4;
    *(uint32_t *)stackbase = (uint32_t)func;
    stackbase -= 4;
    *(uint32_t *)stackbase = 0; /* fake return address */
    stackbase -= 4;
    *(uint32_t *)stackbase = (uint32_t)trampoline;

    task->context->esp = (uint32_t)stackbase;
}