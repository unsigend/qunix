/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_MEMLAYOUT_H
#define QUNIX_MM_MEMLAYOUT_H

#include <qunix/macros.h>
#include <stdint.h>

CONTRACT extern const uintptr_t kernel_phys_start;
CONTRACT extern const uintptr_t kernel_phys_end;

#define KERNEL_PHYS_START kernel_phys_start
#define KERNEL_PHYS_END kernel_phys_end
#define KERNEL_PHYS_SIZE (KERNEL_PHYS_END - KERNEL_PHYS_START)

#endif