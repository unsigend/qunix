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

#include <kernel/mm.h>
#include <kernel/printk.h>

#include <kernel/mm/memmap.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

void mm_init(void)
{
    memmap_init();
    pmm_init();
    page_init();
    LOGM(LOG_LEVEL_INFO, "MM", "Memory Manager initialized successfully");
}