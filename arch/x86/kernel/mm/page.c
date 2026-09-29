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

#include <asm/cpu.h>
#include <asm/page.h>

#include <kernel/compiler.h>
#include <kernel/fmt.h>
#include <kernel/macros.h>
#include <kernel/panic.h>
#include <kernel/printk.h>

#include <kernel/mm/memlayout.h>
#include <kernel/mm/memmap.h>
#include <kernel/mm/page.h>
#include <kernel/mm/vm.h>

static pde_t _kernel_pagedir[PAGE_SIZE / sizeof(pde_t)] __aligned(PAGE_SIZE);
pagetable_t kernel_pagetable = (pagetable_t)_kernel_pagedir;

void page_flush_tlb_all(void) { cr3_write(cr3_read()); }

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
