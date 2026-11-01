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

#include <asm/memlayout.h>
#include <asm/page.h>

#include <kernel/assert.h>
#include <kernel/errno.h>
#include <kernel/macros.h>
#include <kernel/mm/memlayout.h>
#include <kernel/mm/memmap.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>
#include <kernel/printk.h>

/* Map generic kernel flags to x86 page flags */
static inline unsigned int mapflags(unsigned int flags)
{
    unsigned int pageflags = PG_P;
    if (flags & PAGEF_WRITE)
        pageflags |= PG_WR;
    if (flags & PAGEF_USER)
        pageflags |= PG_USER;
    return pageflags;
}

/* Page walk for 32-bit paging, walk the page table and allocated page table if
 * alloc is set. Return the PTE pointer if found, otherwise return NULL. */
static pte_t *page_walk(pagetable_t root, virt_addr_t va, unsigned int flags,
                        bool alloc)
{
    pde_t *pde;
    pte_t *pte;
    unsigned int pageflags;

    pageflags = mapflags(flags);

    pde = (pde_t *)root + pde_index(va);

    if (!(*pde & PG_P)) {
        if (alloc) {
            struct pmm_page *page = pmm_alloc_page();
            if (!page)
                return NULL;
            page_clear(pmm_page_to_va(page));
            pde_make(pde, pa_to_pn(pmm_page_to_pa(page)), pageflags);
            return (pte_t *)pmm_page_to_va(page) + pte_index(va);
        }
        return NULL;
    } else if (alloc)
        *pde |= pageflags;

    pte = (pte_t *)pde_get_pt_va(pde) + pte_index(va);
    return pte;
}

static inline void copy_kernel_pgtbl(pagetable_t root)
{
    memcpy((pde_t *)root + pde_index(KERNEL_VIRT_START),
           (pde_t *)kernel_pagetable + pde_index(KERNEL_VIRT_START),
           (1024 - pde_index(KERNEL_VIRT_START)) * sizeof(pde_t));
}

int page_map(pagetable_t root, phys_addr_t pa, virt_addr_t va,
             unsigned int flags)
{
    pte_t *pte;
    unsigned int pageflags;

    if (!root || !IS_ALIGNED(va, PAGE_SIZE) || !IS_ALIGNED(pa, PAGE_SIZE))
        return -EINVAL;

    pageflags = PG_P;
    if (flags & PAGEF_WRITE)
        pageflags |= PG_WR;
    if (flags & PAGEF_USER)
        pageflags |= PG_USER;

    pte = page_walk(root, va, flags, true);
    if (!pte)
        return -ENOMEM;
    pte_make(pte, pa_to_pn(pa), pageflags);

    if (pa_to_va(pageroot_read()) == root || in_kernel_space(va))
        page_tlb_flush(va);

    return 0;
}

void page_unmap(pagetable_t root, virt_addr_t va)
{
    pte_t *pte;

    if (!root || !IS_ALIGNED(va, PAGE_SIZE))
        return;

    pte = page_walk(root, va, 0, false);
    if (!pte)
        return;
    *pte = 0;

    if (pa_to_va(pageroot_read()) == root || in_kernel_space(va))
        page_tlb_flush(va);
}

int page_lookup(pagetable_t root, virt_addr_t va, phys_addr_t *pa)
{
    pte_t *pte;

    if (!root || !IS_ALIGNED(va, PAGE_SIZE))
        return -EINVAL;

    pte = page_walk(root, va, 0, false);
    if (!pte || !(*pte & PG_P))
        return -ENOENT;

    *pa = pn_to_pa(pte_get_pn(pte));

    return 0;
}

pagetable_t pagetable_create(void)
{
    struct pmm_page *page;
    virt_addr_t va;

    page = pmm_alloc_page();
    if (!page)
        return 0;
    va = pmm_page_to_va(page);
    page_clear(va);
    copy_kernel_pgtbl(va);
    return va;
}

void pagetable_free(pagetable_t root)
{
    pde_t *pde;
    pte_t *pte;
    size_t i, j;

    if (!root)
        return;
    assert(root != kernel_pagetable);

    for (i = 0; i < pde_index(USER_VIRT_END); i++) {
        pde = (pde_t *)root + i;
        if (!(*pde & PG_P))
            continue;

        pte = (pte_t *)pde_get_pt_va(pde);
        for (j = 0; j < 1024; j++) {
            if (pte[j] & PG_P)
                pmm_free_page(pmm_pa_to_page(pn_to_pa(pte_get_pn(&pte[j]))));
        }
        pmm_free_page(pmm_va_to_page((virt_addr_t)pte));
    }

    pmm_free_page(pmm_va_to_page(root));
}

int pagetable_copy(pagetable_t dest, pagetable_t src);
void pagetable_switch(pagetable_t root);

pagetable_t kernel_pagetable;

void page_init(void)
{
    phys_addr_t mapend_pa;
    virt_addr_t pagetable_va;
    struct pmm_page *page;
    int err;
    size_t npages;

    mapend_pa = MIN(
        memmap_max_ram_addr(),
        KERNEL_PHYS_END); /* map either max RAM or 1GB physical address space */
    npages = (mapend_pa - KERNEL_PHYS_START) >> PAGE_SHIFT;
    page = pmm_alloc_page();

    if (!page)
        panic("failed to allocate page root for page table");

    pagetable_va = pmm_page_to_va(page);
    page_clear(pagetable_va);

    kernel_pagetable = pagetable_va;

    if ((err =
             page_map_range(pagetable_va, KERNEL_PHYS_START, KERNEL_VIRT_START,
                            npages, PAGEF_WRITE | PAGEF_EXEC)))
        panic_error(err, "failed to map kernel page table");

    for (uint64_t va = KERNEL_VIRT_START + PAGE_SIZE * npages;
         va < KERNEL_VIRT_END; va += PAGE_SIZE) {
        if (!page_walk(pagetable_va, va, PAGEF_WRITE | PAGEF_EXEC, true))
            panic("failed to pre-allocate page table for kernel");
    }

    pageroot_write(va_to_pa(pagetable_va));

    LOGM_INFO("PAGE", "Page management subsystem initialized successfully");
}