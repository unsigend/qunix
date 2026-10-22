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

#ifndef _QUNIX_SEMAPHORE_H_
#define _QUNIX_SEMAPHORE_H_

#include <kernel/waitq.h>

#define SEMAPHORE_STR "semaphore"

struct semaphore {
    wait_queue_t waitq;
    unsigned int val;
    struct spinlock lock;
};

typedef struct semaphore semaphore_t;

#define SEMAPHORE_INIT(name, n)                                                \
    {.waitq = WAITQ_INIT(name.waitq),                                          \
     .val = n,                                                                 \
     .lock = SPINLOCK_INIT(SEMAPHORE_STR)}

extern void semaphore_init(semaphore_t *sem, unsigned int val);
extern void semaphore_wait(semaphore_t *sem);
extern void semaphore_post(semaphore_t *sem);

#define semaphore_up(sem) semaphore_post(sem)
#define semaphore_down(sem) semaphore_wait(sem)

#endif /* _QUNIX_SEMAPHORE_H_ */