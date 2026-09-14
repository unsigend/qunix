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

#ifndef _BITS_STDINT_H_
#define _BITS_STDINT_H_ 1

#include <feature.h>

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef int8_t int_fast8_t;
typedef int32_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef signed long long intmax_t;
typedef signed long intptr_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint8_t uint_fast8_t;
typedef uint32_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef unsigned long long uintmax_t;
typedef unsigned long uintptr_t;

#define INT8_MAX (0x7F)
#define INT16_MAX (0x7FFF)
#define INT32_MAX (0x7FFFFFFF)
#define INT64_MAX (0x7FFFFFFFFFFFFFFF)

#define INT8_MIN (-1 - INT8_MAX)
#define INT16_MIN (-1 - INT16_MAX)
#define INT32_MIN (-1L - INT32_MAX)
#define INT64_MIN (-1LL - INT64_MAX)

#define UINT8_MAX (0xFF)
#define UINT16_MAX (0xFFFF)
#define UINT32_MAX (0xFFFFFFFFU)
#define UINT64_MAX (0xFFFFFFFFFFFFFFFFULL)

#define INT_FAST8_MIN (INT8_MIN)
#define INT_FAST16_MIN (INT32_MIN)
#define INT_FAST32_MIN (INT32_MIN)
#define INT_FAST64_MIN (INT64_MIN)

#define INT_FAST8_MAX (INT8_MAX)
#define INT_FAST16_MAX (INT32_MAX)
#define INT_FAST32_MAX (INT32_MAX)
#define INT_FAST64_MAX (INT64_MAX)

#define UINT_FAST8_MAX (UINT8_MAX)
#define UINT_FAST16_MAX (UINT32_MAX)
#define UINT_FAST32_MAX (UINT32_MAX)
#define UINT_FAST64_MAX (UINT64_MAX)

#define INT_LEAST8_MIN (INT8_MIN)
#define INT_LEAST16_MIN (INT16_MIN)
#define INT_LEAST32_MIN (INT32_MIN)
#define INT_LEAST64_MIN (INT64_MIN)

#define INT_LEAST8_MAX (INT8_MAX)
#define INT_LEAST16_MAX (INT16_MAX)
#define INT_LEAST32_MAX (INT32_MAX)
#define INT_LEAST64_MAX (INT64_MAX)

#define UINT_LEAST8_MAX (UINT8_MAX)
#define UINT_LEAST16_MAX (UINT16_MAX)
#define UINT_LEAST32_MAX (UINT32_MAX)
#define UINT_LEAST64_MAX (UINT64_MAX)

#define INTMAX_MIN (INT64_MIN)
#define INTMAX_MAX (INT64_MAX)
#define UINTMAX_MAX (UINT64_MAX)

#define INTPTR_MIN (INT32_MIN)
#define INTPTR_MAX (INT32_MAX)
#define UINTPTR_MAX (UINT32_MAX)

#define PTRDIFF_MIN (INT32_MIN)
#define PTRDIFF_MAX (INT32_MAX)
#define SIZE_MAX (UINT32_MAX)

#define SIG_ATOMIC_MAX (INT32_MAX)
#define SIG_ATOMIC_MIN (INT32_MIN)

#define INT8_C(X) (X)
#define INT16_C(X) (X)
#define INT32_C(X) (X)
#define INT64_C(X) (X##LL)

#define UINT8_C(X) (X)
#define UINT16_C(X) (X)
#define UINT32_C(X) (X##U)
#define UINT64_C(X) (X##ULL)

#define INTMAX_C(X) (X##LL)
#define UINTMAX_C(X) (X##ULL)

#endif