/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/i8253.h>
#include <asm/i8259.h>
#include <qunix/clock.h>
#include <qunix/irq.h>
#include <qunix/log.h>
#include <stddef.h>

void clock_init(void)
{
    irq_register(X86_IRQ_TIMER, clock_tick, NULL);
    i8253_init(1000); /* 1000 Hz */
    i8259_clear_mask(X86_IRQ_TIMER);

    LOGM(LOG_LEVEL_INFO, "CLOCK", "Clock initialized successfully");
}