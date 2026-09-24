/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/mb2.h>
#include <boot/multiboot2.h>
#include <qunix/fmt.h>
#include <qunix/kernel.h>
#include <qunix/log.h>
#include <qunix/mm/memlayout.h>
#include <qunix/mm/memmap.h>

void memmap_init(void)
{
    struct multiboot_tag *tag =
        mb2_find_tag(mb2_get_mbi(), MULTIBOOT_TAG_TYPE_MMAP);
    if (!tag)
        panic("missing memory map tag in multiboot2 information");

    struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;

    for (uintptr_t p = (uintptr_t)mmap->entries;
         p < (uintptr_t)mmap + mmap->size; p += mmap->entry_size) {
        struct multiboot_mmap_entry *e = (struct multiboot_mmap_entry *)p;
        if (e->type == MULTIBOOT_MEMORY_AVAILABLE) {
            if (mem_map_add(e->addr, e->len, MEMMAP_AVAILABLE) < 0) {
                panic("failed to add memory map entry 0x%.8llx - 0x%.8llx",
                      e->addr, e->addr + e->len);
            }
        } else {
            if (mem_map_add(e->addr, e->len, MEMMAP_RESERVED) < 0) {
                panic("failed to add memory map entry 0x%.8llx - 0x%.8llx",
                      e->addr, e->addr + e->len);
            }
        }
    }

    char buf[64];
    fmt_mem(buf, sizeof(buf), mem_map_get_available());
    LOGM(LOG_LEVEL_INFO, "MEMMAP", "Total available memory: %s", buf);

    if (mem_map_add(KERNEL_PHYS_START, KERNEL_PHYS_SIZE, MEMMAP_KERNEL) < 0)
        panic("failed to add kernel memory map entry 0x%.8zx - 0x%.8zx",
              KERNEL_PHYS_START, KERNEL_PHYS_START + KERNEL_PHYS_SIZE);
    fmt_mem(buf, sizeof(buf), mem_map_get_kernel());
    LOGM(LOG_LEVEL_INFO, "MEMMAP", "Kernel memory: %s", buf);

    /* Since multiboot2 information struct is not mark as reserved so after
     * this, it is not safe to use it. */
    mb2_set_mbi(0);

    LOGM(LOG_LEVEL_INFO, "MEMMAP", "Memory Map initialized successfully");
}
