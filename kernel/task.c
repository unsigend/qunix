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

#include <kernel/clock.h>
#include <kernel/cpu.h>
#include <kernel/irq.h>
#include <kernel/kmalloc.h>
#include <kernel/list.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/task.h>

#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

static struct task_struct *task_current;
static struct list_head ready_queue;
static pid_t nextpid;
static int need_resched;

void task_init(void)
{
    nextpid = 1; /* next pid always start from 1 since 0 is reserved for the
                    init process */

    /* setup current boot running task */
    task_current = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    if (!task_current)
        panic("Failed to allocate memory for boot task");

    task_current->pid = 0;
    task_current->state = TASK_RUNNING;
    task_current->kstack = task_boot_kstack();
    task_current->context = NULL;

    list_init(&ready_queue);

    LOGM(LOG_LEVEL_INFO, "TASK", "Task system initialized successfully");
}

static pid_t get_nextpid(void)
{
    /* TODO: add lock here in later version */

    /* TODO: optimize this by circular check available pid */
    if (nextpid >= NTASKS)
        return -1;

    return nextpid++;
}

struct task_struct *task_create(void (*func)(void *), void *data)
{
    struct task_struct *task;
    struct pmm_page *phy_page;
    unsigned long flags;

    flags = cpu_save_interrupts();

    task = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    if (!task) {
        cpu_restore_interrupts(flags);
        return NULL;
    }

    phy_page = pmm_alloc_page();
    if (!phy_page) {
        kfree(task);
        cpu_restore_interrupts(flags);
        return NULL;
    }

    task->kstack =
        (char *)(pmm_page_to_virt(phy_page) + PAGE_SIZE); /* stack bottom */

    task->pid = get_nextpid();
    if (task->pid < 0) {
        kfree(task);
        pmm_free_page(phy_page);

        cpu_restore_interrupts(flags);
        return NULL;
    }

    task->context = task_context_init(task->kstack, func, data);
    task->state = TASK_READY;

    list_add_tail(&task->node, &ready_queue);

    cpu_restore_interrupts(flags);

    return task;
}

/* A simple Round-Robin scheduler based on a FIFO queue with a time slice */
void task_schedule(void)
{
    struct task_struct *old_task;
    struct task_struct *new_task;
    unsigned long flags;

    flags = cpu_save_interrupts();

    if (list_empty(&ready_queue)) {
        cpu_restore_interrupts(flags);
        return;
    }

    old_task = task_current;
    new_task = list_entry(ready_queue.next, struct task_struct, node);

    new_task->state = TASK_RUNNING;
    old_task->state = TASK_READY;

    list_add_tail(&old_task->node, &ready_queue);
    list_del(&new_task->node); /* remove new task from ready queue */

    task_current = new_task;

    context_switch(&old_task->context, new_task->context);

    cpu_restore_interrupts(flags);
}

void task_tick(void)
{
    if (jiffies % 10 == 0) /* time slice is 10 ticks, namely 10ms */
        need_resched = 1;
}

void task_preempt(void)
{
    if (!need_resched)
        return;

    need_resched = 0;
    task_schedule();
}