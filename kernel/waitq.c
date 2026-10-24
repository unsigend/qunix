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

#include <kernel/sched.h>
#include <kernel/task.h>
#include <kernel/waitq.h>

void waitq_init(wait_queue_t *wq) { list_init(&wq->waiter); }

void waitq_sleep(wait_queue_t *wq, struct spinlock *lock)
{
    struct task *cur;

    cur = task_current();
    cur->state = TASK_BLOCKED;
    list_add_tail(&wq->waiter, &cur->node);

    spinlock_unlock(lock);

    schedule();

    spinlock_lock(lock);
}

void waitq_sleep_irq(wait_queue_t *wq, struct spinlock *lock,
                     unsigned long *irqflags)
{
    struct task *cur;

    cur = task_current();
    cur->state = TASK_BLOCKED;
    list_add_tail(&wq->waiter, &cur->node);

    spinlock_unlock_irqrestore(lock, *irqflags);

    schedule();

    *irqflags = spinlock_lock_irqsave(lock);
}

void waitq_wakeup(wait_queue_t *wq)
{
    struct task *task;
    struct list_head *node;

    if (list_empty(&wq->waiter))
        return;

    node = list_del_head(&wq->waiter);
    task = list_entry(node, struct task, node);
    sched_wakeup(task);
}

void waitq_wakeup_all(wait_queue_t *wq)
{
    struct list_head *node;
    struct task *task;

    while (!list_empty(&wq->waiter)) {
        node = list_del_head(&wq->waiter);
        task = list_entry(node, struct task, node);
        sched_wakeup(task);
    }
}