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

#include "desc/gdt_desc.h"

#include <asm/gdt.h>

#include <kernel/compiler.h>

static struct gdt_entry gdt[GDT_NR];

struct gdtr {
    uint16_t size; /* size of the table in bytes minus 1 */
    uint64_t base; /* base address of the table */
} __packed;

void gdtr_read(unsigned short *size, unsigned long *base)
{
    struct gdtr gdtr;
    asm volatile("sgdt %0" : "=m"(gdtr));
    *size = gdtr.size;
    *base = gdtr.base;
}

void gdtr_write(unsigned short size, unsigned long base)
{
    struct gdtr gdtr = {.size = size, .base = base};
    asm volatile("lgdt %0" : : "m"(gdtr));
}