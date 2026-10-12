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

#ifndef _X86_ASM_PAGE_H_
#define _X86_ASM_PAGE_H_

#include <asm/cpu.h>

#include <kernel/types.h>

static __always_inline phys_addr_t pageroot_read(void);
static __always_inline void pageroot_write(phys_addr_t root);

/* Disable and enable paging */
static __always_inline void page_enable_paging(void);
static __always_inline void page_disable_paging(void);

/* Disable and enable PAE (Physical Address Extension) */
static __always_inline void page_enable_pae(void);
static __always_inline void page_disable_pae(void);

/* Flush TLB (Translation Lookaside Buffer) */
static __always_inline void page_flush_tlb(virt_addr_t addr);
static __always_inline void page_flush_tlb_all(void);

#ifdef __x86_64__
#include <asm/page_64.h>
#else
#include <asm/page_32.h>
#endif /* __x86_64__ */

static __always_inline phys_addr_t pageroot_read(void)
{
    return (phys_addr_t)cr3_read() & CR3_PDBR_MASK;
}

static __always_inline void pageroot_write(phys_addr_t root)
{
    unsigned long cr3;
    cr3 = (cr3_read() & ~CR3_PDBR_MASK) | ((unsigned long)root & CR3_PDBR_MASK);
    cr3_write(cr3);
}

static __always_inline void page_enable_paging(void)
{
    unsigned long cr0;
    cr0 = cr0_read();
    cr0 |= CR0_PG_MASK;
    cr0_write(cr0);
}

static __always_inline void page_disable_paging(void)
{
    unsigned long cr0;
    cr0 = cr0_read();
    cr0 &= ~CR0_PG_MASK;
    cr0_write(cr0);
}

static __always_inline void page_enable_pae(void)
{
    unsigned long cr4;
    cr4 = cr4_read();
    cr4 |= CR4_PAE_MASK;
    cr4_write(cr4);
}

static __always_inline void page_disable_pae(void)
{
    unsigned long cr4;
    cr4 = cr4_read();
    cr4 &= ~CR4_PAE_MASK;
    cr4_write(cr4);
}

static __always_inline void page_flush_tlb(virt_addr_t addr)
{
    asm volatile("invlpg (%0)" : : "r"(addr) : "memory");
}

static __always_inline void page_flush_tlb_all(void)
{
    pageroot_write(pageroot_read()); /* simply re-write cr3 */
}

#endif