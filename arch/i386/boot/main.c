/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/multiboot2.h>
#include <driver/vga.h>
#include <qunix/kernel.h>
#include <stdint.h>

int boot_main(uint32_t magic, uintptr_t mbi);

int boot_main(uint32_t magic, uintptr_t mbi)
{
    (void)mbi;

    vga_init((void *)VGA_BUFBASE);
    printk("[INIT] VGA display driver initialized successfully\n");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        panic("Invalid multiboot2 magic number: 0x%x\n", magic);

    while (1)
        ;

    return 0;
}