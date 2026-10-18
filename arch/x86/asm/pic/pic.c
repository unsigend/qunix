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

#include "pic.h"

#include <asm/io.h>
#include <asm/pic.h>
#include <asm/vector.h>

#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/printk.h>

static void remap(unsigned int master, unsigned int slave)
{
    /* ICW1: start initialization sequence */
    outb(PIC_MASTER_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC_SLAVE_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /* ICW2: vector offset */
    outb(PIC_MASTER_DATA, master);
    io_wait();
    outb(PIC_SLAVE_DATA, slave);
    io_wait();

    /* ICW3: cascade mode */
    outb(PIC_MASTER_DATA, 1 << PIC_CASCADE_IRQ);
    io_wait();
    outb(PIC_SLAVE_DATA, PIC_CASCADE_IRQ);
    io_wait();

    /* ICW4: environment information */
    outb(PIC_MASTER_DATA, ICW4_8086);
    io_wait();
    outb(PIC_SLAVE_DATA, ICW4_8086);
    io_wait();
}

static void eoi(unsigned int irq)
{
    if (irq >= 8)
        outb(PIC_SLAVE_CMD, PIC_EOI);
    outb(PIC_MASTER_CMD, PIC_EOI);
}

__unused static void mask_all(void)
{
    outb(PIC_MASTER_DATA, 0xff);
    outb(PIC_SLAVE_DATA, 0xff);
}

__unused static void unmask_all(void)
{
    outb(PIC_MASTER_DATA, 0x00);
    outb(PIC_SLAVE_DATA, 0x00);
}

static void mask(unsigned int irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = PIC_MASTER_DATA;
    else {
        port = PIC_SLAVE_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

static void unmask(unsigned int irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = PIC_MASTER_DATA;
    else {
        port = PIC_SLAVE_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

static struct irq_chip pic_chip = {
    .name = "8259 PIC",
    .mask = mask,
    .unmask = unmask,
    .eoi = eoi,
};

void pic_init(void)
{
    int err;

    remap(X86_VECTOR_IRQ_NR_MIN, X86_VECTOR_IRQ_NR_MIN + 8);

    for (unsigned int i = 0; i < X86_VECTOR_IRQ_NR; i++) {
        if ((err = irq_set_chip(i, &pic_chip)))
            panic_error(err, "failed to set irq chip");
    }
    unmask(PIC_CASCADE_IRQ);
    LOGM_INFO("PIC", "8259 PIC initialized successfully");
}