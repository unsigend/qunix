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

#include <kernel/clock.h>
#include <kernel/cpu.h>
#include <kernel/init.h>
#include <kernel/mm.h>
#include <kernel/printk.h>
#include <kernel/task.h>

int kernel_main(void)
{
    cpu_init();
    clock_init();
    mm_init();
    task_init();

    LOGM(LOG_LEVEL_INFO, "KERNEL", "Kernel initialized successfully");

    return 0;
}