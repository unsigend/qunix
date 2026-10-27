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

#ifndef _QUNIX_ASM_CMOS_H_
#define _QUNIX_ASM_CMOS_H_

#include <asm/io.h>

#include <kernel/compiler.h>
#include <kernel/stdint.h>

#define CMOS_REG_ADDR 0x70
#define CMOS_REG_DATA 0x71

#define NMI_DISABLE 0x80
#define NMI_ENABLE 0x00

#define CMOS_REG_SECONDS 0x00
#define CMOS_REG_ALARM_SECONDS 0x01
#define CMOS_REG_MINUTES 0x02
#define CMOS_REG_ALARM_MINUTES 0x03
#define CMOS_REG_HOURS 0x04
#define CMOS_REG_ALARM_HOURS 0x05
#define CMOS_REG_WEEKDAY 0x06
#define CMOS_REG_DAY 0x07
#define CMOS_REG_MONTH 0x08
#define CMOS_REG_YEAR 0x09
#define CMOS_REG_STATUS_A 0x0A
#define CMOS_REG_STATUS_B 0x0B
#define CMOS_REG_STATUS_C 0x0C
#define CMOS_REG_STATUS_D 0x0D
#define CMOS_REG_FLOPPY 0x10
#define CMOS_REG_CENTURY 0x32 /* may not exist */

/* CMOS status register A */
#define CMOS_STA_UIP (1 << 7) /* update in progress */
#define CMOS_STA_DV_MASK                                                       \
    0x70 /* bits 6-4: divider select (normally 010 = 32.768 kHz) */
#define CMOS_STA_RS_MASK 0x0F /* bits 3-0: periodic interrupt rate select */

/* CMOS status register B */
#define CMOS_STB_SET (1 << 7)    /* halt updates (to set the clock safely) */
#define CMOS_STB_PIE (1 << 6)    /* periodic interrupt enable */
#define CMOS_STB_AIE (1 << 5)    /* alarm interrupt enable */
#define CMOS_STB_UIE (1 << 4)    /* update-ended interrupt enable */
#define CMOS_STB_SQWE (1 << 3)   /* square wave enable */
#define CMOS_STB_BINARY (1 << 2) /* 1 = binary, 0 = BCD */
#define CMOS_STB_24H (1 << 1)    /* 1 = 24-hour, 0 = 12-hour */
#define CMOS_STB_DSE (1 << 0)    /* daylight saving enable */

/* CMOS status register C */
#define CMOS_STC_IRQF (1 << 7) /* any interrupt pending */
#define CMOS_STC_PF (1 << 6)   /* periodic interrupt fired */
#define CMOS_STC_AF (1 << 5)   /* alarm fired */
#define CMOS_STC_UF (1 << 4)   /* update ended */

/* Status D (0x0D), read-only */
#define CMOS_STD_VRT (1 << 7) /* 1 = battery/RAM valid; 0 = battery dead */

static __always_inline void cmos_select_reg(uint8_t reg);
static __always_inline uint8_t cmos_read(uint8_t reg);

static __always_inline void cmos_select_reg(uint8_t reg)
{
    outb(CMOS_REG_ADDR, reg | NMI_DISABLE);
}

static __always_inline uint8_t cmos_read(uint8_t reg)
{
    cmos_select_reg(reg);
    return inb(CMOS_REG_DATA);
}

#endif /* _QUNIX_ASM_CMOS_H_ */