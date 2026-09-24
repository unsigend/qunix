/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm.h>
#include <asm/gdt.h>
#include <asm/i8259.h>
#include <asm/idt.h>
#include <asm/isr.h>
#include <asm/traps.h>
#include <qunix/log.h>
#include <stdint.h>

void idtr_write(const struct idtr *idtr)
{
    asm volatile("lidt %0" : : "m"(*idtr));
}

void idtr_read(struct idtr *idtr) { asm volatile("sidt %0" : "=m"(*idtr)); }

void idt_entry_set(struct idt_entry *entry, uint32_t offset, uint16_t selector,
                   uint8_t flags)
{
    entry->offset_low = (offset & 0xFFFF);
    entry->selector = selector;
    entry->reserved = 0; /* always 0 */
    entry->flags = flags;
    entry->offset_high = (offset >> 16) & 0xFFFF;
}

static struct idt_entry idt[IDT_MAX_ENTRIES];

void idt_init(void)
{
    struct idtr idtr = {.size = sizeof(idt) - 1, .base = (uint32_t)idt};
    uint16_t code_sel =
        SEGS_MAKE(SEG_KER_CODE_IDX, SEGS_TI_GDT, SEGS_RPL_RING0);
    uint8_t trap_flags = GATE_MAKE_FLAGS(1, GATE_DPL_RING0, GATE_TYPE_TRAP32);
    uint8_t int_flags = GATE_MAKE_FLAGS(1, GATE_DPL_RING0, GATE_TYPE_INT32);

    (void)trap_flags; /* used later */

    /* System Interrupts (Exceptions) from CPU */
    idt_entry_set(&idt[X86_TRAP_DIVIDE], (uint32_t)&isr0, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_DEBUG], (uint32_t)&isr1, code_sel, trap_flags);
    idt_entry_set(&idt[X86_TRAP_NMI], (uint32_t)&isr2, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_BRKPT], (uint32_t)&isr3, code_sel, trap_flags);
    idt_entry_set(&idt[X86_TRAP_ILLOP], (uint32_t)&isr6, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_DBLFLT], (uint32_t)&isr8, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_STACK], (uint32_t)&isr12, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_GPFLT], (uint32_t)&isr13, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_PGFLT], (uint32_t)&isr14, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_FPERR], (uint32_t)&isr16, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_ALIGN], (uint32_t)&isr17, code_sel, int_flags);
    idt_entry_set(&idt[X86_TRAP_MCHK], (uint32_t)&isr18, code_sel, int_flags);

    /* External Hardware Interrupts */
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_TIMER], (uint32_t)&irq0, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_KBD], (uint32_t)&irq1, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_CASCADE], (uint32_t)&irq2,
                  code_sel, int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_COM2], (uint32_t)&irq3, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_COM1], (uint32_t)&irq4, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_LPT2], (uint32_t)&irq5, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_FLOPPY], (uint32_t)&irq6,
                  code_sel, int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_LPT1], (uint32_t)&irq7, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_CMOS], (uint32_t)&irq8, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_AVAL1], (uint32_t)&irq9, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_AVAL2], (uint32_t)&irq10,
                  code_sel, int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_AVAL3], (uint32_t)&irq11,
                  code_sel, int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_PS2M], (uint32_t)&irq12, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_FPU], (uint32_t)&irq13, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_ATA], (uint32_t)&irq14, code_sel,
                  int_flags);
    idt_entry_set(&idt[X86_IRQ_BASE + X86_IRQ_ATA2], (uint32_t)&irq15, code_sel,
                  int_flags);

    i8259_init(X86_IRQ_BASE, X86_IRQ_BASE + 8); /* master, slave */

    idtr_write(&idtr);
    sti(); /* enable interrupts */

    LOGM(LOG_LEVEL_INFO, "IDT", "IDT initialized successfully");
}