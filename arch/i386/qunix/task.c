/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/task.h>
#include <qunix/kernel.h>
#include <qunix/task.h>
#include <stdint.h>

extern char stack_top[]; /* from boot.S */

char *task_boot_kstack(void) { return stack_top; }

static void trampoline(void (*func)(void *), void *data)
{
    func(data);

    /* TODO: implement task destroy, zombie task handling, and task exit */

    panic("trampoline function should not return"); /* should not reach here */
}

struct task_context *task_context_init(char *kstack, void (*func)(void *),
                                       void *data)
{
    struct task_context *context;

    /* Build the trampoline function stack frame:
     *
     *  kstack       stack bottom address 4KB aligned
     *  kstack-4     padding
     *  kstack-8     padding
     *  kstack-12    data
     *  kstack-16    func
     *  kstack-20    unused return address
     *  kstack-24    eip = trampoline
     *  kstack-28    ebp = 0
     *  kstack-32    ebx = 0
     *  kstack-36    esi = 0
     *  kstack-40    edi = 0
     *
     * Base on System V ABI for i386, the call requires esp % 16 == 0, so need
     * padding to 16 bytes follow the stack layout.
     */

    *(uint32_t *)(kstack - 4) = 0;               /* padding */
    *(uint32_t *)(kstack - 8) = 0;               /* padding */
    *(uint32_t *)(kstack - 12) = (uint32_t)data; /* trampoline arg 2 */
    *(uint32_t *)(kstack - 16) = (uint32_t)func; /* trampoline arg 1 */
    *(uint32_t *)(kstack - 20) = 0;              /* return address (unused) */

    context = (struct task_context *)(kstack - 40);
    context->eip = (uint32_t)trampoline;
    context->ebp = 0;
    context->ebx = 0;
    context->esi = 0;
    context->edi = 0;

    return context;
}