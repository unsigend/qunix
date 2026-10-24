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

#include <kernel/errno.h>
#include <kernel/interrupt.h>
#include <kernel/stdint.h>

#include <asm/cmos.h>
#include <asm/rtc.h>

static inline uint8_t bcd_to_bin(uint8_t bcd)
{
    return ((bcd & 0xF0) >> 1) + ((bcd & 0xF0) >> 3) + (bcd & 0x0F);
}

static inline int cmos_uip(void)
{
    return cmos_read(CMOS_REG_STATUS_A) & CMOS_STA_UIP;
}

int cmos_rtc_gettime(struct rtc_time *time)
{
    unsigned long irqflags;
    uint8_t status;
    uint8_t sec, min, hour, weekday, day, month, year;
    uint8_t _sec, _min, _hour, _weekday, _day, _month, _year;
    uint8_t pm;

    if (!time)
        return -EINVAL;

    irqflags = interrupt_save();

    while (cmos_uip())
        ;
    sec = cmos_read(CMOS_REG_SECONDS);
    min = cmos_read(CMOS_REG_MINUTES);
    hour = cmos_read(CMOS_REG_HOURS);
    weekday = cmos_read(CMOS_REG_WEEKDAY);
    day = cmos_read(CMOS_REG_DAY);
    month = cmos_read(CMOS_REG_MONTH);
    year = cmos_read(CMOS_REG_YEAR);

    do {
        _sec = sec;
        _min = min;
        _hour = hour;
        _weekday = weekday;
        _day = day;
        _month = month;
        _year = year;

        while (cmos_uip())
            ;
        sec = cmos_read(CMOS_REG_SECONDS);
        min = cmos_read(CMOS_REG_MINUTES);
        hour = cmos_read(CMOS_REG_HOURS);
        weekday = cmos_read(CMOS_REG_WEEKDAY);
        day = cmos_read(CMOS_REG_DAY);
        month = cmos_read(CMOS_REG_MONTH);
        year = cmos_read(CMOS_REG_YEAR);
    } while (_sec != sec || _min != min || _hour != hour ||
             _weekday != weekday || _day != day || _month != month ||
             _year != year);

    status = cmos_read(CMOS_REG_STATUS_B);

    interrupt_restore(irqflags);

    pm = 0;
    if (!(status & CMOS_STB_24H)) {
        pm = hour & 0x80;
        hour &= 0x7F;
    }

    if (!(status & CMOS_STB_BINARY)) {
        sec = bcd_to_bin(sec);
        min = bcd_to_bin(min);
        hour = bcd_to_bin(hour);
        weekday = bcd_to_bin(weekday);
        day = bcd_to_bin(day);
        month = bcd_to_bin(month);
        year = bcd_to_bin(year);
    }

    if (!(status & CMOS_STB_24H)) {
        if (hour == 12)
            hour = 0;
        if (pm)
            hour += 12;
    }

    time->sec = sec;
    time->min = min;
    time->hour = hour;
    time->weekday = weekday;
    time->day = day;
    time->month = month;
    time->year = year + ((year < 80) ? 2000 : 1900);

    return 0;
}
