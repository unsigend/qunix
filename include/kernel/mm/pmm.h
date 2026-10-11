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
#include <kernel/config.h>
#include <kernel/types.h>

struct pmm_page; /* Opaque page frame managed by the PMM */

struct pmm_stats {
    size_t free_pages;
    size_t alloc_pages;
    size_t total_pages;
    unsigned int max_order; /* max order support */
};

struct pmm_ops {
    /* Allocate 2^order page frames. Return NULL if allocation fails. */
    struct pmm_page *(*alloc)(unsigned int order);
    void (*free)(struct pmm_page *page, unsigned int order);
    void (*stats)(struct pmm_stats *stats);
};

extern void pmm_init(void);

extern struct pmm_page *pmm_alloc_page(void);
extern struct pmm_page *pmm_alloc_pages(unsigned int order);

extern void pmm_free_page(struct pmm_page *page);
extern void pmm_free_pages(struct pmm_page *page, unsigned int order);

extern void pmm_stats(struct pmm_stats *stats);

extern phys_addr_t pmm_page_to_pa(struct pmm_page *page);
extern virt_addr_t pmm_page_to_va(struct pmm_page *page);
extern struct pmm_page *pmm_pa_to_page(phys_addr_t pa);
extern struct pmm_page *pmm_va_to_page(virt_addr_t va);

/* Register the PMM backend. A second installation will panic. Only the PMM
 * backend provider should call this function. */
extern void pmm_register(const struct pmm_ops *ops);

#endif /* _QUNIX_MM_PMM_H_ */