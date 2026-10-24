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
#include <kernel/list.h>
#include <kernel/panic.h>
#include <kernel/sched.h>
#include <kernel/spinlock.h>
#include <kernel/task.h>
#include <kernel/waitq.h>

volatile unsigned long jiffies = 0;

struct ksleep_task {
    struct task *task;
    unsigned long expires;
    struct list_head node;
    struct wait_queue wq;
};

struct ksleep_queue {
    struct list_head head;
    struct spinlock lock;
};

static struct ksleep_queue ksleep_queue = {
    .head = LIST_HEAD_INIT(ksleep_queue.head),
    .lock = SPINLOCK_INIT("ksleep_queue"),
};

void ksleep(unsigned long ticks)
{
    struct ksleep_task waiter;
    struct ksleep_task *pos;
    int insert = 0;
    unsigned long irqflags;

    if (task_current() == cpu_current()->task_idle)
        panic("ksleep: idle task cannot sleep");

    waiter.task = task_current();
    waiter.expires = jiffies + ticks;
    list_init(&waiter.node);
    waitq_init(&waiter.wq);

    irqflags = spinlock_lock_irqsave(&ksleep_queue.lock);

    list_for_each_entry(&ksleep_queue.head, pos, struct ksleep_task, node)
    {
        if ((long)(waiter.expires - pos->expires) < 0) {
            insert = 1;
            __list_add(&waiter.node, pos->node.prev, &pos->node);
            break;
        }
    }
    if (!insert)
        list_add_tail(&ksleep_queue.head, &waiter.node);

    waitq_sleep_irq(&waiter.wq, &ksleep_queue.lock, &irqflags);

    spinlock_unlock_irqrestore(&ksleep_queue.lock, irqflags);
}

static void wakeup_sleep_queue(void)
{
    struct ksleep_task *pos, *tmp;
    unsigned long irqflags;

    irqflags = spinlock_lock_irqsave(&ksleep_queue.lock);

    list_for_each_entry_safe(&ksleep_queue.head, pos, tmp, struct ksleep_task,
                             node)
    {
        if ((long)(pos->expires - jiffies) <= 0) {
            list_del(&pos->node);
            waitq_wakeup(&pos->wq);
        } else
            break;
    }

    spinlock_unlock_irqrestore(&ksleep_queue.lock, irqflags);
}

void clock_tick(void)
{
    jiffies++;
    sched_tick();
    wakeup_sleep_queue();
}