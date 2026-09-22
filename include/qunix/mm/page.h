/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MM_PAGE_H
#define QUNIX_MM_PAGE_H

#include <qunix/config.h>
#include <qunix/macros.h>

#define DEFAULT_PAGE_SIZE 4096
#define DEFAULT_PAGE_SHIFT 12

#ifndef CONFIG_PAGESIZE
#define PAGE_SIZE DEFAULT_PAGESIZE
#else
#define PAGE_SIZE CONFIG_PAGESIZE
#endif

#ifndef CONFIG_PAGESHIFT
#define PAGE_SHIFT DEFAULT_PAGESHIFT
#else
#define PAGE_SHIFT CONFIG_PAGESHIFT
#endif

CONTRACT extern void page_init(void);

#endif