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

#include <kernel/errno.h>
#include <kernel/panic.h>
#include <kernel/spinlock.h>
#include <kernel/string.h>
#include <kernel/tty.h>
#include <kernel/waitq.h>

#define MAX_TTY_DEVICES 8
#define RINGBUF_SIZE 128

struct ringbuf {
    char buf[RINGBUF_SIZE];
    size_t head;
    size_t tail;
    size_t size;
};

static inline int ringbuf_is_empty(struct ringbuf *buf)
{
    return buf->size == 0;
}

static inline int ringbuf_is_full(struct ringbuf *buf)
{
    return buf->size == RINGBUF_SIZE;
}

static inline void ringbuf_push(struct ringbuf *buf, char c)
{
    buf->buf[buf->tail] = c;
    buf->tail = (buf->tail + 1) % RINGBUF_SIZE;
    buf->size++;
}

static inline char ringbuf_pop(struct ringbuf *buf)
{
    char c = buf->buf[buf->head];
    buf->head = (buf->head + 1) % RINGBUF_SIZE;
    buf->size--;
    return c;
}

struct tty_core {
    struct tty_ops *devs[MAX_TTY_DEVICES];
    size_t ndevs;
    struct ringbuf ringbuf;
    struct wait_queue waitq;
    struct spinlock lock;
};

static struct tty_core tty_core = {
    .devs = {NULL},
    .ndevs = 0,
    .ringbuf =
        {
            .buf = {0},
            .head = 0,
            .tail = 0,
            .size = 0,
        },
    .lock = SPINLOCK_INIT("tty"),
    .waitq = WAITQ_INIT(tty_core.waitq),
};

int tty_register(struct tty_ops *ops)
{
    if (tty_core.ndevs >= MAX_TTY_DEVICES)
        return -ENOSPC;

    if (!ops || !ops->putc)
        return -EINVAL;

    spinlock_lock(&tty_core.lock);
    tty_core.devs[tty_core.ndevs++] = ops;
    spinlock_unlock(&tty_core.lock);
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
    if (ringbuf_is_full(&tty_core.ringbuf)) {
        spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
        return;
    }
    ringbuf_push(&tty_core.ringbuf, c);
    waitq_wakeup(&tty_core.waitq);
    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}

void tty_read(char *buf, size_t len)
{
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&tty_core.lock);

    for (size_t i = 0; i < len; i++) {
        while (ringbuf_is_empty(&tty_core.ringbuf))
            waitq_sleep_irq(&tty_core.waitq, &tty_core.lock, &irqflags);
        buf[i] = ringbuf_pop(&tty_core.ringbuf);
    }

    spinlock_unlock_irqrestore(&tty_core.lock, irqflags);
}