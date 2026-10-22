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

#include <kernel/cmdline.h>
#include <kernel/compiler.h>
#include <kernel/fmt.h>
#include <kernel/init.h>
#include <kernel/mm/memmap.h>
#include <kernel/printk.h>
#include <kernel/stdint.h>
#include <kernel/string.h>

#include <asm/mb2.h>
#include <asm/memlayout.h>

#include <driver/vga.h>

void bootmain(uint32_t, uint32_t);

/* symbols from ld/ld-i386.ld */
extern char _reclaimable_phys_start[];
extern char _reclaimable_phys_end[];

static void memmap_parse(const struct multiboot_tag_mmap *mmap)
{
    struct multiboot_mmap_entry *entry;
    uint32_t base, tagend;
    int err;

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
            if ((err = memmap_add_region(entry->addr, end, MEM_AVAILABLE)))
                panic_error(err, "failed to add memory region");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
        case MULTIBOOT_MEMORY_NVS:
        case MULTIBOOT_MEMORY_BADRAM:
            if ((err = memmap_add_region(entry->addr, end, MEM_RESERVED)))
                panic_error(err, "failed to add memory region");

            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            if ((err = memmap_add_region(entry->addr, end, MEM_ACPI)))
                panic_error(err, "failed to add memory region");
            break;
        default:
            if ((err = memmap_add_region(entry->addr, end, MEM_RESERVED)))
                panic_error(err, "failed to add memory region");
            break;
        }
        base += mmap->entry_size;
        entry = (struct multiboot_mmap_entry *)base;
    }
}

static void cmdline_parse(const struct multiboot_tag_string *cmdline)
{
    char *s, *key, *val, *sep;
    int err;

    s = (char *)cmdline->string;
    while (*s) {
        while (*s == ' ')
            s++;
        if (!*s)
            break;

        key = s;
        while (*s && *s != ' ')
            s++;
        if (*s)
            *s++ = '\0';

        val = NULL;
        sep = strchr(key, '=');
        if (sep) {
            *sep = '\0';
            val = sep + 1;
        }

        if (!key[0])
            continue;

        if ((err = cmdline_set(key, val)))
            panic_error(err, "failed to set cmdline");
    }
}

static void memmap_log(void)
{
    struct mem_region r;
    char buf[64];
    size_t bytes = 0;

    for (size_t i = 0; i < memmap_count(); i++) {
        memmap_get_region(i, &r);
        if (r.type == MEM_AVAILABLE)
            bytes += r.end - r.start;
        LOGM_INFO("MEMMAP", "Memmap region %-9s: 0x%.8x - 0x%.8x",
                  memmap_type_to_str(r.type), (unsigned int)r.start,
                  (unsigned int)r.end);
    }

    fmt_mem(buf, sizeof(buf), bytes);
    LOGM_INFO("MEMMAP", "Total available RAM memory: %s", buf);
}

void bootmain(uint32_t magic, uint32_t mbi)
{
    struct mb2_header *header;
    struct multiboot_tag *tag;
    uint32_t tag_end;
    int err;

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
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            cmdline_parse((struct multiboot_tag_string *)tag);
            break;
        }

        tag = mb2_next_tag(tag);
    }

    memmap_log();

    /* Reserve the kernel image, then reclaim the temporary boot data to
     * available memory. pmm_init walks the available regions once to build
     * the free list, so both updates must be done before pmm_init. */

    /* reserve kernel image memory */
    if ((err = memmap_punch_region(KERNEL_IMAGE_PHYS_START,
                                   KERNEL_IMAGE_PHYS_END, MEM_KERNEL)))
        panic_error(err, "failed to reserve kernel image memory");

    /* reclaim the memory region from temporary boot data section */
    if ((err = memmap_punch_region((phys_addr_t)_reclaimable_phys_start,
                                   (phys_addr_t)_reclaimable_phys_end,
                                   MEM_AVAILABLE)))
        panic_error(err, "failed to punch reclaimable memory");

    kernel_main(); /* pass control to the kernel */
}