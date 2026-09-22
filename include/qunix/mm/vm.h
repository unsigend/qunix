/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_VM_H
#define QUNIX_MM_VM_H

#include <qunix/macros.h>
#include <qunix/mm/types.h>
#include <stddef.h>
#include <stdint.h>

#define VM_PERM_READ 0x01
#define VM_PERM_WRITE 0x02
#define VM_PERM_EXEC 0x04
#define VM_PERM_USER 0x08

/* Map one page of physical page to virtual page, return 0 on success -errno on
 * error, if alloc is set, then allocate necessary tables. */
CONTRACT extern int vm_map_page(pagetable_t pagetable, virt_addr_t va,
                                phys_addr_t pa, uint32_t perm, int alloc);

/* Unmap the page at virtual address  */
CONTRACT extern void vm_unmap_page(pagetable_t pagetable, virt_addr_t va);

extern int vm_map_pages(pagetable_t pagetable, virt_addr_t va, phys_addr_t pa,
                        size_t n, uint32_t perm, int alloc);
extern void vm_unmap_pages(pagetable_t pagetable, virt_addr_t va, size_t n);

#endif