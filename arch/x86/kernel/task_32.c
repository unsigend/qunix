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

#include <asm/task.h>
#include <asm/tss.h>

#include <kernel/cpu.h>
#include <kernel/interrupt.h>
#include <kernel/mm/page.h>
#include <kernel/string.h>
#include <kernel/task.h>

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