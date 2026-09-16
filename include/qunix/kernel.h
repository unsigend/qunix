/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_KERNEL_H
#define QUNIX_KERNEL_H

#include <qunix/macros.h>

/* printk is a printf-like function that prints a formatted string to the
 * console. */
__attribute__((format(printf, 1, 2))) extern int printk(const char *fmt, ...);

/* panic is a function that print the panic message and then hang the system. */
__attribute__((format(printf, 1, 2))) extern void panic(const char *fmt, ...)
    __attribute__((noreturn));

/* qunix kernel main function, the ctx is the hardware specific context
 * pointer ignored by the kernel. */
extern int kernel_main(void *ctx);

#endif