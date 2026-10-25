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
#include <kernel/spinlock.h>
#include <kernel/tty.h>

static struct tty_ops *dev;
static struct spinlock tty_lock;

void tty_register(struct tty_ops *ops)
{
    if (dev)
        panic("tty already registered");
    if (!ops->putc)
        panic("tty ops missing functions");

    dev = ops;
}

void tty_putc(char c)
{
    unsigned long irqflags;
    if (!dev)
        return;

    irqflags = spinlock_lock_irqsave(&tty_lock);
    dev->putc(c);
    spinlock_unlock_irqrestore(&tty_lock, irqflags);
}

void tty_write(const char *s, size_t len)
{
    unsigned long irqflags;

    if (!dev)
        return;

    irqflags = spinlock_lock_irqsave(&tty_lock);
    while (len--)
        dev->putc(*s++);
    spinlock_unlock_irqrestore(&tty_lock, irqflags);
}

void tty_clear(void)
{
    unsigned long irqflags;

    if (!dev || !dev->clear)
        return;

    irqflags = spinlock_lock_irqsave(&tty_lock);
    dev->clear();
    spinlock_unlock_irqrestore(&tty_lock, irqflags);
}

void tty_write_raw(const char *s, size_t len)
{
    if (!dev)
        return;
    while (len--)
        dev->putc(*s++);
}