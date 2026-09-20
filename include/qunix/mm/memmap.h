/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_MMAP_H
#define QUNIX_MM_MMAP_H

#include <qunix/macros.h>
#include <stddef.h>
#include <stdint.h>

enum mem_map_type {
    MEMMAP_AVAILABLE = 1, /* Available memory for use */
    MEMMAP_RESERVED = 2,  /* Hardware reserved memory */
    MEMMAP_KERNEL = 3,    /* Kernel memory */
};

struct mem_map_entry {
    uint64_t addr;
    uint64_t len;
    enum mem_map_type type;
};

extern const struct mem_map_entry *mem_map_get(void);
extern size_t mem_map_get_size(void);

/* Return amount of the memory in bytes. */
extern uint64_t mem_map_get_available(void);
extern uint64_t mem_map_get_reserved(void);
extern uint64_t mem_map_get_kernel(void);

/* Append a new memory map entry to the memory map, return -errno on failure. If
 * the type is MEMMAP_KERNEL, the range must be within the MEMMAP_AVAILABLE
 * range, rejected if the range is MEMMAP_RESERVED. Duplicate region add will be
 * rejected. */
extern int mem_map_add(uint64_t addr, uint64_t len, enum mem_map_type type);

/* Find a memory map entry by address, return NULL if not found. */
extern const struct mem_map_entry *mem_map_find(uint64_t addr);

/* Find the memory map entry containing the given range [addr, addr + len),
 * return NULL if not found. */
extern const struct mem_map_entry *mem_map_find_range(uint64_t addr,
                                                      uint64_t len);

/* Find the memory map entry with the exact address, length and type, return
 * NULL if not found. */
extern const struct mem_map_entry *
mem_map_find_type(uint64_t addr, uint64_t len, enum mem_map_type type);

/* Hardware specific memory map initialization. */
CONTRACT extern void memmap_init(void);

#endif