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

#include <kernel/panic.h>
#include <kernel/tty.h>

static struct tty_ops *dev;

void tty_register(struct tty_ops *ops)
{
    if (dev)
        panic("tty already registered");
    if (!ops->putchar)
        panic("tty ops missing functions");

    dev = ops;
}

void tty_putc(char c)
{
    if (!dev)
        return;

    dev->putchar(c);
}

void tty_write(const char *s, size_t len)
{
    if (!dev)
        return;

    while (len--)
        dev->putchar(*s++);
}