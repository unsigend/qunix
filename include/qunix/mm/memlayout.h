/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_MEMLAYOUT_H
#define QUNIX_MM_MEMLAYOUT_H

#include <qunix/config.h>

#define KERNEL_VIRT_BASE CONFIG_KERNEL_VIRT_BASE

#ifndef ASM_FILE

#include <qunix/macros.h>
#include <qunix/mm/types.h>
#include <stddef.h>
#include <stdint.h>

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

/* Convert between physical and virtual addresses */
static inline virt_addr_t phys_to_virt(phys_addr_t pa)
{
    return (virt_addr_t)(pa + KERNEL_VIRT_BASE);
}

static inline phys_addr_t virt_to_phys(virt_addr_t va)
{
    return (phys_addr_t)(va - KERNEL_VIRT_BASE);
}

#endif

#endif