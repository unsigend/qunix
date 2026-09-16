/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/gdt.h>
#include <qunix/cpu.h>
#include <qunix/kernel.h>

void cpu_halt(void)
{
    for (;;)
        asm volatile("hlt");
}

void cpu_init(void)
{
    gdt_init();
    printk("[INIT] Global Descriptor Table (GDT) initialized successfully\n");
}