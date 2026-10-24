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

#ifndef _QUNIX_WAITQ_H_
#define _QUNIX_WAITQ_H_

#include <kernel/list.h>
#include <kernel/spinlock.h>

struct wait_queue {
    struct list_head waiter;
};

typedef struct wait_queue wait_queue_t;

#define WAITQ_INIT(name) {.waiter = LIST_HEAD_INIT((name).waiter)}

extern void waitq_init(wait_queue_t *wq);
extern void waitq_sleep(wait_queue_t *wq, struct spinlock *lock);
extern void waitq_sleep_irq(wait_queue_t *wq, struct spinlock *lock,
                            unsigned long *irqflags);
extern void waitq_wakeup(wait_queue_t *wq);
extern void waitq_wakeup_all(wait_queue_t *wq);

#endif /* _QUNIX_WAITQ_H_ */