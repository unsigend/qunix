/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm.h>
#include <asm/i8253.h>
#include <qunix/error.h>
#include <qunix/log.h>
#include <stdint.h>

/* The 8253 has three counters and one command port. Channel 0 is directly
 * connected to the IRQ0 (timer), Channel 1 is unusable and may not exists,
 * Channel 2 is connected to the PC speaker. */

#define CHAN0_DATA_PORT 0x40
#define CHAN1_DATA_PORT 0x41
#define CHAN2_DATA_PORT 0x42

/* command port layout:
    Bits         Usage
    7 and 6      Select channel :
                    0 0 = Channel 0
                    0 1 = Channel 1
                    1 0 = Channel 2
                    1 1 = Read-back command (8254 only)
    5 and 4      Access mode :
                    0 0 = Latch count value command
                    0 1 = Access mode: lobyte only
                    1 0 = Access mode: hibyte only
                    1 1 = Access mode: lobyte/hibyte
    3 to 1       Operating mode :
                    0 0 0 = Mode 0 (interrupt on terminal count)
                    0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                    0 1 0 = Mode 2 (rate generator)
                    0 1 1 = Mode 3 (square wave generator)
                    1 0 0 = Mode 4 (software triggered strobe)
                    1 0 1 = Mode 5 (hardware triggered strobe)
                    1 1 0 = Mode 2 (rate generator, same as 010b)
                    1 1 1 = Mode 3 (square wave generator, same as 011b)
    0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/
#define CMD_PORT 0x43

#define MODE_0 0x00 /* Interrupt on terminal count */
#define MODE_1 0x01 /* Hardware Re-triggerable one-shot */
#define MODE_2 0x02 /* Rate Generator */
#define MODE_3 0x03 /* Square Wave Generator */
#define MODE_4 0x04 /* Software Triggered Strobe */
#define MODE_5 0x05 /* Hardware Triggered Strobe */

/* The oscillator used by the PIT chip runs at (roughly) 1.193182 MHz */
#define I8253_FREQ 1193182u

int i8253_init(uint32_t hz)
{
    uint32_t counter; /* counter in I8253 is 16-bit */

    if (hz == 0 || hz > I8253_FREQ)
        return -EINVAL;

    counter = I8253_FREQ / hz;
    if (counter > UINT16_MAX)
        return -EINVAL;

    outb(CMD_PORT, 0x34); /* 0x34 = 0b00110100 Channel 0, lobyte/hibyte,
                             Mode 2 Rate Generator */
    outb(CHAN0_DATA_PORT, counter & 0xFF);
    outb(CHAN0_DATA_PORT, counter >> 8);

    LOGM(LOG_LEVEL_INFO, "8253 PIT", "Initialized with frequency %u Hz", hz);

    return 0;
}