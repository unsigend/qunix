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
#include <kernel/rtc.h>
#include <kernel/sprintf.h>

#define MONTH 12
#define WEEKDAY 7

static const char *monthstr[MONTH] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static const char *wdaystr[WEEKDAY] = {"Sun", "Mon", "Tue", "Wed",
                                       "Thu", "Fri", "Sat"};

const char *rtc_month_name(int month)
{
    return (month >= 1 && month <= MONTH) ? monthstr[month - 1] : "???";
}

const char *rtc_weekday_name(int weekday)
{
    return (weekday >= 1 && weekday <= WEEKDAY) ? wdaystr[weekday - 1] : "???";
}

int rtc_fmttime(struct rtc_time *time, char *buf, size_t size)
{
    if (!time || !buf)
        return -EINVAL;

    return snprintf(buf, size, "%s %2d %s %4d %02d:%02d:%02d",
                    rtc_weekday_name(time->weekday), time->day,
                    rtc_month_name(time->month), time->year, time->hour,
                    time->min, time->sec);
}