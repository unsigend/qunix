/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/page.h>
#include <qunix/fmt.h>
#include <qunix/log.h>
#include <qunix/mm/memlayout.h>
#include <qunix/mm/memmap.h>
#include <qunix/mm/page.h>
#include <qunix/mm/types.h>
#include <qunix/mm/vm.h>
#include <stddef.h>

static pde_t _kernel_pagedir[1024] __attribute__((aligned(PAGE_SIZE)));
pagetable_t kernel_pagetable = (pagetable_t)_kernel_pagedir;

void page_set_pagedir(phys_addr_t pagedir)
{
    asm volatile("movl %0, %%cr3" : : "r"(pagedir) : "memory");
}

phys_addr_t page_get_pagedir(void)
{
    phys_addr_t pagedir;
    asm volatile("movl %%cr3, %0" : "=r"(pagedir));
    return pagedir;
}

uint32_t pde_make_flag(int present, int write, int user)
{
    return (present ? PG_P_MASK : 0) | (write ? PG_RW_MASK : 0) |
           (user ? PG_USER_MASK : 0);
}

uint32_t pte_make_flag(int present, int write, int user, int pat, int global)
{
    return (present ? PG_P_MASK : 0) | (write ? PG_RW_MASK : 0) |
           (user ? PG_USER_MASK : 0) | (pat ? PTE_PAT_MASK : 0) |
           (global ? PTE_G_MASK : 0);
}

pde_t pde_make(phys_addr_t pa, uint32_t flags)
{
    return (pa & PG_ADDR_MASK) | flags;
}

pte_t pte_make(phys_addr_t pa, uint32_t flags)
{
    return (pa & PG_ADDR_MASK) | flags;
}

void page_flush_tlb_all(void)
{
    uintptr_t cr3;
    asm volatile("movl %%cr3, %0\n\t"
                 "movl %0, %%cr3"
                 : "=r"(cr3)
                 :
                 : "memory");
}

void page_flush_tlb_one(virt_addr_t va)
{
    asm volatile("invlpg (%0)" : : "r"(va) : "memory");
}

void page_init(void)
{
    phys_addr_t max_phys = mem_map_get_max_phys();
    if (max_phys >
        KERNEL_VIRT_MAX) /* current mapping strategy can maximum maps 0-1GB
                            physical memory to 3GB-4GB virtual memory. */
    {
        char phy_buf[64];
        char max_buf[64];
        fmt_mem(phy_buf, sizeof(phy_buf), mem_map_get_max_phys());
        fmt_mem(max_buf, sizeof(max_buf), KERNEL_VIRT_MAX);
        panic("the amount of physical memory %s is greater than the current "
              "maximum virtual space for kernel %s",
              phy_buf, max_buf);
    }

    /* maps all the physical memory from 0-PHYS_MAX to 3GB-4GB virtual memory */
    int err = 0;
    size_t n = ROUND_DOWN(max_phys, PAGE_SIZE) / PAGE_SIZE;

    if ((err = vm_map_pages(kernel_pagetable, KERNEL_VIRT_BASE, 0, n,
                            VM_PERM_READ | VM_PERM_WRITE | VM_PERM_EXEC)) < 0)
        panic("failed to map physical memory to kernel virtual memory");

    /* load kernel page directory into CR3 register */
    page_set_pagedir(virt_to_phys(kernel_pagetable));

    LOGM(LOG_LEVEL_INFO, "PAGE", "Paging initialized successfully");
}