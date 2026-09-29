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

#include <asm/mb2.h>
#include <asm/multiboot2.h>

#include <driver/vga.h>

#include <kernel/init.h>
#include <kernel/mm/memlayout.h>
#include <kernel/printk.h>

/* Forward declarations: only boot.S calls this function */
int bootmain(uint32_t, uintptr_t);

int bootmain(uint32_t magic, uintptr_t mbi)
{
    vga_init((void *)phys_to_virt(VGA_BUFBASE));
    LOGM(LOG_LEVEL_INFO, "VGA", "VGA driver initialized successfully");

    mb2_validate(magic, mbi);
    mb2_set_mbi(mbi);

    kernel_main();

    return 0;
}