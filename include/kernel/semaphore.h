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

#include <kernel/wait.h>

typedef struct semaphore {
    int count;
    struct wait_queue wq;
} semaphore_t;

extern void semaphore_init(semaphore_t *sem, int count);
extern void semaphore_wait(semaphore_t *sem);
extern void semaphore_post(semaphore_t *sem);

/* alias */
#define semaphore_down(sem) semaphore_wait(sem)
#define semaphore_up(sem) semaphore_post(sem)

#endif /* _QUNIX_SEMAPHORE_H_ */