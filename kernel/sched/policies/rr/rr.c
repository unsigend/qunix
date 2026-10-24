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

#include <kernel/compiler.h>
#include <kernel/list.h>
#include <kernel/task.h>

#define RR_SLICE 10

static void enqueue(struct sched_rq *rq, struct task *task);
static void dequeue(struct sched_rq *rq, struct task *task);
static struct task *pick_next(struct sched_rq *rq);
static bool tick(struct sched_rq *rq);

const struct sched_class rr_class = {
    .name = "Round Robin",
    .enqueue = enqueue,
    .dequeue = dequeue,
    .pick_next = pick_next,
    .tick = tick,
    .slice = RR_SLICE,
};

static void enqueue(struct sched_rq *rq, struct task *task)
{
    list_add_tail(&rq->ready_queue, &task->node);
    rq->nr_running++;
}

static void dequeue(struct sched_rq *rq, struct task *task)
{
    struct task *cur;

    if (list_empty(&rq->ready_queue))
        return;

    list_for_each_entry(&rq->ready_queue, cur, struct task, node)
    {
        if (cur == task) {
            list_del(&cur->node);
            rq->nr_running--;
            break;
        }
    }
}

static struct task *pick_next(struct sched_rq *rq)
{
    struct list_head *node;

    if (list_empty(&rq->ready_queue))
        return NULL;

    node = list_del_head(&rq->ready_queue);
    rq->nr_running--;
    return list_entry(node, struct task, node);
}

static bool tick(struct sched_rq *rq)
{
    struct task *cur;
    UNUSED(rq);

    cur = task_current();
    cur->slice--;
    if (cur->slice == 0) {
        cur->slice = RR_SLICE;
        return true;
    }

    return false;
}