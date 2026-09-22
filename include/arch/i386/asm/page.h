/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_PAGE_H
#define ARCH_I386_ASM_PAGE_H

#include <qunix/mm/types.h>
#include <stdint.h>

#define VIRTADDR_OFFSET(va) ((va) & 0xFFF)
#define VIRTADDR_PGDIR_IDX(va) ((va) >> 22)
#define VIRTADDR_PGTABLE_IDX(va) (((va) >> 12) & 0x3FF)

/* Set or get the page directory root address in CPU. */
extern void page_set_pagedir(uintptr_t pagedir);
extern uintptr_t page_get_pagedir(void);

typedef uint32_t pte_t; /* page table entry */
typedef uint32_t pde_t; /* page directory entry */

#define PG_P_MASK 0x001         /* Present*/
#define PG_RW_MASK 0x002        /* Read/Write */
#define PG_USER_MASK 0x004      /* User accessible */
#define PG_PWT_MASK 0x008       /* Write through / Write back */
#define PG_PCD_MASK 0x010       /* Cache disabled */
#define PG_A_MASK 0x020         /* Accessed */
#define PG_ADDR_MASK 0xFFFFF000 /* Address mask */

#define PDE_PS_MASK 0x080 /* Page size 1=4MB, 0=4KB */

#define PTE_DIRTY_MASK 0x040 /* Dirty */
#define PTE_PAT_MASK                                                           \
    0x080 /* Page attribute table,  If PAT is supported, then PAT along with   \
             PCD and PWT shall indicate the memory caching type. Otherwise, it \
             is reserved and must be set to 0. */
#define PTE_G_MASK                                                             \
    0x100 /* Global, tell the processor not to invalidate the page when TLB is \
             flushed */

extern uint32_t pde_make_flag(int present, int write, int user);
extern uint32_t pte_make_flag(int present, int write, int user, int pat,
                              int global);

extern pde_t pde_make(phys_addr_t pa, uint32_t flags);
extern pte_t pte_make(phys_addr_t pa, uint32_t flags);

#endif