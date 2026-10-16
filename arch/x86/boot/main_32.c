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

#include <kernel/compiler.h>
#include <kernel/fmt.h>
#include <kernel/init.h>
#include <kernel/mm/memmap.h>
#include <kernel/printk.h>
#include <kernel/stdint.h>

#include <asm/mb2.h>
#include <asm/memlayout.h>

#include <driver/vga.h>

void bootmain(uint32_t, uint32_t);

static void memmap_parse(const struct multiboot_tag_mmap *mmap)
{
    struct multiboot_mmap_entry *entry;
    uint32_t base, tagend;

    base = (uint32_t)mmap->entries;
    tagend = (uint32_t)mmap + mmap->size;
    entry = (struct multiboot_mmap_entry *)base;

    while (base + mmap->entry_size <= tagend) {
        phys_addr_t end = entry->addr + entry->len;

        if (entry->addr > UINT32_MAX) {
            base += mmap->entry_size;
            entry = (struct multiboot_mmap_entry *)base;
            continue;
        }

        if (entry->addr + entry->len > UINT32_MAX) /* wrap around */
            end = UINT32_MAX;

        switch (entry->type) {
        case MULTIBOOT_MEMORY_AVAILABLE:
            if (memmap_add_region(entry->addr, end, MEM_AVAILABLE))
                panic("failed to add memory region");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
        case MULTIBOOT_MEMORY_NVS:
        case MULTIBOOT_MEMORY_BADRAM:
            if (memmap_add_region(entry->addr, end, MEM_RESERVED))
                panic("failed to add memory region");

            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            if (memmap_add_region(entry->addr, end, MEM_ACPI))
                panic("failed to add memory region");
            break;
        default:
            if (memmap_add_region(entry->addr, end, MEM_RESERVED))
                panic("failed to add memory region");
            break;
        }
        base += mmap->entry_size;
        entry = (struct multiboot_mmap_entry *)base;
    }
}

static void memmap_log(void)
{
    const struct mem_region *r;
    char buf[64];
    size_t bytes = 0;

    for (size_t i = 0; i < memmap_count(); i++) {
        r = memmap_get_region(i);
        if (r->type == MEM_AVAILABLE)
            bytes += r->end - r->start;
        LOGM_INFO("MEMMAP", "Memmap region %-9s: 0x%.8x - 0x%.8x",
                  memmap_type_to_str(r->type), (unsigned int)r->start,
                  (unsigned int)r->end);
    }

    fmt_mem(buf, sizeof(buf), bytes);
    LOGM_INFO("MEMMAP", "Total available RAM memory: %s", buf);
}

void bootmain(uint32_t magic, uint32_t mbi)
{
    struct mb2_header *header;
    struct multiboot_tag *tag;
    uint32_t tag_end;

    vga_init(); /* initialize the VGA text mode driver first,
                   everything else depends on it */

    mb2_validate(magic, mbi);

    header = mb2_get_header(mbi);
    tag = mb2_get_tag(mbi);
    tag_end = (uint32_t)header + header->total_size;

    while ((uint32_t)tag < tag_end) {
        switch (tag->type) {
        case MULTIBOOT_TAG_TYPE_MMAP:
            memmap_parse((struct multiboot_tag_mmap *)tag);
            break;
        }
        tag = mb2_next_tag(tag);
    }

    memmap_log();

    /* reserve kernel image memory */
    if (memmap_punch_region(KERNEL_IMAGE_PHYS_START, KERNEL_IMAGE_PHYS_END,
                            MEM_KERNEL))
        panic("failed to reserve kernel image memory");

    kernel_main();
}