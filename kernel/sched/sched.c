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

#include "policies/rr/rr.h"

#include <asm/task.h>

#include <kernel/cpu.h>
#include <kernel/interrupt.h>
#include <kernel/kmalloc.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>
#include <kernel/preempt.h>
#include <kernel/printk.h>
#include <kernel/sched.h>
#include <kernel/string.h>
#include <kernel/task.h>

#define ROOT_TASK_PID 0
#define IDLE_TASK_PID 1

#define ROOT_SLICE 10
#define IDLE_SLICE 1

extern char kernel_stack_top[]; /* the stack top of the root task */

struct sched {
    struct {
        struct task *current;
        struct task *idle;
    } tasks;
    const struct sched_class *policy;
    volatile unsigned int need_resched;
};

static struct sched sched_core;

struct task *task_current(void) { return sched_core.tasks.current; }
bool need_resched(void) { return sched_core.need_resched; }

static void sched_check(void)
{
    if (!preemptible())
        panic("sched: preempt is disabled");
    if (in_interrupt())
        panic("sched: in interrupt");
}

static void do_idle(void *arg)
{
    UNUSED(arg);
    cpu_idle();
}

void sched_init(void)
{
    struct pmm_page *page;
    struct task *root, *idle;
    virt_addr_t stack_va;

    /* initialize the root task */
    root = kmalloc(sizeof(struct task));
    if (!root)
        panic("sched: failed to allocate memory for root task");

    root->pid = ROOT_TASK_PID;
    root->name = "root";
    root->state = TASK_RUNNING;
    root->kstack = (uintptr_t)kernel_stack_top;
    root->context = kmalloc(sizeof(struct task_context));
    if (!root->context)
        panic("sched: failed to allocate memory for root task context");
    list_init(&root->node);
    root->slice = ROOT_SLICE;
    root->exit = 0;

    /* initialize the idle task */
    idle = kmalloc(sizeof(struct task));
    if (!idle)
        panic("sched: failed to allocate memory for idle task");
    page = pmm_alloc_page();
    if (!page)
        panic("sched: failed to allocate page for idle task stack");
    stack_va = pmm_page_to_va(page);
    memset((void *)stack_va, 0, PAGE_SIZE);

    idle->pid = IDLE_TASK_PID;
    idle->name = "idle";
    idle->state = TASK_RUNNING; /* always running */
    idle->kstack = (uintptr_t)stack_va;
    task_context_init(idle, do_idle, NULL);
    list_init(&idle->node);
    idle->slice = IDLE_SLICE;
    idle->exit = 0;

    sched_core.tasks.current = root;
    sched_core.tasks.idle = idle;
    sched_core.need_resched = 0;
    sched_core.policy = &rr_class;

    LOGM_INFO("SCHED", "Scheduler initialized successfully");
}

void schedule(void)
{
    struct task *cur, *next;

    sched_check();

    cur = task_current();

    if (cur->state == TASK_RUNNING &&
        cur != sched_core.tasks.idle) /* only running tasks will be put back to
                                         the ready queue */
    {
        cur->state = TASK_READY;
        sched_core.policy->enqueue(cur);
    }

    sched_core.need_resched = 0;

    next = sched_core.policy->pick_next();
    if (!next)
        next = sched_core.tasks.idle;

    next->state = TASK_RUNNING;
    sched_core.tasks.current = next;
    task_context_switch(cur, next);
}

void sched_yield(void)
{
    struct task *cur, *next;

    sched_check();

    cur = task_current();

    if (cur == sched_core.tasks.idle)
        return;

    cur->state = TASK_READY;
    sched_core.policy->enqueue(cur);

    next = sched_core.policy->pick_next();
    if (next == cur)
        cur->state = TASK_RUNNING;
    else {
        next->state = TASK_RUNNING;
        sched_core.tasks.current = next;
        task_context_switch(cur, next);
    }
}

void sched_tick(void)
{
    if (task_current() == sched_core.tasks.idle) {
        sched_core.need_resched = 1;
        return;
    }

    if (sched_core.policy->tick())
        sched_core.need_resched = 1;
}