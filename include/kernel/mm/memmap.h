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

#include <kernel/types.h>

enum mem_type {
    MEM_AVAILABLE,
    MEM_RESERVED,
    MEM_KERNEL,
    MEM_ACPI,
};

struct mem_region {
    phys_addr_t start;
    phys_addr_t end;
    enum mem_type type;
};

extern void memmap_init(void);

/* Add a memory region [start, end) to the memory map. Only append to a new
 * region, overlap with existing regions will panic. */
extern int memmap_add_region(phys_addr_t start, phys_addr_t end,
                             enum mem_type type);

/* Update a memory region in memory map, split and merge may happen. */
extern int memmap_update_region(phys_addr_t start, phys_addr_t end,
                                enum mem_type type);

extern size_t memmap_count(void);
extern const struct mem_region *memmap_get_region(size_t index);

#endif /* _QUNIX_MM_MEMMAP_H_ */