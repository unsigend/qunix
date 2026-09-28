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

#ifndef _QUNIX_MM_PMM_H_
#define _QUNIX_MM_PMM_H_

#include <kernel/compiler.h>
#include <kernel/mm/memlayout.h>
#include <kernel/types.h>

struct pmm_page; /* A opaque struct representing a phyiscal page */

extern void pmm_init(void);

/* Allocate or free a page based on the order of the page, namely 2^order pages.
 * The page will be contiguous in physical memory. */
extern struct pmm_page *pmm_alloc_pages(uint32_t order);
extern void pmm_free_pages(struct pmm_page *page, uint32_t order);

extern phys_addr_t pmm_page_to_phys(struct pmm_page *page);
extern struct pmm_page *pmm_phys_to_page(phys_addr_t phys);

static __always_inline virt_addr_t pmm_page_to_virt(struct pmm_page *page)
{
    return phys_to_virt(pmm_page_to_phys(page));
}

extern struct pmm_page *pmm_alloc_page(void);
extern void pmm_free_page(struct pmm_page *page);

#endif /* _QUNIX_MM_PMM_H_ */