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

#include "simple.h"

#include <kernel/mm/memlayout.h>
#include <kernel/mm/memmap.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/spinlock.h>
#include <kernel/types.h>

#define MAX_ORDER 0

/*
 * Single-page free list.
 *
 * pages[] has one descriptor per frame in [0, max_pa). A free frame is
 * linked through next, an allocated one is not. The list is LIFO, and
 * only order 0 is served. free_pages + alloc_pages == total_pages: the
 * frames that were available at init. The descriptor array itself stays
 * off the list.
 *
 * alloc, free, and stats take an irqsave spinlock: an interrupt handler
 * may allocate. The array base and npages are fixed after init, so
 * page_to_pa and pa_to_page do not take the lock. Descriptors remain
 * valid for the life of the kernel.
 */

struct pmm_page {
    struct pmm_page *next;
};

struct page_array {
    struct pmm_page *pages;
    size_t npages;
};

struct pmm_simple_pool {
    struct pmm_page *free_list;
    struct page_array array;
    size_t free_pages;
    size_t alloc_pages;
    size_t total_pages;
    struct spinlock lock;
};

static struct pmm_simple_pool pool;

static struct pmm_page *alloc(unsigned int order)
{
    struct pmm_page *page;
    unsigned long flags;

    flags = spinlock_lock_irqsave(&pool.lock);

    if (order > MAX_ORDER || !pool.free_list) {
        spinlock_unlock_irqrestore(&pool.lock, flags);
        return NULL;
    }

    page = pool.free_list;
    pool.free_list = page->next;
    page->next = NULL;

    pool.free_pages--;
    pool.alloc_pages++;

    spinlock_unlock_irqrestore(&pool.lock, flags);
    return page;
}

static void free(struct pmm_page *page, unsigned int order)
{
    pn_t fpn;
    unsigned long flags;

    if (order > MAX_ORDER || !page)
        panic("pmm_simple: invalid arguments");

    if (page < pool.array.pages)
        panic("pmm_simple: invalid page number");
    fpn = (pn_t)(page - pool.array.pages);
    if (fpn >= pool.array.npages)
        panic("pmm_simple: invalid page number");

    flags = spinlock_lock_irqsave(&pool.lock);
    page->next = pool.free_list;
    pool.free_list = page;

    pool.free_pages++;
    pool.alloc_pages--;

    spinlock_unlock_irqrestore(&pool.lock, flags);
}

static void stats(struct pmm_stats *stats)
{
    struct pmm_stats s;
    unsigned long flags;

    s.max_order = MAX_ORDER;

    flags = spinlock_lock_irqsave(&pool.lock);
    s.free_pages = pool.free_pages;
    s.alloc_pages = pool.alloc_pages;
    s.total_pages = pool.total_pages;
    spinlock_unlock_irqrestore(&pool.lock, flags);

    *stats = s;
}

static phys_addr_t page_to_pa(struct pmm_page *page)
{
    return (phys_addr_t)(page - pool.array.pages) << PAGE_SHIFT;
}

static struct pmm_page *pa_to_page(phys_addr_t pa)
{
    pn_t fpn;

    fpn = pa >> PAGE_SHIFT;
    if (fpn >= pool.array.npages)
        return NULL;

    return pool.array.pages + fpn;
}

static struct pmm_ops ops = {
    .alloc = alloc,
    .free = free,
    .stats = stats,
    .page_to_pa = page_to_pa,
    .pa_to_page = pa_to_page,
};

static void init_free_list(void)
{
    struct mem_region r;
    struct pmm_page *page;
    phys_addr_t aligned;

    for (size_t i = 0; i < memmap_count(); i++) {
        memmap_get_region(i, &r);
        if (r.type != MEM_AVAILABLE)
            continue;

        aligned = PAGE_ALIGN_UP(r.start);
        if (aligned >= r.end) /* smaller then one page */
            continue;

        phys_addr_t addr = aligned;
        while (r.end - addr >= PAGE_SIZE) {
            page = pool.array.pages + (addr >> PAGE_SHIFT);
            page->next = pool.free_list;
            pool.free_list = page;
            pool.total_pages++;
            addr += PAGE_SIZE;
        }
    }

    pool.free_pages = pool.total_pages;
}

void pmm_simple_init(void)
{
    phys_addr_t max_pa;
    size_t array_bytes;
    struct mem_region r;
    int err;

    max_pa = memmap_max_ram_addr();
    if (!max_pa)
        panic("No physical memory regions available");

    pool.array.npages = PAGE_ALIGN_UP(max_pa) >> PAGE_SHIFT;
    array_bytes = pool.array.npages * sizeof(struct pmm_page);
    array_bytes = PAGE_ALIGN_UP(array_bytes);

    for (size_t i = 0; i < memmap_count(); i++) {
        memmap_get_region(i, &r);
        if (r.type != MEM_AVAILABLE)
            continue;

        phys_addr_t aligned = PAGE_ALIGN_UP(r.start);
        if (aligned < r.end && r.end - aligned >= array_bytes) {
            pool.array.pages = (struct pmm_page *)pa_to_va(aligned);
            if ((err = memmap_punch_region(aligned, aligned + array_bytes,
                                           MEM_KERNEL)))
                panic_error(err, "pmm_simple: failed to punch memory region");
            break;
        }
    }

    if (!pool.array.pages)
        panic("pmm_simple: not enough available memory");

    init_free_list();
    spinlock_init(&pool.lock, "pmm_simple");

    pmm_register(&ops);

    LOGM_INFO("PMM", "PMM initialized successfully. Provider: simple");
}
