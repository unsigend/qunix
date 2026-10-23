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

#ifndef _QUNIX_KTEST_H_
#define _QUNIX_KTEST_H_

#define KTEST_SUITE(sname)
#define KTEST_CASE(sname, cname)

#define ASSERT_TRUE(expr)
#define ASSERT_FALSE(expr)

#define ASSERT_NULL(p)
#define ASSERT_NOT_NULL(p)

#define ASSERT_EQ_PTR(actual, expected)
#define ASSERT_NE_PTR(actual, expected)
#define ASSERT_GT_PTR(actual, expected)
#define ASSERT_GE_PTR(actual, expected)
#define ASSERT_LT_PTR(actual, expected)
#define ASSERT_LE_PTR(actual, expected)

#define ASSERT_EQ_INT(actual, expected)
#define ASSERT_NE_INT(actual, expected)
#define ASSERT_GT_INT(actual, expected)
#define ASSERT_GE_INT(actual, expected)
#define ASSERT_LT_INT(actual, expected)
#define ASSERT_LE_INT(actual, expected)

#define ASSERT_EQ_UINT(actual, expected)
#define ASSERT_NE_UINT(actual, expected)
#define ASSERT_GT_UINT(actual, expected)
#define ASSERT_GE_UINT(actual, expected)
#define ASSERT_LT_UINT(actual, expected)
#define ASSERT_LE_UINT(actual, expected)

#define ASSERT_EQ_STR(actual, expected)
#define ASSERT_NE_STR(actual, expected)

extern void ktest_run_all(void);
extern void ktest_run_all_parallel(unsigned int nthreads);
extern void ktest_run_suite(const char *name);

extern void ktest_init(void);

#include <kernel/config.h>

#if CONFIG_TEST
#include <kernel/ktest/assert.h>
#include <kernel/ktest/core.h>
#endif /* CONFIG_TEST */

#endif /* _QUNIX_KTEST_H_ */