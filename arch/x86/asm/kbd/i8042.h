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

#ifndef _KBD_I8042_H_
#define _KBD_I8042_H_

#include <asm/io.h>

#include <kernel/compiler.h>

#define PS2_REG_DATA 0x60
#define PS2_REG_STATUS 0x64
#define PS2_REG_CMD 0x64

#define PS2_ST_OUT 0 /* Output buffer status */
#define PS2_ST_IN 1  /* Input buffer status */
#define PS2_ST_SYS 2 /* System flag */
#define PS2_ST_CMD                                                             \
    3 /* Command/status register select 0 = data written to input buffer is    \
         data for PS/2 device, 1 = data written to input buffer is data for    \
         PS/2 controller command */
#define PS2_ST_UNKNOWN1 4 /* Unknown status bit 1 */
#define PS2_ST_UNKNOWN2 5 /* Unknown status bit 2 */
#define PS2_ST_TIMEOUT 6  /* Timeout */
#define PS2_ST_PARITY 7   /* Parity error */

#define PS2_ST_OUT_MASK (1u << PS2_ST_OUT)
#define PS2_ST_IN_MASK (1u << PS2_ST_IN)
#define PS2_ST_SYS_MASK (1u << PS2_ST_SYS)
#define PS2_ST_CMD_MASK (1u << PS2_ST_CMD)
#define PS2_ST_UNKNOWN1_MASK (1u << PS2_ST_UNKNOWN1)
#define PS2_ST_UNKNOWN2_MASK (1u << PS2_ST_UNKNOWN2)
#define PS2_ST_TIMEOUT_MASK (1u << PS2_ST_TIMEOUT)
#define PS2_ST_PARITY_MASK (1u << PS2_ST_PARITY)

static __always_inline int ps2_outbuf_empty(void);
static __always_inline int ps2_inbuf_empty(void);

static __always_inline int ps2_outbuf_empty(void)
{
    return !(inb(PS2_REG_STATUS) & PS2_ST_OUT_MASK);
}

static __always_inline int ps2_inbuf_empty(void)
{
    return !(inb(PS2_REG_STATUS) & PS2_ST_IN_MASK);
}

#endif /* _KBD_I8042_H_ */
