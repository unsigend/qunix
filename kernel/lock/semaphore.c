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

#include <kernel/semaphore.h>

void semaphore_init(semaphore_t *sem, unsigned int val)
{
    spinlock_init(&sem->lock, SEMAPHORE_STR);
    sem->val = val;
    waitq_init(&sem->waitq);
}

void semaphore_wait(semaphore_t *sem)
{
    spinlock_lock(&sem->lock);

    while (sem->val == 0)
        waitq_sleep(&sem->waitq, &sem->lock);

    sem->val--;

    spinlock_unlock(&sem->lock);
}

void semaphore_post(semaphore_t *sem)
{
    spinlock_lock(&sem->lock);

    sem->val++;

    waitq_wakeup(&sem->waitq);

    spinlock_unlock(&sem->lock);
}