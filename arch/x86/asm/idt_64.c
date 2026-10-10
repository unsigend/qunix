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

#include "desc/idt_desc.h"

#include <asm/idt.h>

__unused static struct idt_entry idt[IDT_NR];

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