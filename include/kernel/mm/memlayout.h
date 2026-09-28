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

#ifndef _QUNIX_MM_MEMLAYOUT_H_
#define _QUNIX_MM_MEMLAYOUT_H_

#include <kernel/config.h>

#define KERNEL_VIRT_BASE CONFIG_KERNEL_VIRT_BASE

#ifndef __ASSEMBLER__

#include <kernel/compiler.h>
#include <kernel/types.h>

/* Kernel physical memory layout */
CONTRACT extern const phys_addr_t kernel_phys_start;
CONTRACT extern const phys_addr_t kernel_phys_end;

#define KERNEL_PHYS_START kernel_phys_start
#define KERNEL_PHYS_END kernel_phys_end
#define KERNEL_PHYS_SIZE (KERNEL_PHYS_END - KERNEL_PHYS_START)

/* Kernel virtual memory layout */
CONTRACT extern const virt_addr_t kernel_virt_start;
CONTRACT extern const virt_addr_t kernel_virt_end;

CONTRACT extern const size_t
    kernel_virt_max; /* maximum virtual space in bytes */
#define KERNEL_VIRT_MAX kernel_virt_max

#define KERNEL_VIRT_START kernel_virt_start
#define KERNEL_VIRT_END kernel_virt_end
#define KERNEL_VIRT_SIZE (KERNEL_VIRT_END - KERNEL_VIRT_START)

static __always_inline virt_addr_t phys_to_virt(phys_addr_t pa)
{
    return (virt_addr_t)(pa + KERNEL_VIRT_BASE);
}

static __always_inline phys_addr_t virt_to_phys(virt_addr_t va)
{
    return (phys_addr_t)(va - KERNEL_VIRT_BASE);
}

#endif /* __ASSEMBLER__ */

#endif /* _QUNIX_MM_MEMLAYOUT_H_ */