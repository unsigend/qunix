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

#include <kernel/cond.h>
#include <kernel/sched.h>
#include <kernel/task.h>

void cond_init(cond_t *cond)
{
    spinlock_init(&cond->lock, COND_STR);
    waitq_init(&cond->waitq);
}

void cond_wait(cond_t *cond, mutex_t *mutex)
{
    spinlock_lock(&cond->lock);
    mutex_unlock(mutex);
    waitq_sleep(&cond->waitq, &cond->lock);
    spinlock_unlock(&cond->lock);

    mutex_lock(mutex);
}

void cond_signal(cond_t *cond)
{
    spinlock_lock(&cond->lock);
    waitq_wakeup(&cond->waitq);
    spinlock_unlock(&cond->lock);
}

void cond_broadcast(cond_t *cond)
{
    spinlock_lock(&cond->lock);
    waitq_wakeup_all(&cond->waitq);
    spinlock_unlock(&cond->lock);
}