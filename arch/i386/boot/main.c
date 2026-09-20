/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/mb2.h>
#include <driver/vga.h>
#include <qunix/kernel.h>
#include <stdint.h>

/* Forward declarations: only boot.S calls this function */
int bootmain(uint32_t, uintptr_t);

int bootmain(uint32_t magic, uintptr_t mbi)
{
    vga_init((void *)VGA_BUFBASE);
    printk("[INIT] VGA display driver initialized successfully\n");

    mb2_validate(magic, mbi);
    mb2_set_mbi(mbi);

    kernel_main();

    return 0;
}