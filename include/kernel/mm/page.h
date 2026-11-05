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

#ifndef _QUNIX_MM_PAGE_H_
#define _QUNIX_MM_PAGE_H_

#include <kernel/config.h>

#define DEFAULT_PAGE_SHIFT 12
#define DEFAULT_PAGE_SIZE (1UL << DEFAULT_PAGE_SHIFT)

#ifdef CONFIG_PAGESHIFT
#define PAGE_SHIFT CONFIG_PAGESHIFT
#else
#define PAGE_SHIFT DEFAULT_PAGE_SHIFT
#endif /* CONFIG_PAGESHIFT */

#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))
#define PAGE_ALIGN(x) (((x) + PAGE_SIZE - 1) & PAGE_MASK)
#define PAGE_ALIGN_DOWN(x) ((x) & PAGE_MASK)
#define PAGE_ALIGN_UP(x) (PAGE_ALIGN(x))

/* page mapping flags */
#define PAGEF_WRITE (1u << 0)
#define PAGEF_EXEC (1u << 2)

#ifndef __ASSEMBLER__

#include <kernel/string.h>
#include <kernel/types.h>

#define page_clear(page) memset((void *)(page), 0, PAGE_SIZE)
#define page_copy(dest, src) memcpy((void *)(dest), (void *)(src), PAGE_SIZE)

/*
 * A pagetable_t is one process address space. page_map / unmap / lookup
 * and the range wrappers act on its user half only.
 *
 * The kernel page table is private to the arch. page_init builds it once
 * and preallocates every kernel-half page table. Those entries are copied
 * into each process table and never changed after boot.
 */

typedef uintptr_t pagetable_t; /* virtual address of the page table */

extern int page_map(pagetable_t root, phys_addr_t pa, virt_addr_t va,
                    unsigned int flags);
extern int page_unmap(pagetable_t root, virt_addr_t va);
extern int page_map_range(pagetable_t root, phys_addr_t pa, virt_addr_t va,
                          size_t n, unsigned int flags);
extern int page_unmap_range(pagetable_t root, virt_addr_t va, size_t n);
extern int page_lookup(pagetable_t root, virt_addr_t va, phys_addr_t *pa);

extern pagetable_t pagetable_create(void);
extern void pagetable_free(pagetable_t root);
extern int pagetable_copy(pagetable_t dest, pagetable_t src);
extern void pagetable_switch(pagetable_t root);
extern void pagetable_switch_kernel(void);

extern void page_init(void);

#endif /* __ASSEMBLER__ */

#endif /* _QUNIX_MM_PAGE_H_ */