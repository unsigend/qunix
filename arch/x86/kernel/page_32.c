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

#include <kernel/errno.h>
#include <kernel/macros.h>
#include <kernel/mm/memlayout.h>
#include <kernel/mm/memmap.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>
#include <kernel/printk.h>

int page_map(pagetable_t root, phys_addr_t pa, virt_addr_t va,
             unsigned int flags)
{
    pde_t *pde;
    pte_t *pte;
    unsigned int pageflags;
    struct pmm_page *page;
    phys_addr_t page_pa;
    virt_addr_t page_va, pt_va;
    pagetable_t cpu_pagetable;

    if (!root || !IS_ALIGNED(va, PAGE_SIZE) || !IS_ALIGNED(pa, PAGE_SIZE))
        return -EINVAL;

    pageflags = PG_P;
    if (flags & PAGEF_WRITE)
        pageflags |= PG_WR;
    if (flags & PAGEF_USER)
        pageflags |= PG_USER;

    pde = (pde_t *)root + pde_index(va);
    if (!(*pde & PG_P)) {
        page = pmm_alloc_page();
        if (!page)
            return -ENOMEM;
        page_pa = pmm_page_to_pa(page);
        page_va = pmm_page_to_va(page);
        page_clear(page_va);
        pde_make(pde, pa_to_pn(page_pa), pageflags);
    } else if ((flags & PAGEF_WRITE) || (flags & PAGEF_USER))
        *pde |= pageflags;

    pt_va = pde_get_pt_va(pde);
    pte = (pte_t *)pt_va + pte_index(va);
    pte_make(pte, pa_to_pn(pa), pageflags);

    cpu_pagetable = pa_to_va(pageroot_read());
    if (cpu_pagetable == root)
        page_flush_tlb(va); /* flush TLB for the page */

    return 0;
}

void page_unmap(pagetable_t root, virt_addr_t va)
{
    pde_t *pde;
    pte_t *pte;
    virt_addr_t pt_va;

    if (!root || !IS_ALIGNED(va, PAGE_SIZE))
        return;

    pde = (pde_t *)root + pde_index(va);

    if (!(*pde & PG_P))
        return;

    pt_va = pde_get_pt_va(pde);
    pte = (pte_t *)pt_va + pte_index(va);

    if (!(*pte & PG_P))
        return;

    *pte = 0;
    if (pa_to_va(pageroot_read()) == root)
        page_flush_tlb(va);
}

int page_lookup(pagetable_t root, virt_addr_t va, phys_addr_t *pa)
{
    pde_t *pde;
    pte_t *pte;
    virt_addr_t pt_va;

    if (!root || !IS_ALIGNED(va, PAGE_SIZE))
        return -EINVAL;

    pde = (pde_t *)root + pde_index(va);
    if (!(*pde & PG_P))
        return -ENOENT;

    pt_va = pde_get_pt_va(pde);
    pte = (pte_t *)pt_va + pte_index(va);
    if (!(*pte & PG_P))
        return -ENOENT;

    *pa = pn_to_pa(pte_get_pn(pte));
    return 0;
}

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

    if ((err =
             page_map_range(pagetable_va, KERNEL_PHYS_START, KERNEL_VIRT_START,
                            npages, PAGEF_WRITE | PAGEF_EXEC)))
        panic_error(err, "failed to map kernel page table");

    pageroot_write(va_to_pa(pagetable_va));

    LOGM_INFO("PAGE", "Page management subsystem initialized successfully");
}