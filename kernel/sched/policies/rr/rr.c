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

#include "rr.h"

#include <kernel/list.h>
#include <kernel/task.h>

#define RR_SLICE 10

static void enqueue(struct task *task);
static void dequeue(struct task *task);
static struct task *pick_next(void);
static bool tick(void);

static struct list_head ready_queue = LIST_HEAD_INIT(ready_queue);

const struct sched_class rr_class = {
    .name = "Round Robin",
    .enqueue = enqueue,
    .dequeue = dequeue,
    .pick_next = pick_next,
    .tick = tick,
    .slice = RR_SLICE,
};

static void enqueue(struct task *task)
{
    list_add_tail(&ready_queue, &task->node);
}

static void dequeue(struct task *task)
{
    struct task *cur;

    if (list_empty(&ready_queue))
        return;

    list_for_each_entry(&ready_queue, cur, struct task, node)
    {
        if (cur == task) {
            list_del(&cur->node);
            break;
        }
    }
}

static struct task *pick_next(void)
{
    struct list_head *node;

    if (list_empty(&ready_queue))
        return NULL;

    node = list_del_head(&ready_queue);
    return list_entry(node, struct task, node);
}

static bool tick(void)
{
    struct task *cur;

    cur = task_current();
    cur->slice--;
    if (cur->slice == 0) {
        cur->slice = RR_SLICE;
        return true;
    }

    return false;
}