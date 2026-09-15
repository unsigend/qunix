/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <ext/printf.h>
#include <qunix/kernel.h>
#include <qunix/tty.h>
#include <stdarg.h>

int printk(const char *fmt, ...)
{
    char buffer[1024];

    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    tty_puts(buffer);
    return ret;
}