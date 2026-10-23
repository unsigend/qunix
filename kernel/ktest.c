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
#include <kernel/ktest.h>
#include <kernel/list.h>
#include <kernel/printk.h>
#include <kernel/sprintf.h>
#include <kernel/stdarg.h>
#include <kernel/string.h>
#include <kernel/task.h>

#if CONFIG_TEST

struct ktest_summary {
    size_t nspassed;
    size_t nsfailed;
    size_t nstotal;
    size_t ncpassed;
    size_t ncfailed;
    size_t nctotal;
};

static void print_summary(struct ktest_summary *summary)
{
    printk("\n[KTEST ] Summary:\n");
    printk("[CASES ] failed: %zu/%zu, passed %zu/%zu\n", summary->ncfailed,
           summary->nctotal, summary->ncpassed, summary->nctotal);
    printk("[SUITES] failed: %zu/%zu, passed %zu/%zu\n", summary->nsfailed,
           summary->nstotal, summary->nspassed, summary->nstotal);
}

static void run_one(struct ktest_suite *s)
{
    UNUSED(s);

    struct task *task;
    struct ktest_case *c;

    task = task_current();
    s->pid = task->pid;
    s->npassed = 0;
    s->nfailed = 0;

    printk("[RUNNING] Test suite: %s\n", s->name);

    list_for_each_entry(&s->cases, c, struct ktest_case, node)
    {
        c->status = KTEST_PASSED;
        c->func(c);
        if (c->status == KTEST_PASSED)
            s->npassed++;
        else
            s->nfailed++;
    }

    if (s->nfailed)
        printk("[FAILED ] Test suite: %s\n", s->name);
    else
        printk("[PASSED ] Test suite: %s\n", s->name);
}

void ktest_failed(struct ktest_case *self, const char *fmt, ...)
{
    va_list args;
    char buf[128];

    self->status = KTEST_FAILED;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    printk("  [FAILED ] Test case: %s\n", self->name);
    printk("  %s\n", buf);
}

void ktest_init(void)
{
    struct ktest_case *c;
    struct ktest_suite *s;

    for (size_t i = 0; i < ktest_ncases(); i++) {
        c = &ktest_cases()[i];
        s = c->suite;

        list_add_tail(&s->cases, &c->node);
        s->ncases++;
    }

    printk("[KTEST ] Kernel test framework initialized\n");
    printk("[KTEST ] %zu test cases, %zu test suites\n", ktest_ncases(),
           ktest_nsuites());
    printk("[KTEST ] KTest framework is ready to run\n\n");
}

void ktest_run_all(void)
{
    struct ktest_suite *s;
    struct ktest_summary summary;

    memset(&summary, 0, sizeof(summary));

    for (size_t i = 0; i < ktest_nsuites(); i++) {
        s = &ktest_suites()[i];
        run_one(s);

        summary.nstotal++;
        if (s->nfailed)
            summary.nsfailed++;
        else
            summary.nspassed++;

        summary.ncpassed += s->npassed;
        summary.ncfailed += s->nfailed;
        summary.nctotal += s->ncases;
    }

    print_summary(&summary);
    ktest_isa_exit(summary.nsfailed ? 1 : 0);
}

void ktest_run_all_parallel(unsigned int nthreads)
{
#if CONFIG_SMP
#error "SMP support is not implemented"
#else
    UNUSED(nthreads);
    ktest_run_all();
#endif /* CONFIG_SMP */
}

void ktest_run_suite(const char *name)
{
    struct ktest_suite *s;

    for (size_t i = 0; i < ktest_nsuites(); i++) {
        s = &ktest_suites()[i];
        if (strcmp(s->name, name) == 0) {
            run_one(s);
            break;
        }
    }
}

#else

void ktest_init(void) {}
void ktest_run_all(void) {}
void ktest_run_all_parallel(unsigned int nthreads) { UNUSED(nthreads); }
void ktest_run_suite(const char *name) { UNUSED(name); }

#endif /* CONFIG_TEST */