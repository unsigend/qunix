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

#include <asm/cpu.h>
#include <asm/gdt.h>
#include <asm/idt.h>

#include <kernel/printk.h>

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

    UNUSED(code_sel);
    UNUSED(trap_flags);
    UNUSED(int_flags);

    /* TODO : implement this */

    idtr_write(&idtr);

    LOGM(LOG_LEVEL_INFO, "IDT", "IDT initialized successfully");
}