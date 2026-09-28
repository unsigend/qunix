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

#ifndef _QUNIX_MM_VM_H_
#define _QUNIX_MM_VM_H_

#include <kernel/compiler.h>
#include <kernel/types.h>

#define VM_PERM_READ 0x01
#define VM_PERM_WRITE 0x02
#define VM_PERM_EXEC 0x04
#define VM_PERM_USER 0x08

CONTRACT extern pagetable_t vm_alloc_pagetable(void);

CONTRACT extern int vm_map_page(pagetable_t pagetable, virt_addr_t va,
                                phys_addr_t pa, uint32_t perm);
CONTRACT extern int vm_unmap_page(pagetable_t pagetable, virt_addr_t va);

extern int vm_map_pages(pagetable_t pagetable, virt_addr_t va, phys_addr_t pa,
                        size_t n, uint32_t perm);
extern int vm_unmap_pages(pagetable_t pagetable, virt_addr_t va, size_t n);

#endif /* _QUNIX_MM_VM_H_ */