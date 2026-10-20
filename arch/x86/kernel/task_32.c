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

void task_context_switch(struct task *saved, struct task *restored)
{
    context_switch(saved->context, restored->context);
}

void task_context_init(struct task *task, void (*func)(void *), void *arg)
{
    unsigned char *stack_btm;

    stack_btm = (unsigned char *)task->kstack + PAGE_SIZE;
    stack_btm -= sizeof(struct task_context);
    task->context = (struct task_context *)stack_btm;

    memset(task->context, 0, sizeof(struct task_context));

    stack_btm -= 4;
    *(uint32_t *)stack_btm = (uint32_t)arg;
    stack_btm -= 4;
    *(uint32_t *)stack_btm = (uint32_t)func;
    stack_btm -= 4;
    *(uint32_t *)stack_btm = 0; /* fake return address */
    stack_btm -= 4;
    *(uint32_t *)stack_btm = (uint32_t)trampoline;

    task->context->esp = (uint32_t)stack_btm;
}