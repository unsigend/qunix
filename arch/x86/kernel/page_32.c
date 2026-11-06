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

/*
 * i386 two-level paging.
 *
 * kernel_pgtable is the arch-private kernel directory. After page_init its
 * kernel-half PDEs never change and are copied into every process table.
 * This file allocates only directories and page tables. Data frames passed
 * to page_map stay owned by the caller. pagetable_copy allocates dest
 * frames and the caller frees them.
 */

static pagetable_t kernel_pgtable;

static inline unsigned int mapflags(unsigned int flags)
{
    unsigned int flag = PG_P | PG_USER;

    if (flags & PAGEF_WRITE)
        flag |= PG_RW;

    return flag;
}

/*
 * copy_kernel_pgtable - copy kernel-half PDEs from kernel_pgtable into @root.
 *
 * User-half entries are not touched. Kernel PTs are shared, not duplicated.
 */
static inline void copy_kernel_pgtable(pagetable_t root)
{
    size_t npages;
    virt_addr_t va;
    pde_t *pde;

    npages = (size_t)(KERNEL_VIRT_END - KERNEL_VIRT_START) >> PAGE_SHIFT;
    for (size_t i = 0; i < npages; i += PT_ENTRIES) {
        va = KERNEL_VIRT_START + i * PAGE_SIZE;
        pde = (pde_t *)root + pde_index(va);
        *pde = *((pde_t *)kernel_pgtable + pde_index(va));
    }
}

/*
 * page_walk - look up the PTE for @va, optionally allocating a page table.
 *
 * If the PDE is missing and @alloc is set, a zeroed PT is installed with
 * @pdflags. An existing PDE is left unchanged.
 *
 * Returns the PTE slot, or NULL if the PT is absent (@alloc false) or
 * cannot be allocated (@alloc true).
 */
static pte_t *page_walk(pagetable_t root, virt_addr_t va, unsigned int pdflags,
                        int alloc)
{
    pde_t *pde;
    pte_t *pte;

    pde = (pde_t *)root;
    pde = &pde[pde_index(va)];

    if (!pg_test_flag(*pde, PG_P)) {
        if (!alloc)
            return NULL;

        struct pmm_page *page;

        page = pmm_alloc_page();
        if (!page)
            return NULL;

        page_clear(pmm_page_to_va(page));
        *pde = pde_make(pmm_page_to_pa(page), pdflags);
        pg_set_flag(*pde, PG_P);
    }

    pte = (pte_t *)pa_to_va(pde_pa(*pde)) + pte_index(va);
    return pte;
}

void pagetable_switch(pagetable_t root) { pageroot_write(va_to_pa(root)); }
void pagetable_switch_kernel(void) { pageroot_write(va_to_pa(kernel_pgtable)); }

/*
 * pagetable_create - allocate a process page directory.
 *
 * The directory is zeroed and the kernel-half PDEs are copied from
 * kernel_pgtable. User half is empty.
 *
 * Returns the new table, or NULL on OOM.
 */
pagetable_t pagetable_create(void)
{
    struct pmm_page *page;
    virt_addr_t root;

    page = pmm_alloc_page();
    if (!page)
        return (pagetable_t)NULL;

    root = pmm_page_to_va(page);
    page_clear(root);
    copy_kernel_pgtable(root);
    return root;
}

/*
 * page_map - map caller-owned frame @pa at user @va in @root.
 *
 * @pa and @va must be page-aligned. Does not take ownership of @pa.
 * Flushes @va if @root is the loaded table. A new user PDE is
 * PG_P | PG_RW | PG_USER; PTE flags come from @flags.
 *
 * Returns 0, -EINVAL, -EFAULT, -ENOMEM, or -EEXIST.
 */
int page_map(pagetable_t root, phys_addr_t pa, virt_addr_t va,
             unsigned int flags)
{
    pte_t *pte;
    unsigned int archflag;

    if (!IS_ALIGNED(pa, PAGE_SIZE) || !IS_ALIGNED(va, PAGE_SIZE) || !root)
        return -EINVAL;
    if (!in_user_space(va))
        return -EFAULT;

    archflag = mapflags(flags);
    pte = page_walk(root, va, PG_P | PG_RW | PG_USER, 1);
    if (!pte)
        return -ENOMEM;
    if (pg_test_flag(*pte, PG_P))
        return -EEXIST;
    *pte = pte_make(pa, archflag);

    if (pageroot_read() == va_to_pa(root))
        page_tlb_flush(va);
    return 0;
}

/*
 * page_unmap - clear the user mapping at @va. The frame is not freed.
 *
 * Flushes @va if @root is loaded.
 *
 * Returns 0, -EINVAL, -EFAULT, or -ENOENT if @va is not mapped.
 */
int page_unmap(pagetable_t root, virt_addr_t va)
{
    pte_t *pte;

    if (!IS_ALIGNED(va, PAGE_SIZE) || !root)
        return -EINVAL;
    if (!in_user_space(va))
        return -EFAULT;
    pte = page_walk(root, va, 0, 0);
    if (!pte || !pg_test_flag(*pte, PG_P))
        return -ENOENT;
    *pte = pte_make(0, 0);
    if (pageroot_read() == va_to_pa(root))
        page_tlb_flush(va);
    return 0;
}

/*
 * page_lookup - write the physical address mapped at user @va into @pa.
 *
 * The tables are not modified.
 *
 * Returns 0, -EINVAL, -EFAULT, or -ENOENT if @va is not mapped.
 */
int page_lookup(pagetable_t root, virt_addr_t va, phys_addr_t *pa)
{
    pte_t *pte;

    if (!IS_ALIGNED(va, PAGE_SIZE) || !root || !pa)
        return -EINVAL;
    if (!in_user_space(va))
        return -EFAULT;

    pte = page_walk(root, va, 0, 0);
    if (!pte || !pg_test_flag(*pte, PG_P))
        return -ENOENT;
    *pa = pte_pa(*pte);
    return 0;
}

/*
 * pagetable_copy - deep-copy src's user half into dest.
 *
 * dest must be a fresh table from pagetable_create. Each present user
 * page gets a new frame with the same contents and PTE flags. On
 * -ENOMEM dest's copied frames are freed and its PTEs cleared. dest
 * remains freeable (caller pagetable_free). Success: dest's data
 * frames are owned by the caller.
 *
 * Returns 0 or -ENOMEM.
 */
int pagetable_copy(pagetable_t dest, pagetable_t src)
{
    struct pmm_page *page;
    size_t npages, i = 0;
    virt_addr_t va, vpage;
    pde_t *pde;
    pte_t *destpte, *srcpte;

    if (!dest || !src)
        return -EINVAL;

    npages = (size_t)(USER_VIRT_END - USER_VIRT_START) >> PAGE_SHIFT;
    while (i < npages) {
        va = USER_VIRT_START + i * PAGE_SIZE;
        pde = (pde_t *)src + pde_index(va);
        if (!pg_test_flag(*pde, PG_P)) {
            i += PT_ENTRIES;
            continue;
        }

        destpte = page_walk(dest, va, PG_P | PG_RW | PG_USER, 1);
        if (!destpte)
            goto fail;

        srcpte = page_walk(src, va, 0, 0);
        if (!srcpte || !pg_test_flag(*srcpte, PG_P)) {
            i++;
            continue;
        }

        page = pmm_alloc_page();
        if (!page)
            goto fail;
        vpage = pmm_page_to_va(page);
        page_copy(vpage, pa_to_va(pte_pa(*srcpte)));
        *destpte = pte_make(pmm_page_to_pa(page), pg_flags(*srcpte));
        i++;
    }

    return 0;

fail:
    i = 0;
    while (i < npages) {
        va = USER_VIRT_START + i * PAGE_SIZE;
        pde = (pde_t *)dest + pde_index(va);
        if (!pg_test_flag(*pde, PG_P)) {
            i += PT_ENTRIES;
            continue;
        }

        destpte = page_walk(dest, va, 0, 0);
        if (!destpte || !pg_test_flag(*destpte, PG_P)) {
            i++;
            continue;
        }

        pmm_free_page(pmm_pa_to_page(pte_pa(*destpte)));
        *destpte = pte_make(0, 0);
        i++;
    }

    return -ENOMEM;
}

/*
 * pagetable_free - free @root's user page tables and the directory.
 *
 * Does not free mapped data frames (caller-owned) and does not touch the
 * kernel half. @root must not be kernel_pgtable or the loaded table.
 */
void pagetable_free(pagetable_t root)
{
    size_t npages;
    size_t i = 0;
    virt_addr_t va;
    pde_t *pde;

    if (!root)
        return;

    assert(root != kernel_pgtable && root != pa_to_va(pageroot_read()));

    npages = (size_t)(USER_VIRT_END - USER_VIRT_START) >> PAGE_SHIFT;

    while (i < npages) {
        va = USER_VIRT_START + i * PAGE_SIZE;
        pde = (pde_t *)root + pde_index(va);

        if (pg_test_flag(*pde, PG_P)) {
            pmm_free_page(pmm_pa_to_page(pde_pa(*pde)));
            *pde = pde_make(0, 0);
        }

        i += PT_ENTRIES;
    }

    pmm_free_page(pmm_va_to_page(root));
}

/*
 * page_init - build kernel_pgtable and load it.
 *
 * Direct-maps [0, min(RAM, KERNEL_PHYS_END)) at KERNEL_VIRT_START, then
 * preallocates the remaining kernel-half page tables so those PDEs stay
 * fixed. Panics on OOM.
 */
void page_init(void)
{
    struct pmm_page *page;
    virt_addr_t root;
    size_t totalpages, mappages;
    pte_t *pte;
    virt_addr_t va;

    page = pmm_alloc_page();
    if (!page)
        panic("failed to allocate kernel page directory");
    root = pmm_page_to_va(page);
    page_clear(root);
    kernel_pgtable = root;

    totalpages = KERNEL_PHYS_END >> PAGE_SHIFT;
    mappages = PAGE_ALIGN_UP(MIN(memmap_max_ram_addr(), KERNEL_PHYS_END)) >>
               PAGE_SHIFT;

    for (size_t i = 0; i < mappages; i++) {
        va = KERNEL_VIRT_START + i * PAGE_SIZE;
        pte = page_walk(kernel_pgtable, va, PG_P | PG_RW, 1);
        if (!pte)
            panic("failed to allocate kernel page table");
        *pte = pte_make(va_to_pa(va), PG_P | PG_RW);
    }

    for (size_t i = ROUND_UP(mappages, PT_ENTRIES); i < totalpages;
         i += PT_ENTRIES) {
        va = KERNEL_VIRT_START + i * PAGE_SIZE;
        if (!page_walk(kernel_pgtable, va, PG_P | PG_RW, 1))
            panic("failed to preallocate kernel page table");
    }

    pageroot_write(va_to_pa(kernel_pgtable));
    LOGM_INFO("PAGE", "Page management subsystem initialized successfully");
}