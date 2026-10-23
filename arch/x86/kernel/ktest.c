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

#include <kernel/compiler.h>
#include <kernel/config.h>
#include <kernel/ktest.h>

#include <asm/io.h>

#if CONFIG_TEST

extern char __ktest_suites_start[];
extern char __ktest_suites_end[];
extern char __ktest_cases_start[];
extern char __ktest_cases_end[];

struct ktest_suite *ktest_suites(void)
{
    return (struct ktest_suite *)__ktest_suites_start;
}

size_t ktest_nsuites(void)
{
    return (size_t)((struct ktest_suite *)__ktest_suites_end -
                    (struct ktest_suite *)__ktest_suites_start);
}

struct ktest_case *ktest_cases(void)
{
    return (struct ktest_case *)__ktest_cases_start;
}

size_t ktest_ncases(void)
{
    return (size_t)((struct ktest_case *)__ktest_cases_end -
                    (struct ktest_case *)__ktest_cases_start);
}

#if CONFIG_TEST_QEMU_EXIT

void ktest_isa_exit(int status)
{
    outb(CONFIG_QEMU_DEBUG_PORT, status ? 1 : 0);
}

#else

void ktest_isa_exit(int status) { UNUSED(status); }

#endif /* CONFIG_TEST_QEMU_EXIT */

#endif /* CONFIG_TEST */
