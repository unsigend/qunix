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

#ifndef _QUNIX_MUTEX_H_
#define _QUNIX_MUTEX_H_

#include <kernel/waitq.h>

#define MUTEX_STR "mutex"

struct mutex {
    wait_queue_t waitq;
    struct spinlock lock;
    struct task *owner;
};

typedef struct mutex mutex_t;

#define MUTEX_INIT(name)                                                       \
    {.waitq = WAITQ_INIT(name.waitq),                                          \
     .lock = SPINLOCK_INIT(MUTEX_STR),                                         \
     .owner = NULL}

extern void mutex_init(mutex_t *mutex);
extern bool mutex_is_locked(mutex_t *mutex);
extern bool mutex_trylock(mutex_t *mutex);
extern void mutex_lock(mutex_t *mutex);
extern void mutex_unlock(mutex_t *mutex);

#endif /* _QUNIX_MUTEX_H_ */