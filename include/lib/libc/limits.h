/* qlibc - A light-weight and portable C standard library
 * Copyright (C) 2025 Qiu Yixiang
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
#ifndef _LIMITS_H_
#define _LIMITS_H_ 1

#include <bits/limits.h>
#include <feature.h>

#define CHAR_BIT 8

#define CHAR_MIN (-128)
#define CHAR_MAX 127

#define SCHAR_MIN (CHAR_MIN)
#define SCHAR_MAX (CHAR_MAX)

#define SHRT_MAX (0x7fff)
#define SHRT_MIN (-1 - SHRT_MAX)

#define INT_MAX (0x7fffffff)
#define INT_MIN (-1 - INT_MAX)

#define LONG_MIN (_LONG_MIN_)
#define LONG_MAX (_LONG_MAX_)

#define LLONG_MAX (0x7fffffffffffffff)
#define LLONG_MIN (-1 - LLONG_MAX)

#define UCHAR_MAX (0xff)
#define USHRT_MAX (0xffff)
#define UINT_MAX (0xffffffff)
#define ULONG_MAX (_ULONG_MAX_)
#define ULLONG_MAX (0xffffffffffffffff)

#if __SUPPORT_MB
#define MB_LEN_MAX 4
#endif

#endif