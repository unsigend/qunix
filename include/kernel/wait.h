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

#ifndef _QUNIX_WAIT_H_
#define _QUNIX_WAIT_H_

#include <kernel/list.h>
#include <kernel/spinlock.h>

struct wait_queue {
    struct list_head tasks;
    spinlock_t lock;
};

void wait_queue_init(struct wait_queue *wq);
void wait_queue_sleep(struct wait_queue *wq);
void wait_queue_wakeup(struct wait_queue *wq);
void wait_queue_wakeup_all(struct wait_queue *wq);

#endif /* _QUNIX_WAIT_H_ */