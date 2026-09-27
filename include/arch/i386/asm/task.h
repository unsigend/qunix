/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_TASK_H
#define ARCH_I386_ASM_TASK_H

#include <qunix/task.h>
#include <stdint.h>

/* Context of a task, 5 callee-saved registers */
struct task_context {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
};

#endif