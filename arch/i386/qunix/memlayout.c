/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/mm/memlayout.h>

extern char _kernel_phys_start[];
extern char _kernel_phys_end[];

const uintptr_t kernel_phys_start = (uintptr_t)_kernel_phys_start;
const uintptr_t kernel_phys_end = (uintptr_t)_kernel_phys_end;