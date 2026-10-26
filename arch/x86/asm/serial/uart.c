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

#include "uart.h"

#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/spinlock.h>
#include <kernel/stddef.h>
#include <kernel/stdint.h>
#include <kernel/string.h>
#include <kernel/tty.h>
#include <kernel/waitq.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uart.h>

#define UART_8N1                                                               \
    (LCR_DATA_8 | LCR_STOP_1 |                                                 \
     LCR_PARITY_NONE) /* 8 data bits, 1 stop bit, no parity */

#define RINGBUF_SIZE 128

struct ringbuf {
    char buf[RINGBUF_SIZE];
    size_t head;
    size_t tail;
    size_t size;
};

struct uart_core {
    struct spinlock lock;
    struct wait_queue waitq;
    struct ringbuf ringbuf;
};

static struct uart_core uart_core;

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

static inline void set_baud_rate(uint16_t port, unsigned long rate)
{
    uint16_t div;
    uint8_t old_lcr;

    div = UART_BAUD_RATE / rate;
    old_lcr = inb(port + UART_LCR);

    outb(port + UART_LCR, old_lcr | (1 << LCR_DLAB));
    outb(port + UART_DLL, div & 0xFF);
    outb(port + UART_DLM, div >> 8);
    outb(port + UART_LCR, old_lcr & ~(1 << LCR_DLAB));
}

static irqreturn_t uart_handle_read(unsigned int irq, void *dev)
{
    UNUSED(irq);
    UNUSED(dev);

    unsigned long irqflags;
    char c;

    irqflags = spinlock_lock_irqsave(&uart_core.lock);

    while (inb(UART_COM1 + UART_LSR) & LSR_DR_MASK) {
        c = inb(UART_COM1 + UART_RBR);
        if (!ringbuf_is_full(&uart_core.ringbuf)) {
            ringbuf_push(&uart_core.ringbuf, c);
            waitq_wakeup(&uart_core.waitq);
        }
        tty_input(c);
    }

    spinlock_unlock_irqrestore(&uart_core.lock, irqflags);

    return IRQ_HANDLED;
}

void uart_init(void)
{
    set_baud_rate(UART_COM1, UART_BAUD_RATE);
    outb(UART_COM1 + UART_IER, 0);
    outb(UART_COM1 + UART_LCR, UART_8N1);

    LOGM_INFO("UART", "16550a UART initialized");
}

void uart_init_irq(void)
{
    int err;

    outb(UART_COM1 + UART_MCR, MCR_OUT2_MASK);
    outb(UART_COM1 + UART_IER, IER_RDA_MASK);

    spinlock_init(&uart_core.lock, "uart_core");
    waitq_init(&uart_core.waitq);
    ringbuf_init(&uart_core.ringbuf);

    if ((err = irq_register(IRQ_COM1, uart_handle_read, 0, "16550 UART", NULL)))
        panic_error(err, "failed to register UART IRQ handler");
}

void uart_putc(char c)
{
    /* sync write */
    while (!(inb(UART_COM1 + UART_LSR) & LSR_THRE_MASK))
        ;
    outb(UART_COM1 + UART_THR, c);
}

char uart_getc(void)
{
    unsigned long irqflags;
    char c;

    irqflags = spinlock_lock_irqsave(&uart_core.lock);

    while (ringbuf_is_empty(&uart_core.ringbuf))
        waitq_sleep_irq(&uart_core.waitq, &uart_core.lock, &irqflags);
    c = ringbuf_pop(&uart_core.ringbuf);

    spinlock_unlock_irqrestore(&uart_core.lock, irqflags);

    return c;
}