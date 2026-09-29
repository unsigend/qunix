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
#include <kernel/irq.h>
#include <kernel/kmalloc.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/task.h>

#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

struct task_struct *task_current;

struct task_node {
    struct task_struct *task;
    struct task_node *next;
};

struct task_list /* a FIFO queue for tasks */
{
    struct task_node *head;
    struct task_node *tail;
};

static pid_t nextpid;
static struct task_list ready_queue;
static struct task_node *current_node;
int need_resched;

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

    current_node = (struct task_node *)kmalloc(sizeof(struct task_node));
    if (!current_node)
        panic("Failed to allocate memory for current task node");

    current_node->task = task_current;
    current_node->next = NULL;

    LOGM(LOG_LEVEL_INFO, "TASK", "Task system initialized successfully");
}

static void enqueue(struct task_node *node)
{
    node->next = NULL;

    if (!ready_queue.head) {
        ready_queue.head = node;
        ready_queue.tail = node;
    } else {
        ready_queue.tail->next = node;
        ready_queue.tail = node;
    }
}

static struct task_node *dequeue(void)
{
    struct task_node *node;

    if (!ready_queue.head)
        return NULL;

    node = ready_queue.head;
    ready_queue.head = node->next;
    node->next = NULL;

    if (ready_queue.tail == node)
        ready_queue.tail = NULL;

    return node;
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
    struct task_node *node;

    task = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    if (!task)
        return NULL;

    phy_page = pmm_alloc_page();
    if (!phy_page) {
        kfree(task);
        return NULL;
    }

    task->kstack =
        (char *)(pmm_page_to_virt(phy_page) + PAGE_SIZE); /* stack bottom */

    task->pid = get_nextpid();
    if (task->pid < 0) {
        kfree(task);
        pmm_free_page(phy_page);
        return NULL;
    }

    task->context = task_context_init(task->kstack, func, data);
    task->state = TASK_READY;

    node = (struct task_node *)kmalloc(sizeof(struct task_node));
    if (!node) {
        kfree(task);
        pmm_free_page(phy_page);
        return NULL;
    }

    node->task = task;
    node->next = NULL;
    enqueue(node);

    return task;
}

/* A simple Round-Robin scheduler based on a FIFO queue with a time slice */
void task_schedule(void)
{
    struct task_node *new_node;
    struct task_node *old_node;

    if (!ready_queue.head)
        return;

    old_node = current_node;
    old_node->task->state = TASK_READY;
    enqueue(old_node);

    new_node = dequeue();
    new_node->task->state = TASK_RUNNING;
    task_current = new_node->task;
    current_node = new_node;

    context_switch(&old_node->task->context, task_current->context);
}

void task_tick(void)
{
    if (!task_current || !current_node) /* Guard against that the interrupt
                                           triggers before call task_init */
        return;

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