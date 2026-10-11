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

#include "idt_desc.h"

#include <asm/idt.h>

#include <kernel/panic.h>

#define MASK (1u << 15)

static struct idt_entry idt[IDT_NR];

struct idtr {
    uint16_t size;
    uint64_t base;
} __packed;

void idtr_read(unsigned short *size, unsigned long *base)
{
    struct idtr idtr;
    asm volatile("sidt %0" : "=m"(idtr));
    *size = idtr.size;
    *base = idtr.base;
}

void idtr_write(unsigned short size, unsigned long base)
{
    struct idtr idtr = {.size = size, .base = base};
    asm volatile("lidt %0" : : "m"(idtr));
}

void idt_set_entry(unsigned int index, unsigned short selector,
                   unsigned long addr, unsigned int flags)
{
    struct idt_entry *entry;

    if (index >= IDT_NR)
        panic("IDT index out of bounds: %u", index);

    entry = &idt[index];

    entry->offset_low = addr & 0xFFFF;
    entry->offset_middle = (addr >> 16) & 0xFFFF;
    entry->offset_high = (addr >> 32) & 0xFFFFFFFF;
    entry->selector = selector;
    entry->reserved = 0;
    entry->flags = flags | MASK;
}