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

#include "pit.h"

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/pit.h>

#include <kernel/clock.h>
#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/sched.h>

static uint16_t chan_to_port(uint16_t chan)
{
    switch (chan) {
    case PIT_CHAN0: return PIT_CHAN0_DATA;
    case PIT_CHAN1: return PIT_CHAN1_DATA;
    case PIT_CHAN2: return PIT_CHAN2_DATA;
    default: panic("invalid PIT channel: %u", chan);
    }
}

static void set_counter(unsigned count, uint16_t chan)
{
    uint16_t port = chan_to_port(chan);

    if (count > PIT_COUNTER_MAX)
        panic("PIT counter value is too high");

    outb(port, count & 0xFF);
    outb(port, (count >> 8) & 0xFF);
}

__unused static unsigned read_counter(uint16_t chan)
{
    uint16_t port = chan_to_port(chan);

    outb(PIT_CMD_PORT, chan << 6);

    return inb(port) | (inb(port) << 8);
}

static irqreturn_t pit_tick(unsigned int irq, void *dev)
{
    UNUSED(irq);
    UNUSED(dev);
    jiffies++;
    sched_tick();
    return IRQ_HANDLED;
}

void pit_init(void)
{
    unsigned counter;
    int err;

    counter = PIT_FREQ_HZ / CLOCK_HZ;

    outb(PIT_CMD_PORT,
         0b00110100); /* Channel 0, lobyte/hibyte, Mode 2 (rate generator) */
    set_counter(counter, PIT_CHAN0);

    if ((err = irq_register(IRQ_TIMER, pit_tick, 0, "8253/8254 PIT", NULL)))
        panic_error(err, "failed to register PIT IRQ");

    LOGM_INFO("PIT", "8253/8254 PIT initialized with %u Hz", CLOCK_HZ);
}