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

#ifndef _QUNIX_KTEST_CORE_H_
#define _QUNIX_KTEST_CORE_H_

#include <kernel/compiler.h>
#include <kernel/list.h>
#include <kernel/stddef.h>
#include <kernel/types.h>

enum ktest_status {
    KTEST_PASSED,
    KTEST_FAILED,
};

struct ktest_suite {
    pid_t pid;
    const char *name;
    size_t ncases;
    struct list_head cases;
    size_t npassed;
    size_t nfailed;
} __aligned(sizeof(void *));

struct ktest_case {
    const char *name;
    void (*func)(struct ktest_case *self);
    enum ktest_status status;
    struct list_head node;
    struct ktest_suite *suite;
} __aligned(sizeof(void *));

CONTRACT extern struct ktest_suite *ktest_suites(void);
CONTRACT extern size_t ktest_nsuites(void);

CONTRACT extern struct ktest_case *ktest_cases(void);
CONTRACT extern size_t ktest_ncases(void);

extern void ktest_failed(struct ktest_case *self, const char *fmt, ...)
    __printf(2, 3);

CONTRACT extern void ktest_isa_exit(int status);

#define __KTEST_PREFIX __ktest_
#define __KTEST_SUITE_SECTION ".ktest_suites"
#define __KTEST_CASE_SECTION ".ktest_cases"

/*
 * Naming conventions for ktest structs and functions
 *
 * suite: __ktest_s_<suite_name>
 * case:  __ktest_c_<suite_name>_<case_name>
 * func:  __ktest_f_<suite_name>_<case_name>
 */

#define __KTEST_SUITE_NAME(sname) CONCAT3(__KTEST_PREFIX, s_, sname)
#define __KTEST_CASE_NAME(sname, cname)                                        \
    CONCAT5(__KTEST_PREFIX, c_, sname, _, cname)
#define __KTEST_FUNC_NAME(sname, cname)                                        \
    CONCAT5(__KTEST_PREFIX, f_, sname, _, cname)

#undef KTEST_SUITE
#undef KTEST_CASE

#define KTEST_SUITE(sname)                                                     \
    struct ktest_suite __KTEST_SUITE_NAME(sname)                               \
        __used __section(__KTEST_SUITE_SECTION) = {                            \
            .pid = 0,                                                          \
            .name = #sname,                                                    \
            .ncases = 0,                                                       \
            .cases = LIST_HEAD_INIT(__KTEST_SUITE_NAME(sname).cases),          \
            .npassed = 0,                                                      \
            .nfailed = 0,                                                      \
    };

#define KTEST_CASE(sname, cname)                                               \
    static void __KTEST_FUNC_NAME(sname, cname)(struct ktest_case * self);     \
    extern struct ktest_suite __KTEST_SUITE_NAME(sname);                       \
    static struct ktest_case __KTEST_CASE_NAME(sname, cname)                   \
        __used __section(__KTEST_CASE_SECTION) = {                             \
            .name = #cname,                                                    \
            .func = __KTEST_FUNC_NAME(sname, cname),                           \
            .status = KTEST_PASSED,                                            \
            .node = LIST_HEAD_INIT(__KTEST_CASE_NAME(sname, cname).node),      \
            .suite = &__KTEST_SUITE_NAME(sname),                               \
    };                                                                         \
    static void __KTEST_FUNC_NAME(sname, cname)(struct ktest_case * self)

#endif /* _QUNIX_KTEST_CORE_H_ */