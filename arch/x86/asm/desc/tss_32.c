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

#include "tss_desc.h"

#include <asm/gdt.h>
#include <asm/segment.h>
#include <asm/tss.h>

#include <kernel/cpu.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/types.h>

static struct tss_entry tss_entries[MAX_CPU];

static inline struct tss_entry *get_cpu_tss(cpuid_t cpuid)
{
    return &tss_entries[cpuid];
}

void tss_init_cpu(cpuid_t cpuid)
{
    struct tss_entry *tss;

    if (cpuid >= MAX_CPU)
        panic("CPU ID out of bounds: %u", cpuid);

    tss = get_cpu_tss(cpuid);
    tss->ss0 = SEGSEL_KDATA;
    tss->iopb = sizeof(struct tss_entry); /* no I/O permissions */

    gdt_set_sys_entry(
        cpuid, SEG_TSS_IDX, (unsigned long)tss, sizeof(struct tss_entry) - 1,
        GDT_P | GDT_DPL(CPU_RING0) | GDT_S_SYS | GDT_G_1B | GDT_TSS);

    asm volatile("ltr %w0" : : "r"(SEGSEL_TSS));

    LOGM_INFO("TSS", "TSS initialized successfully for CPU %u", cpuid);
}

void tss_set_kstack(cpuid_t cpuid, uintptr_t stacktop)
{
    struct tss_entry *tss;

    if (cpuid >= MAX_CPU)
        panic("CPU ID out of bounds: %u", cpuid);

    tss = get_cpu_tss(cpuid);
    tss->esp0 = stacktop;
}