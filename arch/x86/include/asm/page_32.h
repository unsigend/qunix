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

#ifndef _X86_ASM_PAGE_32_H_
#define _X86_ASM_PAGE_32_H_

#include <kernel/compiler.h>
#include <kernel/mm/memlayout.h>
#include <kernel/stdbool.h>
#include <kernel/types.h>

#define PG_P (1u << 0)    /* Present */
#define PG_WR (1u << 1)   /* Writable */
#define PG_USER (1u << 2) /* User Accessible */
#define PG_PWT (1u << 3)  /* Write Through */
#define PG_PCD (1u << 4)  /* Cache Disabled */
#define PG_ACC (1u << 5)  /* Accessed */

typedef uint32_t pde_t; /* page directory entry */
typedef uint32_t pte_t; /* page table entry */

static __always_inline uint32_t pde_index(virt_addr_t va);
static __always_inline uint32_t pte_index(virt_addr_t va);
static __always_inline uint32_t page_offset(virt_addr_t va);

static __always_inline pn_t va_to_pn(virt_addr_t va);
static __always_inline pn_t pa_to_pn(phys_addr_t pa);
static __always_inline phys_addr_t pn_to_pa(pn_t pn);
static __always_inline virt_addr_t pn_to_va(pn_t pn);

/* page table entries functions */

static __always_inline void pte_make(pte_t *pte, pn_t pn, unsigned int flags);
static __always_inline pn_t pte_get_pn(pte_t *pte);

static __always_inline bool pte_is_dirty(pte_t *pte);
static __always_inline bool pte_is_pat(pte_t *pte);
static __always_inline bool pte_is_global(pte_t *pte);
static __always_inline void pte_set_dirty(pte_t *pte, bool dirty);
static __always_inline void pte_set_pat(pte_t *pte, bool pat);
static __always_inline void pte_set_global(pte_t *pte, bool global);

#define pte_clear_dirty(pte) pte_set_dirty(pte, false)
#define pte_clear_pat(pte) pte_set_pat(pte, false)
#define pte_clear_global(pte) pte_set_global(pte, false)

/* page directory entries functions */

static __always_inline void pde_make(pde_t *pde, pn_t pn, unsigned int flags);
static __always_inline void pde_huge_make(pde_t *pde, phys_addr_t pa,
                                          unsigned int flags);
static __always_inline pn_t pde_get_pn(pde_t *pde);
static __always_inline phys_addr_t pde_huge_get_pa(pde_t *pde);
static __always_inline phys_addr_t pde_get_pt_pa(pde_t *pde);
static __always_inline virt_addr_t pde_get_pt_va(pde_t *pde);

static __always_inline bool pde_huge_is_dirty(pde_t *pde);
static __always_inline bool pde_huge_is_pat(pde_t *pde);
static __always_inline bool pde_huge_is_global(pde_t *pde);
static __always_inline void pde_huge_set_dirty(pde_t *pde, bool dirty);
static __always_inline void pde_huge_set_pat(pde_t *pde, bool pat);
static __always_inline void pde_huge_set_global(pde_t *pde, bool global);

#define pde_huge_clear_dirty(pde) pde_huge_set_dirty(pde, false)
#define pde_huge_clear_pat(pde) pde_huge_set_pat(pde, false)
#define pde_huge_clear_global(pde) pde_huge_set_global(pde, false)

#define PN_SHIFT 12

static __always_inline uint32_t pde_index(virt_addr_t va)
{
    return (va >> 22) & 0x3FF; /* va[31:22] */
}
static __always_inline uint32_t pte_index(virt_addr_t va)
{
    return (va >> 12) & 0x3FF; /* va[21:12] */
}
static __always_inline uint32_t page_offset(virt_addr_t va)
{
    return va & 0xFFF; /* va[11:0] */
}

static __always_inline pn_t va_to_pn(virt_addr_t va)
{
    return (va >> PN_SHIFT) & 0xFFFFFu;
}
static __always_inline pn_t pa_to_pn(phys_addr_t pa)
{
    return (pa >> PN_SHIFT) & 0xFFFFFu;
}

static __always_inline phys_addr_t pn_to_pa(pn_t pn) { return pn << PN_SHIFT; }
static __always_inline virt_addr_t pn_to_va(pn_t pn)
{
    phys_addr_t pa;
    pa = pn_to_pa(pn);
    return pa_to_va(pa);
}

static __always_inline void pte_make(pte_t *pte, pn_t pn, unsigned int flags)
{
    *pte = (pn << PN_SHIFT) | flags;
}
static __always_inline pn_t pte_get_pn(pte_t *pte)
{
    return (*pte >> PN_SHIFT) & 0xFFFFFu;
}

static __always_inline bool pte_is_dirty(pte_t *pte)
{
    return (*pte & (1u << 6)) != 0;
}
static __always_inline bool pte_is_pat(pte_t *pte)
{
    return (*pte & (1u << 7)) != 0;
}
static __always_inline bool pte_is_global(pte_t *pte)
{
    return (*pte & (1u << 8)) != 0;
}

static __always_inline void pte_set_dirty(pte_t *pte, bool dirty)
{
    if (dirty)
        *pte |= (1u << 6);
    else
        *pte &= ~(1u << 6);
}
static __always_inline void pte_set_pat(pte_t *pte, bool pat)
{
    if (pat)
        *pte |= (1u << 7);
    else
        *pte &= ~(1u << 7);
}
static __always_inline void pte_set_global(pte_t *pte, bool global)
{
    if (global)
        *pte |= (1u << 8);
    else
        *pte &= ~(1u << 8);
}

static __always_inline void pde_make(pde_t *pde, pn_t pn, unsigned int flags)
{
    *pde = (pn << PN_SHIFT) | flags;
}
static __always_inline void pde_huge_make(pde_t *pde, phys_addr_t pa,
                                          unsigned int flags)
{
    *pde = (pa & 0xFFC00000u) | flags;
    *pde |= (1u << 7); /* PS bit */
}
static __always_inline pn_t pde_get_pn(pde_t *pde)
{
    return (*pde >> PN_SHIFT) & 0xFFFFFu;
}
static __always_inline phys_addr_t pde_huge_get_pa(pde_t *pde)
{
    return (*pde & 0xFFC00000u);
}
static __always_inline phys_addr_t pde_get_pt_pa(pde_t *pde)
{
    return *pde & 0xFFFFF000u;
}
static __always_inline virt_addr_t pde_get_pt_va(pde_t *pde)
{
    return pa_to_va(pde_get_pt_pa(pde));
}

static __always_inline bool pde_huge_is_dirty(pde_t *pde)
{
    return (*pde & (1u << 6)) != 0;
}
static __always_inline bool pde_huge_is_pat(pde_t *pde)
{
    return (*pde & (1u << 12)) != 0;
}
static __always_inline bool pde_huge_is_global(pde_t *pde)
{
    return (*pde & (1u << 8)) != 0;
}

static __always_inline void pde_huge_set_dirty(pde_t *pde, bool dirty)
{
    if (dirty)
        *pde |= (1u << 6);
    else
        *pde &= ~(1u << 6);
}
static __always_inline void pde_huge_set_pat(pde_t *pde, bool pat)
{
    if (pat)
        *pde |= (1u << 12);
    else
        *pde &= ~(1u << 12);
}
static __always_inline void pde_huge_set_global(pde_t *pde, bool global)
{
    if (global)
        *pde |= (1u << 8);
    else
        *pde &= ~(1u << 8);
}

#endif /* _X86_ASM_PAGE_32_H_ */