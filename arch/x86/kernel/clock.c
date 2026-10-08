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

#include <asm/i8253.h>
#include <asm/irq.h>

#include <kernel/clock.h>
#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/task.h>

static irqreturn_t tick(unsigned int irq, void *dev)
{
    UNUSED(irq);
    UNUSED(dev);

    jiffies++;
    task_tick();

    return IRQ_HANDLED;
}

void clock_init(void)
{
    if (irq_register(X86_IRQ_TIMER, tick, 0, "i8253 clock", NULL))
        panic("Failed to register i8253 clock");

    i8253_init(1000); /* 1000 Hz */

    LOGM(LOG_LEVEL_INFO, "CLOCK", "Clock initialized successfully");
}