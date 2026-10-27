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

#include <kernel/cdev.h>
#include <kernel/compiler.h>
#include <kernel/errno.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/ringbuf.h>
#include <kernel/spinlock.h>
#include <kernel/string.h>
#include <kernel/tty.h>
#include <kernel/waitq.h>

#define MAX_TTY_DEVICES 8
#define TTY_INBUF_SIZE 128
#define TTY_MAJOR 4

struct tty_core {
    struct tty_ops *devs[MAX_TTY_DEVICES];
    size_t ndevs;
    struct ringbuf ringbuf;
    struct wait_queue waitq;
    struct spinlock lock;
};

static char tty_inbuf[TTY_INBUF_SIZE];

static struct tty_core tty_core = {
    .devs = {NULL},
    .ndevs = 0,
    .ringbuf = RINGBUF_INIT(tty_inbuf, TTY_INBUF_SIZE, sizeof(char)),
    .lock = SPINLOCK_INIT("tty"),
    .waitq = WAITQ_INIT(tty_core.waitq),
};

int tty_register(struct tty_ops *ops)
{
    unsigned long irqflags;

    if (tty_core.ndevs >= MAX_TTY_DEVICES)
        return -ENOSPC;

    if (!ops || !ops->putc)
        return -EINVAL;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);
    tty_core.devs[tty_core.ndevs++] = ops;
    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
    return 0;
}

void tty_putc(char c)
{
    unsigned long irqflags;
    if (tty_core.ndevs == 0)
        return;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);

    for (size_t i = 0; i < tty_core.ndevs; i++)
        tty_core.devs[i]->putc(c);

    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}

void tty_write(const char *s, size_t len)
{
    unsigned long irqflags;

    if (tty_core.ndevs == 0)
        return;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);
    for (size_t i = 0; i < tty_core.ndevs; i++) {
        for (size_t j = 0; j < len; j++)
            tty_core.devs[i]->putc(s[j]);
    }
    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}

void tty_clear(void)
{
    unsigned long irqflags;

    if (tty_core.ndevs == 0)
        return;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);
    for (size_t i = 0; i < tty_core.ndevs; i++) {
        if (tty_core.devs[i]->clear)
            tty_core.devs[i]->clear();
    }
    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}

void tty_write_raw(const char *s, size_t len)
{
    if (tty_core.ndevs == 0)
        return;

    for (size_t i = 0; i < tty_core.ndevs; i++) {
        for (size_t j = 0; j < len; j++)
            tty_core.devs[i]->putc(s[j]);
    }
}

void tty_input(char c)
{
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);
    if (!ringbuf_push(&tty_core.ringbuf, &c))
        waitq_wakeup(&tty_core.waitq);
    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}

void tty_read(char *buf, size_t len)
{
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);

    for (size_t i = 0; i < len; i++) {
        while (ringbuf_pop(&tty_core.ringbuf, &buf[i]))
            waitq_sleep_irq(&tty_core.waitq, &tty_core.lock, &irqflags);
    }

    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}

static ssize_t tty_cdev_read(struct cdev *cdev, void *buf, size_t count)
{
    UNUSED(cdev);

    tty_read(buf, count);
    return count;
}

static ssize_t tty_cdev_write(struct cdev *cdev, const void *buf, size_t count)
{
    UNUSED(cdev);

    tty_write(buf, count);
    return count;
}

static const struct cdev_ops tty_cdev_ops = {
    .read = tty_cdev_read,
    .write = tty_cdev_write,
};

static struct cdev tty_cdev = {
    .major = TTY_MAJOR,
    .minor = 0,
    .ops = &tty_cdev_ops,
};

void tty_init(void)
{
    int err;

    if ((err = cdev_register(&tty_cdev)))
        panic_error(err, "failed to register tty cdev");

    LOGM_INFO("TTY", "TTY initialized successfully");
}