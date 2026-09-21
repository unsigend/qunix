/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/kernel.h>
#include <qunix/log.h>
#include <qunix/mm/memmap.h>
#include <qunix/mm/mm.h>

void mm_init(void)
{
    memmap_init();
    LOGM(LOG_LEVEL_INFO, "MM", "Memory Manager initialized successfully");
}