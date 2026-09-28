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