/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/mm/memlayout.h>

extern char _kernel_phys_start[];
extern char _kernel_phys_end[];
extern char _kernel_virt_start[];
extern char _kernel_virt_end[];

const phys_addr_t kernel_phys_start = (phys_addr_t)_kernel_phys_start;
const phys_addr_t kernel_phys_end = (phys_addr_t)_kernel_phys_end;

const virt_addr_t kernel_virt_start = (virt_addr_t)_kernel_virt_start;
const virt_addr_t kernel_virt_end = (virt_addr_t)_kernel_virt_end;

const size_t kernel_virt_max = 1UL << 30; /* 1GB space: 3GB - 4GB */