/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_CLOCK_H
#define QUNIX_CLOCK_H

#include <qunix/macros.h>

extern volatile unsigned long
    jiffies; /* jiffies holds the number of ticks that have occurred since the
                system booted */

CONTRACT extern void clock_init(void);
extern void clock_tick(void *data);

#endif