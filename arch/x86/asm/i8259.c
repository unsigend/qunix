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

#include <asm/i8259.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/printk.h>

#define PIC1_BASE 0x20
#define PIC2_BASE 0xA0
#define PIC1_CMD_PORT PIC1_BASE        /* PIC Master Command Port */
#define PIC1_DATA_PORT (PIC1_BASE + 1) /* PIC Master Data Port */
#define PIC2_CMD_PORT PIC2_BASE        /* PIC Slave Command Port */
#define PIC2_DATA_PORT (PIC2_BASE + 1) /* PIC Slave Data Port */

#define EOI 0x20 /* End of Interrupt */

/* Initialize the PIC controllers, the ICW stands for Initialization Command
 * Word, the ICW1 is the first command word, ICW2 is the vector base, ICW3 is
 * the master/slave configuration, ICW4 is the extra mode byte. */

#define ICW1_ICW4 0x01      /* ICW4 (if present) is required */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define ICW1_INIT 0x10      /* Initialization */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */

#define CASCADE_IRQ 0x02 /* IRQ for slave PIC */

#define PIC_IRR 0x0A /* Interrupt Request Register (IRR) */
#define PIC_ISR 0x0B /* In-Service Register (ISR) */

static void eoi(unsigned int irq)
{
    if (irq >= 8)
        outb(PIC2_CMD_PORT, EOI);
    outb(PIC1_CMD_PORT, EOI);
}

static void maskall(void)
{
    outb(PIC1_DATA_PORT, 0xFF);
    outb(PIC2_DATA_PORT, 0xFF);
}

__unused static void unmaskall(void)
{
    outb(PIC1_DATA_PORT, 0x00);
    outb(PIC2_DATA_PORT, 0x00);
}

static void mask(unsigned int irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = PIC1_DATA_PORT;
    else {
        port = PIC2_DATA_PORT;
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
        port = PIC1_DATA_PORT;
    else {
        port = PIC2_DATA_PORT;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

static void remap_port(uint8_t master, uint8_t slave)
{
    /* ICW1: start initialization sequence */
    outb(PIC1_CMD_PORT, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD_PORT, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /* ICW2: set vector offset */
    outb(PIC1_DATA_PORT, master);
    io_wait();
    outb(PIC2_DATA_PORT, slave);
    io_wait();

    /* ICW3: master/slave configuration, tell the master PIC that there is a
     * slave PIC at IRQ2 */
    outb(PIC1_DATA_PORT, 1 << CASCADE_IRQ);
    io_wait();
    outb(PIC2_DATA_PORT, CASCADE_IRQ);
    io_wait();

    /* ICW4: extra mode byte, set the PICs use 8086 mode */
    outb(PIC1_DATA_PORT, ICW4_8086);
    io_wait();
    outb(PIC2_DATA_PORT, ICW4_8086);
    io_wait();

    maskall(); /* keep the PICs disabled and unmask later */
}

static struct irq_chip chip = {
    .name = "8259 PIC",
    .mask = mask,
    .unmask = unmask,
    .eoi = eoi,
};

void i8259_init(void)
{
    uint8_t master, slave;

    master = X86_IRQ_BASE;
    slave = X86_IRQ_BASE + 8;

    remap_port(master, slave);

    for (unsigned int i = 0; i < IRQ_NR; i++) {
        if (i == X86_IRQ_CASCADE) {
            unmask(i); /* unmask the cascade IRQ */
            continue;
        }
        if (irq_set_chip(i, &chip) < 0)
            panic("Failed to set IRQ chip line %u for 8259 PIC", i);
    }

    LOGM(LOG_LEVEL_INFO, "8259 PIC",
         "Remapped 8259 PICs IRQ to 0x%02X and 0x%02X", master, slave);
}
