/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/multiboot2.h>
#include <qunix/kernel.h>
#include <qunix/mm/memmap.h>
#include <qunix/mm/mm.h>
#include <stdint.h>

/* Print the memory size in a human readable format */
static void printmm(uint64_t bytes)
{
    static const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    int unit_index = 0;

    while (bytes >= 1024 && unit_index < (int)ARRAY_SIZE(units) - 1) {
        bytes /= 1024;
        unit_index++;
    }

    printk("%llu %s\n", bytes, units[unit_index]);
}

static void memmap_init(void *ctx)
{
    uintptr_t base = (uintptr_t)ctx;
    uint32_t size = *(uint32_t *)base;
    base += sizeof(uint32_t) * 2; /* skip size and reserved fields */

    while (base < (uintptr_t)ctx + size) {
        struct multiboot_tag *tag = (struct multiboot_tag *)base;

        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
            for (uintptr_t p = (uintptr_t)mmap->entries;
                 p < (uintptr_t)mmap + mmap->size; p += mmap->entry_size) {
                struct multiboot_mmap_entry *entry =
                    (struct multiboot_mmap_entry *)p;
                if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
                    if (mem_map_add(entry->addr, entry->len, MEMMAP_AVAILABLE) <
                        0) {
                        panic("failed to add memory map entry %llx - %llx",
                              entry->addr, entry->addr + entry->len);
                    }
                } else {
                    if (mem_map_add(entry->addr, entry->len, MEMMAP_RESERVED) <
                        0) {
                        panic("failed to add memory map entry %llx - %llx",
                              entry->addr, entry->addr + entry->len);
                    }
                }
            }
        }
        base = ALIGN(base + tag->size, MULTIBOOT_TAG_ALIGN);
    }
}

void mm_init(void *ctx)
{
    memmap_init(ctx);
    printk("[INIT] Memory map initialized successfully\n");
    printk("[INFO] Total memory: ");
    printmm(mem_map_get_total());
}