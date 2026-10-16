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

#include "gdt_desc.h"

#include <asm/gdt.h>
#include <asm/segment.h>

#include <kernel/panic.h>

#define ACCESS_MASK 0x92
#define ACCESS_SYS_MASK 0x80
#define FLAGS 0b1100
#define FLAGS_MASK (FLAGS << 4)

static struct gdt_entry gdt[GDT_NR];

struct gdtr {
    uint16_t size;
    uint32_t base;
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

void gdt_set_entry(unsigned int index, unsigned long base, unsigned long limit,
                   unsigned int flags)
{
    struct gdt_entry *entry;

    if (index >= GDT_NR)
        panic("GDT index out of bounds: %u", index);
    if (limit > GDT_LIMIT_MAX)
        panic("GDT limit out of bounds: %lu", limit);

    entry = &gdt[index];

    entry->limit_low = limit & 0xffff;
    entry->flags = FLAGS_MASK | ((limit >> 16) & 0xf);
    entry->access = ACCESS_MASK | flags;
    entry->base_low = base & 0xffff;
    entry->base_middle = (base >> 16) & 0xff;
    entry->base_high = (base >> 24) & 0xff;
}

void gdt_set_sys_entry(unsigned int index, unsigned long base,
                       unsigned long limit, unsigned int flags)
{
    struct gdt_sys_entry *entry;

    if (index >= GDT_NR)
        panic("GDT index out of bounds: %u", index);
    if (limit > GDT_LIMIT_MAX)
        panic("GDT limit out of bounds: %lu", limit);

    entry = &gdt[index];

    entry->limit_low = limit & 0xffff;
    entry->flags = (limit >> 16) & 0xf;
    entry->access = ACCESS_SYS_MASK | flags;
    entry->base_low = base & 0xffff;
    entry->base_middle = (base >> 16) & 0xff;
    entry->base_high = (base >> 24) & 0xff;
}

static void reload_segments(uint16_t code, uint16_t data)
{
    asm volatile("pushl %0\n\t"
                 "pushl $1f\n\t"
                 "lretl\n\t"
                 "1:\n\t"
                 "movw %w1, %%ds\n\t"
                 "movw %w1, %%es\n\t"
                 "movw %w1, %%fs\n\t"
                 "movw %w1, %%gs\n\t"
                 "movw %w1, %%ss\n\t"
                 :
                 : "r"((uint32_t)code), "r"((uint32_t)data)
                 : "memory");
}

void gdt_init(void)
{
    gdt_set_entry(SEG_KCODE_IDX, 0, GDT_LIMIT_MAX,
                  GDT_CODE | GDT_DPL(CPU_RING0));
    gdt_set_entry(SEG_KDATA_IDX, 0, GDT_LIMIT_MAX,
                  GDT_DATA | GDT_DPL(CPU_RING0));
    gdt_set_entry(SEG_UCODE_IDX, 0, GDT_LIMIT_MAX,
                  GDT_CODE | GDT_DPL(CPU_RING3));
    gdt_set_entry(SEG_UDATA_IDX, 0, GDT_LIMIT_MAX,
                  GDT_DATA | GDT_DPL(CPU_RING3));

    gdtr_write(sizeof(gdt) - 1, (uint32_t)gdt);
    reload_segments(SEGSEL_KCODE, SEGSEL_KDATA);
}