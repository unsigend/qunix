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

#include <kernel/wait.h>

void wait_queue_init(struct wait_queue *wq)
{
    list_init(&wq->tasks);
    spinlock_init(&wq->lock);
}

void wait_queue_sleep(struct wait_queue *wq)
{
    struct task_struct *task;

    spinlock_lock(&wq->lock);
    task = task_get_current();
    task->state = TASK_BLOCKED;
    list_add_tail(&task->tasks, &wq->tasks);
    spinlock_unlock(&wq->lock);

    task_schedule();
}

void wait_queue_wakeup(struct wait_queue *wq)
{
    struct task_struct *task;

    spinlock_lock(&wq->lock);

    if (list_empty(&wq->tasks)) {
        spinlock_unlock(&wq->lock);
        return;
    }

    task = list_entry(wq->tasks.next, struct task_struct, tasks);
    task->state = TASK_READY;
    list_del(&task->tasks);
    task_enqueue(task);
    spinlock_unlock(&wq->lock);
}

void wait_queue_wakeup_all(struct wait_queue *wq)
{
    struct task_struct *task;

    spinlock_lock(&wq->lock);
    while (!list_empty(&wq->tasks)) {
        task = list_entry(wq->tasks.next, struct task_struct, tasks);
        task->state = TASK_READY;
        list_del(&task->tasks);
        task_enqueue(task);
    }
    spinlock_unlock(&wq->lock);
}