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

/*
 * memmap_add_region - add [start, end) of the given type to the map.
 *
 * Overlapping or adjacent regions of the same type are merged. Overlap with a
 * region of a different type is a bug and panics.
 *
 * Returns 0 on success, -EINVAL if end <= start, -ENOMEM if the map is full.
 */
extern int memmap_add_region(phys_addr_t start, phys_addr_t end,
                             enum mem_type type);

/*
 * memmap_punch_region - change the type of [start, end) to @type.
 *
 * The range must lie within a single existing region, which is split and
 * merged with its neighbour regions as needed.
 *
 * Returns 0 on success, -EINVAL if end <= start, -ENOENT if no region
 * contains the range, -ENOMEM if a split does not fit.
 */
extern int memmap_punch_region(phys_addr_t start, phys_addr_t end,
                               enum mem_type type);

/*
 * memmap_remove_region - remove an exactly matching region from the map.
 *
 * Returns 0 on success, -EINVAL if end <= start, -ENOENT if no region
 * matches start, end and type exactly.
 */
extern int memmap_remove_region(phys_addr_t start, phys_addr_t end,
                                enum mem_type type);

extern const struct mem_region *
memmap_find_region(phys_addr_t start, phys_addr_t end, enum mem_type type);

extern size_t memmap_count(void);
extern const struct mem_region *memmap_get_region(size_t index);
extern const char *memmap_type_to_str(enum mem_type type);

/*
 * memmap_max_ram_addr - end of the highest addressable RAM region.
 *
 * RAM is MEM_AVAILABLE, MEM_KERNEL, and MEM_ACPI.
 *
 * Returns that exclusive end, or 0 if there is no RAM.
 */
extern phys_addr_t memmap_max_ram_addr(void);

#endif /* _QUNIX_MM_MEMMAP_H_ */