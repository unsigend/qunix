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

#include <kernel/printk.h>

void gdtr_write(const struct gdtr *gdtr)
{
    asm volatile("lgdt %0" : : "m"(*gdtr));
}

void gdtr_read(struct gdtr *gdtr) { asm volatile("sgdt %0" : "=m"(*gdtr)); }

void gdt_entry_set(struct gdt_entry *entry, uint32_t base, uint32_t limit,
                   uint8_t flags, uint8_t access)
{
    entry->limit_low = (limit & 0xFFFF);
    entry->granularity = ((flags & 0x0F) << 4) | ((limit >> 16) & 0x0F);
    entry->base_low = (base & 0xFFFF);
    entry->base_mid = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;
    entry->access = access;
}

static struct gdt_entry gdt[SEG_MAX_ENTRIES];

/* Reload the segment descriptors */
static void reload_segments(seg_selector_t code, seg_selector_t data)
{
    asm volatile("pushl %0\n\t"  /* push code selector */
                 "pushl $1f\n\t" /* push return address */
                 "lretl\n\t"     /* far return */
                 "1:\n\t"
                 "movw %w1, %%ds\n\t"
                 "movw %w1, %%es\n\t"
                 "movw %w1, %%fs\n\t"
                 "movw %w1, %%gs\n\t"
                 "movw %w1, %%ss\n\t"
                 :
                 : "r"((uint32_t)code), "r"(data)
                 : "memory");
}

void gdt_init(void)
{
    struct gdtr gdtr = {.size = sizeof(gdt) - 1, .base = (uint32_t)gdt};
    uint8_t flag = (SEG_G_4KB << 3) | (SEG_DB_32 << 2);
    uint8_t acc_mask = 0b10000000;

    uint8_t kernel_code =
        acc_mask | (SEG_DPL_RING0 << 5) | (SEG_S_CODEDATA << 4) | SEG_CODE_EXRD;
    uint8_t kernel_data =
        acc_mask | (SEG_DPL_RING0 << 5) | (SEG_S_CODEDATA << 4) | SEG_DATA_RDWR;
    uint8_t user_code =
        acc_mask | (SEG_DPL_RING3 << 5) | (SEG_S_CODEDATA << 4) | SEG_CODE_EXRD;
    uint8_t user_data =
        acc_mask | (SEG_DPL_RING3 << 5) | (SEG_S_CODEDATA << 4) | SEG_DATA_RDWR;

    gdt_entry_set(&gdt[SEG_NULL_IDX], 0, 0, 0, 0);
    gdt_entry_set(&gdt[SEG_KER_CODE_IDX], 0, 0xFFFFF, flag, kernel_code);
    gdt_entry_set(&gdt[SEG_KER_DATA_IDX], 0, 0xFFFFF, flag, kernel_data);
    gdt_entry_set(&gdt[SEG_USER_CODE_IDX], 0, 0xFFFFF, flag, user_code);
    gdt_entry_set(&gdt[SEG_USER_DATA_IDX], 0, 0xFFFFF, flag, user_data);

    gdtr_write(&gdtr);
    reload_segments(SEGS_MAKE(SEG_KER_CODE_IDX, SEGS_TI_GDT, SEGS_RPL_RING0),
                    SEGS_MAKE(SEG_KER_DATA_IDX, SEGS_TI_GDT, SEGS_RPL_RING0));

    LOGM(LOG_LEVEL_INFO, "GDT", "GDT initialized successfully");
}