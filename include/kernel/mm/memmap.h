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

#ifndef _QUNIX_MM_MEMMAP_H_
#define _QUNIX_MM_MEMMAP_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>
#include <kernel/types.h>

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

/* Return the maximum physical address exclusively. */
extern phys_addr_t mem_map_get_max_phys(void);

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

CONTRACT extern void memmap_init(void);

#endif /* _QUNIX_MM_MEMMAP_H_ */