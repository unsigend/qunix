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
#include <string.h>

#define MAX_ENTRIES 64

static struct mem_map_entry entries[MAX_ENTRIES];
static size_t num_entries = 0;

const struct mem_map_entry *mem_map_get(void) { return entries; }
size_t mem_map_get_size(void) { return num_entries; }

static inline uint64_t sum(enum mem_map_type type)
{
    uint64_t total = 0;
    for (size_t i = 0; i < num_entries; i++)
        if (entries[i].type == type)
            total += entries[i].len;

    return total;
}

uint64_t mem_map_get_available(void) { return sum(MEMMAP_AVAILABLE); }
uint64_t mem_map_get_reserved(void) { return sum(MEMMAP_RESERVED); }
uint64_t mem_map_get_kernel(void) { return sum(MEMMAP_KERNEL); }

const struct mem_map_entry *mem_map_find(uint64_t addr)
{
    for (size_t i = 0; i < num_entries; i++) {
        if (entries[i].addr == addr)
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

const struct mem_map_entry *mem_map_find_range(uint64_t addr, uint64_t len)
{
    for (size_t i = 0; i < num_entries; i++) {
        if (entries[i].addr <= addr &&
            entries[i].addr + entries[i].len >= addr + len)
            return &entries[i];
    }

    return NULL;
}

/* Return 1 if the two entries overlap, 0 otherwise. */
static inline int overlaps(const struct mem_map_entry *e1,
                           const struct mem_map_entry *e2)
{
    return e1->addr < e2->addr + e2->len && e1->addr + e1->len > e2->addr;
}

int mem_map_add(uint64_t addr, uint64_t len, enum mem_map_type type)
{
    if (addr + len < addr)
        return -EINVAL;

    if (type == MEMMAP_KERNEL) {
        struct mem_map_entry *block = NULL;
        struct mem_map_entry tmp;

        for (size_t i = 0; i < num_entries; i++) {
            if (entries[i].type == MEMMAP_AVAILABLE &&
                addr >= entries[i].addr &&
                addr + len <= entries[i].addr +
                                  entries[i].len) /* punch is only allowed in
                                                     the available range */
            {
                block = &entries[i];
                break;
            }
        }
        if (!block)
            return -ENOMEM;

        memcpy(&tmp, block, sizeof(struct mem_map_entry));
        block->addr = addr;
        block->len = len;
        block->type = MEMMAP_KERNEL;

        int err = 0, added = 0;

        if (addr > tmp.addr) /* left split */
        {
            if ((err = mem_map_add(tmp.addr, addr - tmp.addr,
                                   MEMMAP_AVAILABLE)) < 0)
                goto rollback;
            added = 1;
        }

        if (addr + len < tmp.addr + tmp.len &&
            (err = mem_map_add(addr + len, tmp.addr + tmp.len - (addr + len),
                               MEMMAP_AVAILABLE)) < 0)
            goto rollback;

        return 0;

    rollback: /* recover the original block */
        block->addr = tmp.addr;
        block->len = tmp.len;
        block->type = tmp.type;
        if (added)
            num_entries--;

        return err;
    } else {
        struct mem_map_entry newentry = {
            .addr = addr, .len = len, .type = type};

        for (size_t i = 0; i < num_entries; i++)
            if (overlaps(&entries[i], &newentry))
                return -EEXIST;

        if (num_entries >= MAX_ENTRIES)
            return -ENOMEM;
        entries[num_entries++] = newentry;
    }
    return 0;
}