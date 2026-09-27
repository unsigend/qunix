/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/clock.h>
#include <qunix/fmt.h>
#include <qunix/log.h>
#include <qunix/task.h>

volatile unsigned long jiffies = 0;

void clock_tick(void *data)
{
    UNUSED(data);
    jiffies++;

    task_tick();
}