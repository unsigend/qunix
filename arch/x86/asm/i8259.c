/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm.h>
#include <asm/i8259.h>
#include <qunix/irq.h>
#include <qunix/log.h>
#include <stdint.h>

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

void irq_eoi(uint32_t irq)
{
    if (irq >= 8)
        outb(PIC2_CMD_PORT, EOI);
    outb(PIC1_CMD_PORT, EOI);
}

void irq_enable(void) { sti(); }

static struct i8259_isr i8259_read_isr(uint8_t reg)
{
    struct i8259_isr isr;

    outb(PIC1_CMD_PORT, reg);
    outb(PIC2_CMD_PORT, reg);

    isr.master = inb(PIC1_CMD_PORT);
    isr.slave = inb(PIC2_CMD_PORT);

    return isr;
}

i8259_irr i8259_get_irr(void) { return i8259_read_isr(PIC_IRR); }
i8259_isr i8259_get_isr(void) { return i8259_read_isr(PIC_ISR); }

static void i8259_remap(uint8_t master, uint8_t slave)
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

    i8259_mask_all(); /* keep the PICs disabled and unmask later */
}

void i8259_set_mask(uint8_t irq)
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

void i8259_clear_mask(uint8_t irq)
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

void i8259_init(uint8_t master, uint8_t slave)
{
    i8259_remap(master, slave);
    LOGM(LOG_LEVEL_INFO, "8259 PIC",
         "Remapped 8259 PICs IRQ to 0x%02X and 0x%02X", master, slave);
}

void i8259_mask_all(void)
{
    outb(PIC1_DATA_PORT, 0xFF);
    outb(PIC2_DATA_PORT, 0xFF);
}

void i8259_unmask_all(void)
{
    outb(PIC1_DATA_PORT, 0x00);
    outb(PIC2_DATA_PORT, 0x00);
}