/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/mb2.h>
#include <boot/multiboot2.h>
#include <qunix/kernel.h>
#include <qunix/mm/memmap.h>
#include <stdint.h>

void memmap_init(void)
{
    struct multiboot_tag *tag =
        mb2_find_tag(mb2_get_mbi(), MULTIBOOT_TAG_TYPE_MMAP);
    if (!tag)
        panic("Missing memory map tag in multiboot2 information");

    struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;

    for (uintptr_t p = (uintptr_t)mmap->entries;
         p < (uintptr_t)mmap + mmap->size; p += mmap->entry_size) {
        struct multiboot_mmap_entry *e = (struct multiboot_mmap_entry *)p;
        if (e->type == MULTIBOOT_MEMORY_AVAILABLE) {
            if (mem_map_add(e->addr, e->len, MEMMAP_AVAILABLE) < 0) {
                panic("failed to add memory map entry %llx - %llx", e->addr,
                      e->addr + e->len);
            }
        } else {
            if (mem_map_add(e->addr, e->len, MEMMAP_RESERVED) < 0) {
                panic("failed to add memory map entry %llx - %llx", e->addr,
                      e->addr + e->len);
            }
        }
    }
}
