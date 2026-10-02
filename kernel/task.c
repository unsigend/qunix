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
#include <kernel/spinlock.h>
#include <kernel/task.h>

#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

#define TIME_SLICE 10 /* 10ms */

static struct task_struct *cur_task;
static struct list_head ready_queue;
static pid_t nextpid;
static int need_resched;
static spinlock_t lock;

void task_init(void)
{
    nextpid = 1;

    /* setup boot task */
    cur_task = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    if (!cur_task)
        panic("Failed to allocate memory for boot task");

    cur_task->pid = 0;
    cur_task->sliceleft = TIME_SLICE;
    cur_task->state = TASK_RUNNING;
    cur_task->kstack = task_boot_kstack();
    cur_task->context = NULL;

    list_init(&ready_queue);
    spinlock_init(&lock);

    LOGM(LOG_LEVEL_INFO, "TASK", "Task system initialized successfully");
}

struct task_struct *task_get_current(void) { return cur_task; }

static pid_t get_nextpid(void)
{
    if (nextpid >= NTASKS)
        return -1;

    return nextpid++;
}

struct task_struct *task_create(void (*func)(void *), void *data)
{
    struct task_struct *task;
    struct pmm_page *phy_page;

    spinlock_lock(&lock);

    task = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    if (!task) {
        spinlock_unlock(&lock);
        return NULL;
    }

    phy_page = pmm_alloc_page();
    if (!phy_page) {
        kfree(task);
        spinlock_unlock(&lock);
        return NULL;
    }

    task->kstack =
        (char *)(pmm_page_to_virt(phy_page) + PAGE_SIZE); /* stack bottom */

    task->pid = get_nextpid();
    if (task->pid < 0) {
        kfree(task);
        pmm_free_page(phy_page);

        spinlock_unlock(&lock);
        return NULL;
    }

    task->context = task_context_init(task->kstack, func, data);
    task->state = TASK_READY;

    list_add_tail(&task->tasks, &ready_queue);

    spinlock_unlock(&lock);

    return task;
}

/* A simple Round-Robin scheduler based on a FIFO queue with a time slice */
void task_schedule(void)
{
    struct task_struct *old_task;
    struct task_struct *new_task;
    unsigned long flags;

    flags =
        cpu_save_interrupts(); /* save interrupts flags into per-task's stack
                                  and restore it back after context switch */

    if (list_empty(&ready_queue)) {
        if (cur_task->state == TASK_BLOCKED)
            panic("No task to schedule");
        cpu_restore_interrupts(flags);
        return;
    }

    old_task = cur_task;

    if (old_task->state == TASK_RUNNING) {
        old_task->state = TASK_READY;
        task_enqueue(old_task);
    }

    new_task = task_dequeue();
    new_task->state = TASK_RUNNING;
    new_task->sliceleft = TIME_SLICE;

    if (old_task == new_task) {
        cpu_restore_interrupts(flags);
        return;
    }

    cur_task = new_task;

    context_switch(&old_task->context, new_task->context);

    cpu_restore_interrupts(flags);
}

void task_enqueue(struct task_struct *task)
{
    spinlock_lock(&lock);
    list_add_tail(&task->tasks, &ready_queue);
    spinlock_unlock(&lock);
}

struct task_struct *task_dequeue(void)
{
    struct task_struct *task;

    spinlock_lock(&lock);

    if (list_empty(&ready_queue)) {
        spinlock_unlock(&lock);
        return NULL;
    }

    task = list_entry(ready_queue.next, struct task_struct, tasks);
    list_del_head(&ready_queue);

    spinlock_unlock(&lock);

    return task;
}

void task_tick(void)
{
    if (list_empty(&ready_queue)) {
        cur_task->sliceleft = TIME_SLICE;
        return;
    }

    cur_task->sliceleft--;
    if (!cur_task->sliceleft)
        need_resched = 1;
}

void task_preempt(void)
{
    if (!need_resched)
        return;

    need_resched = 0;
    task_schedule();
}