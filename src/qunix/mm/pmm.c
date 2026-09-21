/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/kernel.h>
#include <qunix/log.h>
#include <qunix/mm/memlayout.h>
#include <qunix/mm/memmap.h>
#include <qunix/mm/page.h>
#include <qunix/mm/pmm.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* A simple physical page frame allocator. The implementation is based on a
 * hybrid approach, a free list and a page array mapping, the page array mapping
 * all the physical addresses. */

struct pmm_page {
    struct pmm_page *next;
};

struct pmm_pool {
    struct pmm_page *free_head;  /* free list */
    struct pmm_page *page_array; /* mapping all physical pages */
    size_t page_array_sz;        /* page array size in bytes */
    size_t page_count;           /* number of total physical pages */
    size_t avail_count;          /* number of available physical pages */
    size_t free_count;           /* number of free physical pages */
    size_t alloc_count;          /* number of allocated physical pages */
};

static struct pmm_pool pool;

void pmm_init(void)
{
    phys_addr_t max_phys = mem_map_get_max_phys();
    pool.page_count = max_phys >> PAGE_SHIFT;
    pool.page_array = (struct pmm_page *)(uintptr_t)KERNEL_PHYS_END;
    pool.page_array_sz =
        ROUND_UP(pool.page_count * sizeof(struct pmm_page), PAGE_SIZE);

    memset((void *)pool.page_array, 0, pool.page_array_sz);

    if (mem_map_add((uint64_t)(uintptr_t)pool.page_array, pool.page_array_sz,
                    MEMMAP_KERNEL) < 0)
        panic("Failed to add pmm page array to memmap");

    for (size_t i = 0; i < mem_map_get_size(); i++) {
        const struct mem_map_entry *e = &mem_map_get()[i];
        if (e->type == MEMMAP_AVAILABLE) {
            for (uint64_t aligned = ROUND_UP(e->addr, PAGE_SIZE);
                 aligned < e->addr + e->len; aligned += PAGE_SIZE) {
                /* skip the page frame at address 0 to distinguish from physical
                 * address 0 and a failure return NULL. */
                if (aligned == 0)
                    continue;
                struct pmm_page *page = pmm_phys_to_page(aligned);
                page->next = pool.free_head;
                pool.free_head = page;
                pool.free_count++;
                pool.avail_count++;
            }
        }
    }

    LOGM(
        LOG_LEVEL_INFO, "PMM",
        "Initialized with %llu physical pages, %llu available, %llu free, %llu "
        "allocated",
        (unsigned long long)pool.page_count,
        (unsigned long long)pool.avail_count,
        (unsigned long long)pool.free_count,
        (unsigned long long)pool.alloc_count);
}

struct pmm_page *pmm_alloc_pages(uint32_t order)
{
    if (order != 0)
        return NULL; /* only support order 0 */

    return pmm_alloc_page();
}

void pmm_free_pages(struct pmm_page *page, uint32_t order)
{
    if (order != 0)
        return; /* only support order 0 */

    pmm_free_page(page);
}

phys_addr_t pmm_page_to_phys(struct pmm_page *page)
{
    size_t index = (size_t)(page - pool.page_array);
    return (phys_addr_t)(index << PAGE_SHIFT);
}

struct pmm_page *pmm_phys_to_page(phys_addr_t phys)
{
    size_t index = (size_t)(phys >> PAGE_SHIFT);
    return &pool.page_array[index];
}

struct pmm_page *pmm_alloc_page(void)
{
    if (pool.free_head == NULL)
        return NULL;

    struct pmm_page *page = pool.free_head;
    pool.free_head = page->next;
    page->next = NULL;
    pool.alloc_count++;
    pool.free_count--;
    return page;
}

void pmm_free_page(struct pmm_page *page)
{
    if (page == NULL)
        return;

    page->next = pool.free_head;
    pool.free_head = page;
    pool.free_count++;
    pool.alloc_count--;
}