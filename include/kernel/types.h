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

#ifndef _QUNIX_TYPES_H_
#define _QUNIX_TYPES_H_

#include <kernel/stddef.h>
#include <kernel/stdint.h>

typedef __PTRDIFF_TYPE__ ssize_t;

typedef int32_t pid_t;

typedef uintptr_t phys_addr_t;
typedef uintptr_t virt_addr_t;

typedef uintptr_t pagetable_t;

#endif /* _QUNIX_TYPES_H_ */