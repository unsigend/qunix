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

#ifndef _TIMER_PIT_H_
#define _TIMER_PIT_H_

#include <kernel/stdint.h>

/* The Programmable Interval Timer (PIT) chip (Intel 8253/8254). */

#define PIT_COUNTER_MAX UINT16_MAX
#define PIT_FREQ_HZ 1193182u

#define PIT_CHAN0 0x00 /* IRQ 0 (timer) */
#define PIT_CHAN1 0x01
#define PIT_CHAN2 0x02 /* PC speaker */

/*
 * PIT Mode/Command register:
 * Bits         Usage
 *
 * 7 and 6      Select channel :
 *                 0 0 = Channel 0
 *                 0 1 = Channel 1
 *                 1 0 = Channel 2
 *                 1 1 = Read-back command (8254 only)
 *
 * 5 and 4      Access mode :
 *                 0 0 = Latch count value command
 *                 0 1 = Access mode: lobyte only
 *                 1 0 = Access mode: hibyte only
 *                 1 1 = Access mode: lobyte/hibyte
 *
 * 3 to 1       Operating mode :
 *                 0 0 0 = Mode 0 (interrupt on terminal count)
 *                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
 *                 0 1 0 = Mode 2 (rate generator)
 *                 0 1 1 = Mode 3 (square wave generator)
 *                 1 0 0 = Mode 4 (software triggered strobe)
 *                 1 0 1 = Mode 5 (hardware triggered strobe)
 *                 1 1 0 = Mode 2 (rate generator, same as 010b)
 *                 1 1 1 = Mode 3 (square wave generator, same as 011b)
 *
 * 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
 */
#define PIT_CMD_PORT 0x43

#define PIT_CHAN0_DATA 0x40
#define PIT_CHAN1_DATA 0x41
#define PIT_CHAN2_DATA 0x42

#endif /* _TIMER_PIT_H_ */