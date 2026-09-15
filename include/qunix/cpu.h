/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_CPU_H
#define QUNIX_CPU_H

#include <qunix/macros.h>

/* Halt the CPU. */
CONTRACT extern void cpu_halt(void) __attribute__((noreturn));

#endif