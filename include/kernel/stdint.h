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

#ifndef _QUNIX_STDINT_H_
#define _QUNIX_STDINT_H_

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef __INT_FAST8_TYPE__ int_fast8_t;
typedef __INT_FAST16_TYPE__ int_fast16_t;
typedef __INT_FAST32_TYPE__ int_fast32_t;
typedef __INT_FAST64_TYPE__ int_fast64_t;

typedef __INT_LEAST8_TYPE__ int_least8_t;
typedef __INT_LEAST16_TYPE__ int_least16_t;
typedef __INT_LEAST32_TYPE__ int_least32_t;
typedef __INT_LEAST64_TYPE__ int_least64_t;

typedef __INTMAX_TYPE__ intmax_t;
typedef __INTPTR_TYPE__ intptr_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef __UINT_FAST8_TYPE__ uint_fast8_t;
typedef __UINT_FAST16_TYPE__ uint_fast16_t;
typedef __UINT_FAST32_TYPE__ uint_fast32_t;
typedef __UINT_FAST64_TYPE__ uint_fast64_t;

typedef __UINT_LEAST8_TYPE__ uint_least8_t;
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
typedef __UINT_LEAST64_TYPE__ uint_least64_t;

typedef __UINTMAX_TYPE__ uintmax_t;
typedef __UINTPTR_TYPE__ uintptr_t;

#define INT8_MAX __SCHAR_MAX__
#define INT16_MAX __SHRT_MAX__
#define INT32_MAX __INT_MAX__
#define INT64_MAX __LONG_LONG_MAX__

#define INT8_MIN (-INT8_MAX - 1)
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MIN (-INT32_MAX - 1)
#define INT64_MIN (-INT64_MAX - 1)

#define UINT8_MAX (INT8_MAX * 2 + 1)
#define UINT16_MAX (INT16_MAX * 2 + 1)
#define UINT32_MAX (INT32_MAX * 2U + 1U)
#define UINT64_MAX (INT64_MAX * 2ULL + 1ULL)

#define INT_FAST8_MAX __INT_FAST8_MAX__
#define INT_FAST16_MAX __INT_FAST16_MAX__
#define INT_FAST32_MAX __INT_FAST32_MAX__
#define INT_FAST64_MAX __INT_FAST64_MAX__

#define INT_FAST8_MIN (-INT_FAST8_MAX - 1)
#define INT_FAST16_MIN (-INT_FAST16_MAX - 1)
#define INT_FAST32_MIN (-INT_FAST32_MAX - 1)
#define INT_FAST64_MIN (-INT_FAST64_MAX - 1)

#define UINT_FAST8_MAX __UINT_FAST8_MAX__
#define UINT_FAST16_MAX __UINT_FAST16_MAX__
#define UINT_FAST32_MAX __UINT_FAST32_MAX__
#define UINT_FAST64_MAX __UINT_FAST64_MAX__

#define INT_LEAST8_MAX __INT_LEAST8_MAX__
#define INT_LEAST16_MAX __INT_LEAST16_MAX__
#define INT_LEAST32_MAX __INT_LEAST32_MAX__
#define INT_LEAST64_MAX __INT_LEAST64_MAX__

#define INT_LEAST8_MIN (-INT_LEAST8_MAX - 1)
#define INT_LEAST16_MIN (-INT_LEAST16_MAX - 1)
#define INT_LEAST32_MIN (-INT_LEAST32_MAX - 1)
#define INT_LEAST64_MIN (-INT_LEAST64_MAX - 1)

#define UINT_LEAST8_MAX __UINT_LEAST8_MAX__
#define UINT_LEAST16_MAX __UINT_LEAST16_MAX__
#define UINT_LEAST32_MAX __UINT_LEAST32_MAX__
#define UINT_LEAST64_MAX __UINT_LEAST64_MAX__

#define INTMAX_MAX __INTMAX_MAX__
#define INTMAX_MIN (-INTMAX_MAX - 1)
#define UINTMAX_MAX __UINTMAX_MAX__

#define INTPTR_MAX __INTPTR_MAX__
#define INTPTR_MIN (-INTPTR_MAX - 1)
#define UINTPTR_MAX __UINTPTR_MAX__

#define PTRDIFF_MAX __PTRDIFF_MAX__
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1)
#define SIZE_MAX __SIZE_MAX__

#define INT8_C(x) (x)
#define INT16_C(x) (x)
#define INT32_C(x) (x)
#define INT64_C(x) (x##LL)

#define UINT8_C(x) (x)
#define UINT16_C(x) (x)
#define UINT32_C(x) (x##U)
#define UINT64_C(x) (x##ULL)

#define INTMAX_C(x) __INTMAX_C(x)
#define UINTMAX_C(x) __UINTMAX_C(x)

#endif /* _QUNIX_STDINT_H_ */
