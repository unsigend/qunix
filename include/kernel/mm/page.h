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

#include <kernel/compiler.h>
#include <kernel/config.h>
#include <kernel/types.h>

#ifndef CONFIG_PAGESHIFT
#define PAGE_SHIFT 12
#else
#define PAGE_SHIFT CONFIG_PAGESHIFT
#endif

#define PAGE_SIZE (1UL << PAGE_SHIFT)

CONTRACT extern void page_init(void);

CONTRACT extern pagetable_t kernel_pagetable; /* kernel page table */

/* Flush the TLB (Translation Lookaside Buffer) for all virtual addresses, these
 * functions are based on hardware implementation.  */
CONTRACT extern void page_flush_tlb_all(void);
CONTRACT extern void page_flush_tlb_one(virt_addr_t va);

#endif /* _QUNIX_MM_PAGE_H_ */