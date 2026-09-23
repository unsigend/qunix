/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/mb2.h>
#include <driver/vga.h>
#include <qunix/kernel.h>
#include <qunix/log.h>
#include <qunix/mm/memlayout.h>
#include <stdint.h>

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