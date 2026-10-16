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
#define PAGEF_USER (1u << 1)
#define PAGEF_EXEC (1u << 2)

#ifndef __ASSEMBLER__

#include <kernel/compiler.h>
#include <kernel/string.h>
#include <kernel/types.h>

#define page_clear(page) memset((void *)(page), 0, PAGE_SIZE)
#define page_copy(dest, src) memcpy((void *)(dest), (void *)(src), PAGE_SIZE)

/* virtual address of the page table */
typedef uintptr_t pagetable_t;

/*
 * page_map - map the page at @va to the frame at @pa.
 *
 * @va and @pa must be page-aligned. A missing intermediate page table is
 * allocated. An existing mapping at @va is replaced. If @root is the active
 * page table, the TLB entry for @va is flushed.
 *
 * Returns 0 on success, -EINVAL if @root is NULL or an address is
 * misaligned, -ENOMEM if a page table cannot be allocated.
 */
CONTRACT extern int page_map(pagetable_t root, phys_addr_t pa, virt_addr_t va,
                             unsigned int flags);

/*
 * page_unmap - unmap the page at @va.
 *
 * A NULL @root, a misaligned @va, or an absent mapping is ignored. If @root
 * is the active page table, the TLB entry for @va is flushed.
 */
CONTRACT extern void page_unmap(pagetable_t root, virt_addr_t va);

/*
 * page_map_range - map @n contiguous pages starting at @pa to @va.
 *
 * If mapping one page fails, every page mapped by this call is unmapped.
 *
 * Returns 0 on success, or the error from page_map().
 */
extern int page_map_range(pagetable_t root, phys_addr_t pa, virt_addr_t va,
                          size_t n, unsigned int flags);
extern void page_unmap_range(pagetable_t root, virt_addr_t va, size_t n);

/*
 * page_lookup - store the physical address mapped at @va in @pa.
 *
 * The page tables are not modified.
 *
 * Returns 0 on success, -EINVAL if @root is NULL or @va is misaligned,
 * -ENOENT if @va is not mapped.
 */
CONTRACT extern int page_lookup(pagetable_t root, virt_addr_t va,
                                phys_addr_t *pa);

#endif /* __ASSEMBLER__ */

#endif /* _QUNIX_MM_PAGE_H_ */