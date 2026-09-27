/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/cpu.h>
#include <qunix/kernel.h>
#include <qunix/log.h>
#include <qunix/mm/mm.h>
#include <qunix/task.h>

int kernel_main(void)
{
    cpu_init();
    mm_init();
    task_init();

    LOGM(LOG_LEVEL_INFO, "KERNEL", "Kernel initialized successfully");

    return 0;
}