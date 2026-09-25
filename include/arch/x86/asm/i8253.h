/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_X86_ASM_I8253_H
#define ARCH_X86_ASM_I8253_H

/* Intel 8253 Programmable Interval Timer (PIT) */

#include <stdint.h>

/* Initialize the 8253 programmable interval timer with the given frequency in
 * Hz, return 0 on success, -errno on failure */
extern int i8253_init(uint32_t hz);

#endif