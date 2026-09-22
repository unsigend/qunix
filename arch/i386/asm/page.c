/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/page.h>
#include <qunix/mm/page.h>

static inline void enable_paging(void)
{
    uint32_t cr0;
    asm volatile("movl %%cr0, %0\n\t"
                 "orl $0x80000001, %0\n\t"
                 "movl %0, %%cr0"
                 : "=r"(cr0)
                 :
                 : "memory");
}

void page_set_pagedir(uintptr_t pagedir)
{
    asm volatile("movl %0, %%cr3" : : "r"(pagedir) : "memory");
}

uintptr_t page_get_pagedir(void)
{
    uintptr_t pagedir;
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

void page_init(void)
{
    /* TODO: initialize page tables and page directory */

    /* TODO: load page directory into CR3 register */

    enable_paging();
}