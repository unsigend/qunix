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

#include <kernel/types.h>

#define VM_FAULT_PRESENT (1u << 0)
#define VM_FAULT_WRITE (1u << 1)
#define VM_FAULT_USER (1u << 2)
#define VM_FAULT_EXEC (1u << 3)

extern void vm_handle_fault(virt_addr_t addr, unsigned int flags) __noreturn;

#endif /* _QUNIX_MM_VM_H_ */