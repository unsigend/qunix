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

#ifndef _QUNIX_RTC_H_
#define _QUNIX_RTC_H_

#include <kernel/compiler.h>
#include <kernel/stddef.h>

#define RTC_TIMEBUF_SIZE 32

struct rtc_time {
    unsigned int sec;
    unsigned int min;
    unsigned int hour;
    unsigned int weekday;
    unsigned int day;
    unsigned int month;
    unsigned int year;
};

CONTRACT extern int rtc_gettime(struct rtc_time *time);
extern int rtc_fmttime(struct rtc_time *time, char *buf, size_t size);

extern const char *rtc_month_name(int month);
extern const char *rtc_weekday_name(int weekday);

#endif /* _QUNIX_RTC_H_ */