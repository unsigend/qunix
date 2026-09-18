/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/cpu.h>
#include <qunix/kernel.h>
#include <qunix/mm/mm.h>

int kernel_main(void *ctx)
{
    cpu_init();
    printk("[INIT] CPU initialized successfully\n");

    mm_init(ctx);
    printk("[INIT] Memory Manager initialized successfully\n");

    return 0;
}