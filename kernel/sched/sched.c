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
    struct spinlock lock;
    const struct sched_class *policy;
};

static struct sched sched_core;

static inline void sched_core_init(void)
{
    spinlock_init(&sched_core.lock, "sched_core");
    sched_core.policy = &rr_class;
}

struct task *task_current(void) { return cpu_current()->task_cur; }
bool need_resched(void) { return cpu_current()->need_resched; }

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
    completion_init(&root->completion);
    atomic_store(&root->joined, 0);

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
    completion_init(&idle->completion);
    atomic_store(&idle->joined, 0);

    /* initialize the scheduler core */
    sched_core_init();

    cpu_current()->task_cur = root;
    cpu_current()->task_idle = idle;

    LOGM_INFO("SCHED", "Scheduler initialized successfully");
}

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
        sched_core.policy->enqueue(cur);
    }

    cpu_current()->need_resched = 0;

    next = sched_core.policy->pick_next();
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

void sched_yield(void)
{
    struct task *cur, *next;
    unsigned long irqflags;

    sched_check();

    cur = task_current();

    if (cur == cpu_current()->task_idle)
        return;

    irqflags = spinlock_lock_irqsave(&sched_core.lock);

    cur->state = TASK_READY;
    sched_core.policy->enqueue(cur);

    next = sched_core.policy->pick_next();
    if (next == cur) {
        cur->state = TASK_RUNNING;
        spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
    } else {
        next->state = TASK_RUNNING;
        cpu_current()->task_cur = next;

        cpu_current()->in_sched = true;
        spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
        task_context_switch(cur, next);
        cpu_current()->in_sched = false;
    }
}

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
    if (sched_core.policy->tick())
        cpu_current()->need_resched = 1;
    spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
}

void sched_enqueue(struct task *task)
{
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&sched_core.lock);
    task->slice = sched_core.policy->slice;
    sched_core.policy->enqueue(task);
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
    sched_core.policy->enqueue(task);
    spinlock_unlock_irqrestore(&sched_core.lock, irqflags);
}