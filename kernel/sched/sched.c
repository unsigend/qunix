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

#include <kernel/cmdline.h>
#include <kernel/cpu.h>
#include <kernel/interrupt.h>
#include <kernel/kmalloc.h>
#include <kernel/mm/memlayout.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>
#include <kernel/preempt.h>
#include <kernel/printk.h>
#include <kernel/sched.h>
#include <kernel/string.h>
#include <kernel/task.h>

#define ROOT_TASK_PID 0
#define ROOT_SLICE 10

struct sched {
    struct spinlock lock;
    const struct sched_class *policy;
};

static struct sched sched_core;

static inline void init_policy(void)
{
    const char *policy;
    const char *policy_str = RR_POLICY_STR;

    policy = cmdline_get("sched");

    if (policy) {
        if (strcmp(policy, RR_POLICY_STR) == 0) {
            sched_core.policy = &rr_class;
            policy_str = RR_POLICY_STR;
        } else
            panic("sched: invalid policy: %s", policy);
    } else /* default policy is rr */
        sched_core.policy = &rr_class;

    LOGM_INFO("SCHED", "scheduler policy: %s", policy_str);
}

static void sched_check(void)
{
    if (!preemptible())
        panic("sched: preempt is disabled");
    if (in_interrupt())
        panic("sched: in interrupt");
}

void sched_init(void)
{
    struct task *root;

    /* initialize the root task */
    root = kmalloc(sizeof(struct task));
    if (!root)
        panic("sched: failed to allocate memory for root task");

    root->pid = ROOT_TASK_PID;
    root->name = "root";
    root->state = TASK_RUNNING;
    root->kstack = (uintptr_t)boot_task_stack_top();
    root->context = kmalloc(sizeof(struct task_context));
    if (!root->context)
        panic("sched: failed to allocate memory for root task context");
    list_init(&root->node);
    root->slice = ROOT_SLICE;
    root->exit = 0;
    completion_init(&root->completion);
    atomic_store(&root->joined, 0);

    spinlock_init(&sched_core.lock, "sched_core");

    init_policy();

    /* init the cpu's scheduler data */
    cpu_current()->task_cur = root;
    cpu_current()->task_idle = root;
    list_init(&cpu_current()->rq.ready_queue);
    cpu_current()->rq.nr_running = 0;

    LOGM_INFO("SCHED", "Scheduler initialized successfully");
}

struct task *task_current(void) { return cpu_current()->task_cur; }
bool need_resched(void) { return cpu_current()->need_resched; }

void schedule(void)
{
    struct task *cur, *next;
    unsigned long irqflags;

    if (cpu_current()->in_sched)
        return;

    sched_check();

    cur = task_current();

    irqflags = spinlock_lock_irqsave(&sched_core.lock);

    if (cur->state == TASK_RUNNING &&
        cur != cpu_current()->task_idle) /* only running tasks will be put back
                                         to the ready queue */
    {
        cur->state = TASK_READY;
        sched_core.policy->enqueue(&cpu_current()->rq, cur);
    }

    cpu_current()->need_resched = 0;

    next = sched_core.policy->pick_next(&cpu_current()->rq);
    if (!next)
        next = cpu_current()->task_idle;

    next->state = TASK_RUNNING;

    cpu_current()->in_sched = true;
    spinlock_unlock_irqrestore(&sched_core.lock, irqflags);

    /* Per-CPU data, SMP safe */
    cpu_current()->task_cur = next;

    task_context_switch(cur, next);
    cpu_current()->in_sched = false;
}

void sched_yield(void) { schedule(); }

void sched_tick(void)
{
    unsigned long irqflags;

    if (cpu_current()->task_cur == cpu_current()->task_idle) {
        irqflags = spinlock_lock_irqsave(&sched_core.lock);
        cpu_current()->need_resched = 1;
        spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
        return;
    }

    irqflags = spinlock_lock_irqsave(&sched_core.lock);
    if (sched_core.policy->tick(&cpu_current()->rq))
        cpu_current()->need_resched = 1;
    spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
}

void sched_enqueue(struct task *task)
{
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&sched_core.lock);
    task->slice = sched_core.policy->slice;
    sched_core.policy->enqueue(&cpu_current()->rq, task);
    spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
}

void sched_wakeup(struct task *task)
{
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&sched_core.lock);

    if (task->state != TASK_BLOCKED) {
        spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
        return;
    }

    if (task == task_current()) {
        task->state = TASK_RUNNING;
        spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
        return;
    }

    task->state = TASK_READY;
    sched_core.policy->enqueue(&cpu_current()->rq, task);
    spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
}