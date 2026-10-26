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

#include "i8042.h"
#include "scancode.h"

#include <asm/irq.h>
#include <asm/kbd.h>

#include <kernel/compiler.h>
#include <kernel/interrupt.h>
#include <kernel/panic.h>
#include <kernel/spinlock.h>
#include <kernel/stddef.h>
#include <kernel/stdint.h>
#include <kernel/string.h>
#include <kernel/waitq.h>

#define RINGBUF_SIZE 128

static const char sc1_ascii_map[128] = {
    [SC1_ESC] = '\033', [SC1_1] = '1',        [SC1_2] = '2',
    [SC1_3] = '3',      [SC1_4] = '4',        [SC1_5] = '5',
    [SC1_6] = '6',      [SC1_7] = '7',        [SC1_8] = '8',
    [SC1_9] = '9',      [SC1_0] = '0',        [SC1_MINUS] = '-',
    [SC1_EQUAL] = '=',  [SC1_BS] = '\b',      [SC1_TAB] = '\t',
    [SC1_Q] = 'q',      [SC1_W] = 'w',        [SC1_E] = 'e',
    [SC1_R] = 'r',      [SC1_T] = 't',        [SC1_Y] = 'y',
    [SC1_U] = 'u',      [SC1_I] = 'i',        [SC1_O] = 'o',
    [SC1_P] = 'p',      [SC1_LBRACKET] = '[', [SC1_RBRACKET] = ']',
    [SC1_ENTER] = '\n', [SC1_A] = 'a',        [SC1_S] = 's',
    [SC1_D] = 'd',      [SC1_F] = 'f',        [SC1_G] = 'g',
    [SC1_H] = 'h',      [SC1_J] = 'j',        [SC1_K] = 'k',
    [SC1_L] = 'l',      [SC1_SEMI] = ';',     [SC1_QUOTE] = '\'',
    [SC1_GRAVE] = '`',  [SC1_BSLASH] = '\\',  [SC1_Z] = 'z',
    [SC1_X] = 'x',      [SC1_C] = 'c',        [SC1_V] = 'v',
    [SC1_B] = 'b',      [SC1_N] = 'n',        [SC1_M] = 'm',
    [SC1_COMMA] = ',',  [SC1_DOT] = '.',      [SC1_SLASH] = '/',
    [SC1_SPACE] = ' ',
};

struct ringbuf {
    char buf[RINGBUF_SIZE];
    size_t head;
    size_t tail;
    size_t size;
};

struct kbd_core {
    struct spinlock lock;
    struct wait_queue waitq;
    struct ringbuf ringbuf;
};

static struct kbd_core kbd_core;

static inline void ringbuf_init(struct ringbuf *buf)
{
    memset(buf, 0, sizeof(*buf));
}

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

static char sc1_to_ascii(uint8_t make)
{
    if (make & SC1_BREAK)
        return 0;
    if (make >= sizeof(sc1_ascii_map))
        return 0;

    return sc1_ascii_map[make];
}

static irqreturn_t kdb_handler(unsigned int irq, void *dev)
{

    UNUSED(irq);
    UNUSED(dev);

    uint8_t sc;
    char c;
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&kbd_core.lock);

    sc = inb(PS2_REG_DATA);
    if (sc == SC1_EXT || sc == 0) {
        spinlock_unlock_irqrestore(&kbd_core.lock, irqflags);
        return IRQ_HANDLED;
    }
    c = sc1_to_ascii(sc);

    if (!ringbuf_is_full(&kbd_core.ringbuf) && c) {
        ringbuf_push(&kbd_core.ringbuf, c);
        waitq_wakeup(&kbd_core.waitq);
    }

    spinlock_unlock_irqrestore(&kbd_core.lock, irqflags);

    return IRQ_HANDLED;
}

void kbd_init(void)
{
    int err;

    spinlock_init(&kbd_core.lock, "kbd");
    waitq_init(&kbd_core.waitq);
    ringbuf_init(&kbd_core.ringbuf);

    if ((err = irq_register(IRQ_KBD, kdb_handler, 0, "PS/2 Keyboard", NULL)))
        panic_error(err, "Failed to register PS/2 Keyboard IRQ");
}

char kbd_getc(void)
{
    unsigned long irqflags;
    char c;

    irqflags = spinlock_lock_irqsave(&kbd_core.lock);

    while (ringbuf_is_empty(&kbd_core.ringbuf))
        waitq_sleep_irq(&kbd_core.waitq, &kbd_core.lock, &irqflags);

    c = ringbuf_pop(&kbd_core.ringbuf);

    spinlock_unlock_irqrestore(&kbd_core.lock, irqflags);

    return c;
}