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

#ifndef _QUNIX_KTEST_ASSERT_H_
#define _QUNIX_KTEST_ASSERT_H_

#include <kernel/compiler.h>
#include <kernel/ktest/core.h>
#include <kernel/stddef.h>
#include <kernel/stdint.h>
#include <kernel/string.h>

#undef ASSERT_TRUE
#undef ASSERT_FALSE

#undef ASSERT_NULL
#undef ASSERT_NOT_NULL

#undef ASSERT_EQ_PTR
#undef ASSERT_NE_PTR
#undef ASSERT_GT_PTR
#undef ASSERT_GE_PTR
#undef ASSERT_LT_PTR
#undef ASSERT_LE_PTR

#undef ASSERT_EQ_INT
#undef ASSERT_NE_INT
#undef ASSERT_GT_INT
#undef ASSERT_GE_INT
#undef ASSERT_LT_INT
#undef ASSERT_LE_INT

#undef ASSERT_EQ_UINT
#undef ASSERT_NE_UINT
#undef ASSERT_GT_UINT
#undef ASSERT_GE_UINT
#undef ASSERT_LT_UINT
#undef ASSERT_LE_UINT

#undef ASSERT_EQ_STR
#undef ASSERT_NE_STR

#define __KTEST_ASSERT(cond, fmt, ...)                                         \
    do {                                                                       \
        if (!(cond)) {                                                         \
            ktest_failed(self, "%s:%d: " fmt, __FILE__, __LINE__,              \
                         ##__VA_ARGS__);                                       \
            return;                                                            \
        }                                                                      \
    } while (0)

#define __KTEST_ASSERT_CMP(ctype, ptype, actual, expected, op, fmt)            \
    do {                                                                       \
        ctype __ktest_a = (ctype)(actual);                                     \
        ctype __ktest_e = (ctype)(expected);                                   \
        __KTEST_ASSERT(__ktest_a op __ktest_e, fmt, #actual, #expected,        \
                       #actual, (ptype)__ktest_a);                             \
    } while (0)

#define ASSERT_TRUE(expr)                                                      \
    __KTEST_ASSERT((expr), "expected %s to be true but got false", #expr)
#define ASSERT_FALSE(expr)                                                     \
    __KTEST_ASSERT(!(expr), "expected %s to be false but got true", #expr)

#define ASSERT_NULL(p)                                                         \
    do {                                                                       \
        uintptr_t __ktest_p = (uintptr_t)(p);                                  \
        __KTEST_ASSERT(__ktest_p == 0, "expected %s == NULL but got %s=%p",    \
                       #p, #p, (void *)__ktest_p);                             \
    } while (0)
#define ASSERT_NOT_NULL(p)                                                     \
    do {                                                                       \
        uintptr_t __ktest_p = (uintptr_t)(p);                                  \
        __KTEST_ASSERT(__ktest_p != 0, "expected %s != NULL but got %s=NULL",  \
                       #p, #p);                                                \
    } while (0)

#define ASSERT_EQ_PTR(actual, expected)                                        \
    __KTEST_ASSERT_CMP(uintptr_t, void *, actual, expected, ==,                \
                       "expected %s == %s but got %s=%p")
#define ASSERT_NE_PTR(actual, expected)                                        \
    __KTEST_ASSERT_CMP(uintptr_t, void *, actual, expected, !=,                \
                       "expected %s != %s but got %s=%p")
#define ASSERT_GT_PTR(actual, expected)                                        \
    __KTEST_ASSERT_CMP(uintptr_t, void *, actual, expected, >,                 \
                       "expected %s > %s but got %s=%p")
#define ASSERT_GE_PTR(actual, expected)                                        \
    __KTEST_ASSERT_CMP(uintptr_t, void *, actual, expected, >=,                \
                       "expected %s >= %s but got %s=%p")
#define ASSERT_LT_PTR(actual, expected)                                        \
    __KTEST_ASSERT_CMP(uintptr_t, void *, actual, expected, <,                 \
                       "expected %s < %s but got %s=%p")
#define ASSERT_LE_PTR(actual, expected)                                        \
    __KTEST_ASSERT_CMP(uintptr_t, void *, actual, expected, <=,                \
                       "expected %s <= %s but got %s=%p")

#define ASSERT_EQ_INT(actual, expected)                                        \
    __KTEST_ASSERT_CMP(long, long, actual, expected, ==,                       \
                       "expected %s == %s but got %s=%ld")
#define ASSERT_NE_INT(actual, expected)                                        \
    __KTEST_ASSERT_CMP(long, long, actual, expected, !=,                       \
                       "expected %s != %s but got %s=%ld")
#define ASSERT_GT_INT(actual, expected)                                        \
    __KTEST_ASSERT_CMP(long, long, actual, expected, >,                        \
                       "expected %s > %s but got %s=%ld")
#define ASSERT_GE_INT(actual, expected)                                        \
    __KTEST_ASSERT_CMP(long, long, actual, expected, >=,                       \
                       "expected %s >= %s but got %s=%ld")
#define ASSERT_LT_INT(actual, expected)                                        \
    __KTEST_ASSERT_CMP(long, long, actual, expected, <,                        \
                       "expected %s < %s but got %s=%ld")
#define ASSERT_LE_INT(actual, expected)                                        \
    __KTEST_ASSERT_CMP(long, long, actual, expected, <=,                       \
                       "expected %s <= %s but got %s=%ld")

#define ASSERT_EQ_UINT(actual, expected)                                       \
    __KTEST_ASSERT_CMP(unsigned long, unsigned long, actual, expected, ==,     \
                       "expected %s == %s but got %s=%lu")
#define ASSERT_NE_UINT(actual, expected)                                       \
    __KTEST_ASSERT_CMP(unsigned long, unsigned long, actual, expected, !=,     \
                       "expected %s != %s but got %s=%lu")
#define ASSERT_GT_UINT(actual, expected)                                       \
    __KTEST_ASSERT_CMP(unsigned long, unsigned long, actual, expected, >,      \
                       "expected %s > %s but got %s=%lu")
#define ASSERT_GE_UINT(actual, expected)                                       \
    __KTEST_ASSERT_CMP(unsigned long, unsigned long, actual, expected, >=,     \
                       "expected %s >= %s but got %s=%lu")
#define ASSERT_LT_UINT(actual, expected)                                       \
    __KTEST_ASSERT_CMP(unsigned long, unsigned long, actual, expected, <,      \
                       "expected %s < %s but got %s=%lu")
#define ASSERT_LE_UINT(actual, expected)                                       \
    __KTEST_ASSERT_CMP(unsigned long, unsigned long, actual, expected, <=,     \
                       "expected %s <= %s but got %s=%lu")

#define ASSERT_EQ_STR(actual, expected)                                        \
    do {                                                                       \
        const char *__ktest_a = (actual);                                      \
        const char *__ktest_e = (expected);                                    \
        __KTEST_ASSERT(                                                        \
            (__ktest_a == __ktest_e) ||                                        \
                (__ktest_a && __ktest_e && strcmp(__ktest_a, __ktest_e) == 0), \
            "expected %s == \"%s\" but got \"%s\"", #actual,                   \
            __ktest_e ? __ktest_e : "NULL", __ktest_a ? __ktest_a : "NULL");   \
    } while (0)
#define ASSERT_NE_STR(actual, expected)                                        \
    do {                                                                       \
        const char *__ktest_a = (actual);                                      \
        const char *__ktest_e = (expected);                                    \
        __KTEST_ASSERT(                                                        \
            (__ktest_a != __ktest_e) && (!__ktest_a || !__ktest_e ||           \
                                         strcmp(__ktest_a, __ktest_e) != 0),   \
            "expected %s != \"%s\" but got \"%s\"", #actual,                   \
            __ktest_e ? __ktest_e : "NULL", __ktest_a ? __ktest_a : "NULL");   \
    } while (0)

#endif /* _QUNIX_KTEST_ASSERT_H_ */
