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

#include <kernel/mm/memlayout.h>

extern char _kernel_phys_start[];
extern char _kernel_phys_end[];
extern char _kernel_virt_start[];
extern char _kernel_virt_end[];

const phys_addr_t kernel_phys_start = (phys_addr_t)_kernel_phys_start;
const phys_addr_t kernel_phys_end = (phys_addr_t)_kernel_phys_end;

const virt_addr_t kernel_virt_start = (virt_addr_t)_kernel_virt_start;
const virt_addr_t kernel_virt_end = (virt_addr_t)_kernel_virt_end;

const size_t kernel_virt_max = 1UL << 30; /* 1GB space: 3GB - 4GB */