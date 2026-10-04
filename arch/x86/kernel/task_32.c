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
#include <kernel/panic.h>
#include <kernel/task.h>

extern char stack_top[]; /* from boot.S */

char *task_boot_kstack(void) { return stack_top; }

static void trampoline(void (*func)(void *), void *data)
{
    cpu_enable_interrupts(); /* every new thread starts with interrupts enabled
                                since it is a new context */

    func(data);

    task_exit(0);
}

struct task_context *task_context_init(char *kstack, void (*func)(void *),
                                       void *data)
{
    struct task_context *context;

    /* Stack layout 4-bytes aligned
     *  kstack-4     data
     *  kstack-8     func
     *  kstack-12    return address (unused)
     *  kstack-16    eip
     *  kstack-20    ebp
     *  kstack-24    ebx
     *  kstack-28    esi
     *  kstack-32    edi
     */
    *(uint32_t *)(kstack - 4) = (uint32_t)data; /* trampoline arg 2 */
    *(uint32_t *)(kstack - 8) = (uint32_t)func; /* trampoline arg 1 */
    *(uint32_t *)(kstack - 12) = 0;             /* return address (unused) */

    context =
        (struct task_context *)(kstack - 12 - sizeof(struct task_context));
    context->eip = (uint32_t)trampoline;
    context->ebp = 0;
    context->ebx = 0;
    context->esi = 0;
    context->edi = 0;

    return context;
}