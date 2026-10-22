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

#include <kernel/atomic.h>
#include <kernel/backtrace.h>
#include <kernel/cpu.h>
#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/sprintf.h>
#include <kernel/stdarg.h>
#include <kernel/string.h>
#include <kernel/tty.h>

static atomic_t in_panic = ATOMIC_INIT(0);
static const char *msg = "PANIC: ";

void panic(const char *fmt, ...)
{
    char buf[1024];
    va_list args;

    interrupt_disable();
    if (atomic_xchg(&in_panic, 1) != 0)
        cpu_halt();

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    tty_write_raw(msg, strlen(msg));
    tty_write_raw(buf, strlen(buf));
    tty_write_raw("\n", 1);

    backtrace();

    cpu_halt();
}

void panic_error(int errnum, const char *s)
{
    int err = errnum;

    if (errnum < 0)
        err = -errnum;

    panic("%s : %s", s, strerror(err));
}