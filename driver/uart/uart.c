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

#include "ns16550.h"

#include <asm/io.h>
#include <asm/irq.h>

#include <kernel/compiler.h>
#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/stdint.h>
#include <kernel/tty.h>

#include <driver/uart.h>

#define UART_8N1                                                               \
    (LCR_DATA_8 | LCR_STOP_1 |                                                 \
     LCR_PARITY_NONE) /* 8 data bits, 1 stop bit, no parity */

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

static void uart_putc(char c)
{
    while (!(inb(UART_COM1 + UART_LSR) & LSR_THRE_MASK))
        ;
    outb(UART_COM1 + UART_THR, c);
}

static struct tty_ops uart_tty_ops = {
    .putc = uart_putc,
};

static irqreturn_t uart_handler(unsigned int irq, void *dev)
{
    UNUSED(irq);
    UNUSED(dev);

    while (inb(UART_COM1 + UART_LSR) & LSR_DR_MASK)
        tty_input(inb(UART_COM1 + UART_RBR));

    return IRQ_HANDLED;
}

void uart_early_init(void)
{
    int err;

    set_baud_rate(UART_COM1, UART_BAUD_RATE);
    outb(UART_COM1 + UART_IER, 0);
    outb(UART_COM1 + UART_LCR, UART_8N1);

    if ((err = tty_register(&uart_tty_ops)))
        panic_error(err, "failed to register UART tty");

    LOGM_INFO("UART", "16550a UART initialized");
}

void uart_init(void)
{
    int err;

    if ((err = irq_register(IRQ_COM1, uart_handler, 0, "16550 UART", NULL)))
        panic_error(err, "failed to register UART IRQ");

    outb(UART_COM1 + UART_MCR, MCR_OUT2_MASK);
    outb(UART_COM1 + UART_IER, IER_RDA_MASK);
}
