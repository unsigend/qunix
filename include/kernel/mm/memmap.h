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

extern int memmap_add_region(phys_addr_t start, phys_addr_t end,
                             enum mem_type type);
extern int memmap_punch_region(phys_addr_t start, phys_addr_t end,
                               enum mem_type type);
extern int memmap_remove_region(phys_addr_t start, phys_addr_t end,
                                enum mem_type type);
extern int memmap_find_region(phys_addr_t start, phys_addr_t end,
                              enum mem_type type, struct mem_region *region);

extern size_t memmap_count(void);
extern int memmap_get_region(size_t index, struct mem_region *region);
extern const char *memmap_type_to_str(enum mem_type type);

extern phys_addr_t memmap_max_ram_addr(void);

#endif /* _QUNIX_MM_MEMMAP_H_ */