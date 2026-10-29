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

#include <kernel/cpu.h>
#include <kernel/panic.h>
#include <kernel/string.h>

struct gdtr {
    uint16_t size;
    uint64_t base;
} __packed;

struct gdt_cpu_table {
    struct gdt_entry entries[GDT_NR];
};

static struct gdt_cpu_table gdt_cpu_tables[CPU_NR];

static inline struct gdt_cpu_table *get_cpu_table(cpuid_t cpuid)
{
    return &gdt_cpu_tables[cpuid];
}

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

void gdt_set_entry(cpuid_t cpuid, unsigned int index, unsigned long base,
                   unsigned long limit, unsigned int flags)
{
    struct gdt_entry *entry;
    uint8_t flag, access;

    if (index >= GDT_NR)
        panic("GDT index out of bounds: %u", index);
    if (limit > GDT_LIMIT_MAX)
        panic("GDT limit out of bounds: %lu", limit);
    if (cpuid >= CPU_NR)
        panic("CPU ID out of bounds: %u", cpuid);

    entry = &get_cpu_table(cpuid)->entries[index];
    flag = (flags >> 8) & 0xf;
    access = flags & 0xff;

    entry->limit_low = limit & 0xffff;
    entry->flags = ((limit >> 16) & 0xf) | (flag << 4);
    entry->access = access;
    entry->base_low = base & 0xffff;
    entry->base_middle = (base >> 16) & 0xff;
    entry->base_high = (base >> 24) & 0xff;
}

void gdt_set_sys_entry(cpuid_t cpuid, unsigned int index, unsigned long base,
                       unsigned long limit, unsigned int flags)
{
    struct gdt_sys_entry entry;
    uint8_t flag, access;
    void *p;

    if (index >= GDT_NR - 1)
        panic("GDT index out of bounds: %u", index);
    if (limit > GDT_LIMIT_MAX)
        panic("GDT limit out of bounds: %lu", limit);
    if (cpuid >= CPU_NR)
        panic("CPU ID out of bounds: %u", cpuid);

    p = (void *)&get_cpu_table(cpuid)->entries[index];
    flag = (flags >> 8) & 0xf;
    access = flags & 0xff;

    entry.limit_low = limit & 0xffff;
    entry.flags = ((limit >> 16) & 0xf) | (flag << 4);
    entry.access = access;
    entry.base_low = base & 0xffff;
    entry.base_middle1 = (base >> 16) & 0xff;
    entry.base_middle2 = (base >> 24) & 0xff;
    entry.base_high = (base >> 32) & 0xffffffff;
    entry.reserved = 0;

    memcpy(p, &entry, sizeof(entry));
}

static void flush_segments(uint16_t code, uint16_t data)
{
    unsigned long rip;

    asm volatile("pushq %1\n\t"
                 "leaq 1f(%%rip), %0\n\t"
                 "pushq %0\n\t"
                 "lretq\n\t"
                 "1:\n\t"
                 "movw %w2, %%ds\n\t"
                 "movw %w2, %%es\n\t"
                 "movw %w2, %%fs\n\t"
                 "movw %w2, %%gs\n\t"
                 "movw %w2, %%ss\n\t"
                 : "=&r"(rip)
                 : "r"((unsigned long)code), "r"((unsigned long)data)
                 : "memory");
}

void gdt_init_cpu(cpuid_t cpuid)
{
    struct gdt_cpu_table *table;
    struct gdtr gdtr;

    if (cpuid >= CPU_NR)
        panic("CPU ID out of bounds: %u", cpuid);

    table = get_cpu_table(cpuid);

    gdt_set_entry(cpuid, 0, 0, 0, 0);
    gdt_set_entry(cpuid, SEG_KCODE_IDX, 0, GDT_LIMIT_MAX,
                  GDT_P | GDT_DPL(CPU_RING0) | GDT_S_CD | GDT_LONG | GDT_G_4K |
                      GDT_EXEC | GDT_RW);
    gdt_set_entry(cpuid, SEG_KDATA_IDX, 0, GDT_LIMIT_MAX,
                  GDT_P | GDT_DPL(CPU_RING0) | GDT_S_CD | GDT_DB_32 | GDT_G_4K |
                      GDT_RW);
    gdt_set_entry(cpuid, SEG_UCODE_IDX, 0, GDT_LIMIT_MAX,
                  GDT_P | GDT_DPL(CPU_RING3) | GDT_S_CD | GDT_LONG | GDT_G_4K |
                      GDT_EXEC | GDT_RW);
    gdt_set_entry(cpuid, SEG_UDATA_IDX, 0, GDT_LIMIT_MAX,
                  GDT_P | GDT_DPL(CPU_RING3) | GDT_S_CD | GDT_DB_32 | GDT_G_4K |
                      GDT_RW);

    gdtr.size = sizeof(table->entries) - 1;
    gdtr.base = (uint64_t)table->entries;
    gdtr_write(gdtr.size, gdtr.base);

    flush_segments(SEGSEL_KCODE, SEGSEL_KDATA);
}