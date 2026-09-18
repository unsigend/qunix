/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_MMAP_H
#define QUNIX_MM_MMAP_H

#include <stddef.h>
#include <stdint.h>

enum mem_map_type {
    MEMMAP_AVAILABLE = 1,
    MEMMAP_RESERVED = 2,
};

struct mem_map_entry {
    uint64_t addr;
    uint64_t len;
    enum mem_map_type type;
};

extern const struct mem_map_entry *mem_map_get(void);
extern size_t mem_map_get_size(void);

/* Return total amount of the memory in bytes. */
extern uint64_t mem_map_get_total(void);

/* Append a new memory map entry to the memory map, return -errno on failure. */
extern int mem_map_add(uint64_t addr, uint64_t len, enum mem_map_type type);

/* Find a memory map entry either by address and length or by all fields, return
 * NULL if not found. */
extern const struct mem_map_entry *mem_map_find(uint64_t addr, uint64_t len);
extern const struct mem_map_entry *
mem_map_find_type(uint64_t addr, uint64_t len, enum mem_map_type type);

#endif