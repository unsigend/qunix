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

#include <kernel/config.h>
#include <kernel/mm/memlayout.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>

#if CONFIG_PMM_SIMPLE
#include "simple/simple.h"
#elif CONFIG_PMM_BITMAP
#include "bitmap/bitmap.h"
#elif CONFIG_PMM_BUDDY
#include "buddy/buddy.h"
#else
#error "No PMM backend selected"
#endif

static const struct pmm_ops *backend;

void pmm_register(const struct pmm_ops *ops)
{
    if (backend)
        panic("PMM backend already registered");
    if (!ops)
        panic("Invalid PMM backend arguments");
    if (!ops->alloc || !ops->free || !ops->stats || !ops->page_to_pa ||
        !ops->pa_to_page)
        panic("PMM backend missing functions");
    backend = ops;
}

void pmm_init(void)
{
#if CONFIG_PMM_SIMPLE
    pmm_simple_init();
#elif CONFIG_PMM_BITMAP
    pmm_bitmap_init();
#elif CONFIG_PMM_BUDDY
    pmm_buddy_init();
#else
    panic("No PMM backend selected");
#endif
    if (!backend)
        panic("PMM backend not registered");
}

void pmm_stats(struct pmm_stats *stats)
{
    if (!backend)
        panic("PMM backend not registered");
    backend->stats(stats);
}

struct pmm_page *pmm_alloc_page(void)
{
    if (!backend)
        panic("PMM backend not registered");
    return backend->alloc(0);
}

struct pmm_page *pmm_alloc_pages(unsigned int order)
{
    if (!backend)
        panic("PMM backend not registered");
    return backend->alloc(order);
}

void pmm_free_page(struct pmm_page *page)
{
    if (!backend)
        panic("PMM backend not registered");
    backend->free(page, 0);
}

void pmm_free_pages(struct pmm_page *page, unsigned int order)
{
    if (!backend)
        panic("PMM backend not registered");
    backend->free(page, order);
}

phys_addr_t pmm_page_to_pa(struct pmm_page *page)
{
    if (!backend)
        panic("PMM backend not registered");
    return backend->page_to_pa(page);
}

virt_addr_t pmm_page_to_va(struct pmm_page *page)
{
    phys_addr_t pa;

    if (!backend)
        panic("PMM backend not registered");

    pa = backend->page_to_pa(page);
    return pa_to_va(pa);
}

struct pmm_page *pmm_pa_to_page(phys_addr_t pa)
{
    if (!backend)
        panic("PMM backend not registered");
    return backend->pa_to_page(pa);
}

struct pmm_page *pmm_va_to_page(virt_addr_t va)
{
    phys_addr_t pa;

    if (!backend)
        panic("PMM backend not registered");

    pa = va_to_pa(va);
    return backend->pa_to_page(pa);
}