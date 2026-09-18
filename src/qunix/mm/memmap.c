/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/error.h>
#include <qunix/mm/memmap.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_ENTRIES 64

static struct mem_map_entry entries[MAX_ENTRIES];
static size_t num_entries = 0;

const struct mem_map_entry *mem_map_get(void) { return entries; }
size_t mem_map_get_size(void) { return num_entries; }

uint64_t mem_map_get_total(void)
{
    uint64_t total = 0;
    for (size_t i = 0; i < num_entries; i++) {
        if (entries[i].type == MEMMAP_AVAILABLE)
            total += entries[i].len;
    }

    return total;
}

const struct mem_map_entry *mem_map_find(uint64_t addr, uint64_t len)
{
    for (size_t i = 0; i < num_entries; i++) {
        if (entries[i].addr == addr && entries[i].len == len)
            return &entries[i];
    }

    return NULL;
}

const struct mem_map_entry *mem_map_find_type(uint64_t addr, uint64_t len,
                                              enum mem_map_type type)
{
    for (size_t i = 0; i < num_entries; i++) {
        if (entries[i].addr == addr && entries[i].len == len &&
            entries[i].type == type)
            return &entries[i];
    }

    return NULL;
}

static inline int overlaps(const struct mem_map_entry *e, uint64_t addr,
                           uint64_t len)
{
    return addr >= e->addr && len <= e->len && (addr - e->addr) <= e->len - len;
}

int mem_map_add(uint64_t addr, uint64_t len, enum mem_map_type type)
{
    if (!len || addr + len < addr)
        return -EINVAL;

    for (size_t i = 0; i < num_entries; i++) {
        if (overlaps(&entries[i], addr, len)) {
            if (entries[i].type == type)
                return 0;

            return -EEXIST;
        }
    }

    if (num_entries >= MAX_ENTRIES)
        return -ENOMEM;

    entries[num_entries].addr = addr;
    entries[num_entries].len = len;
    entries[num_entries].type = type;
    num_entries++;

    return 0;
}