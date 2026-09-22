/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_TYPES_H
#define QUNIX_MM_TYPES_H

#include <stdint.h>

typedef uintptr_t phys_addr_t;
typedef uintptr_t virt_addr_t;

/* An opaque pointer representing a page table */
typedef uintptr_t pagetable_t;

#endif