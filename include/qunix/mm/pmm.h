/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_PMM_H
#define QUNIX_MM_PMM_H

#include <qunix/mm/types.h>

struct pmm_page; /* A opaque struct representing a phyiscal page */

extern void pmm_init(void);

/* Allocate or free a page based on the order of the page, namely 2^order pages.
 * The page will be contiguous in physical memory. */
extern struct pmm_page *pmm_alloc_pages(uint32_t order);
extern void pmm_free_pages(struct pmm_page *page, uint32_t order);

extern phys_addr_t pmm_page_to_phys(struct pmm_page *page);
extern struct pmm_page *pmm_phys_to_page(phys_addr_t phys);

extern struct pmm_page *pmm_alloc_page(void);
extern void pmm_free_page(struct pmm_page *page);

#endif