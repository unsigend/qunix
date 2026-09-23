/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */
#include <asm/page.h>
#include <qunix/error.h>
#include <qunix/mm/memlayout.h>
#include <qunix/mm/page.h>
#include <qunix/mm/pmm.h>
#include <qunix/mm/vm.h>
#include <stdint.h>
#include <string.h>

int vm_map_page(pagetable_t pagetable, virt_addr_t va, phys_addr_t pa,
                uint32_t perm)
{
    uint32_t flags = PG_P_MASK;
    size_t pde_idx, pte_idx;
    pde_t *pde;
    pte_t *pte;
    phys_addr_t pte_base_phy;
    virt_addr_t pte_base_virt;

    if (!pagetable)
        return -EINVAL;

    if (!IS_ALIGNED(va, PAGE_SIZE) || !IS_ALIGNED(pa, PAGE_SIZE))
        return -EINVAL;

    if (perm & VM_PERM_WRITE)
        flags |= PG_RW_MASK;
    if (perm & VM_PERM_USER)
        flags |= PG_USER_MASK;

    pde_idx = VIRTADDR_PGDIR_IDX(va);
    pte_idx = VIRTADDR_PGTABLE_IDX(va);
    pde = &((pde_t *)pagetable)[pde_idx];

    if (!(*pde & PG_P_MASK)) {
        struct pmm_page *phy_page = pmm_alloc_page();
        if (!phy_page)
            return -ENOMEM;
        pte_base_phy = pmm_page_to_phys(phy_page);
        pte_base_virt = pmm_page_to_virt(phy_page);

        memset((void *)pte_base_virt, 0, PAGE_SIZE);
        *pde = pde_make(pte_base_phy, flags);
        pte = &((pte_t *)pte_base_virt)[pte_idx];
        *pte = pte_make(pa, flags);
    } else {
        pte_base_phy = *pde & PG_ADDR_MASK;
        pte_base_virt = phys_to_virt(pte_base_phy);

        pte = &((pte_t *)pte_base_virt)[pte_idx];
        if (*pte & PG_P_MASK) /* remapping */
            return -EEXIST;
        *pte = pte_make(pa, flags);
    }

    return 0;
}

int vm_unmap_page(pagetable_t pagetable, virt_addr_t va)
{
    size_t pde_idx, pte_idx;
    pde_t *pde;
    pte_t *pte;
    phys_addr_t pte_base_phy;
    virt_addr_t pte_base_virt;

    if (!pagetable)
        return -EINVAL;
    if (!IS_ALIGNED(va, PAGE_SIZE))
        return -EINVAL;

    pde_idx = VIRTADDR_PGDIR_IDX(va);
    pte_idx = VIRTADDR_PGTABLE_IDX(va);

    pde = &((pde_t *)pagetable)[pde_idx];
    pte_base_phy = *pde & PG_ADDR_MASK;
    pte_base_virt = phys_to_virt(pte_base_phy);

    if (!(*pde & PG_P_MASK))
        return -ENOENT;

    pte = &((pte_t *)pte_base_virt)[pte_idx];
    if (!(*pte & PG_P_MASK))
        return -ENOENT;

    *pte = 0;
    page_flush_tlb_one(va);
    return 0;
}

pagetable_t vm_alloc_pagetable(void)
{
    pagetable_t pagetable;
    struct pmm_page *phy_page = pmm_alloc_page();
    if (!phy_page)
        return (pagetable_t)NULL;

    pagetable = (pagetable_t)pmm_page_to_virt(phy_page);
    memset((void *)pagetable, 0, PAGE_SIZE);
    return pagetable;
}