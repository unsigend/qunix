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

#include <kernel/mutex.h>
#include <kernel/panic.h>
#include <kernel/task.h>

void mutex_init(mutex_t *mutex)
{
    spinlock_init(&mutex->lock, MUTEX_STR);
    waitq_init(&mutex->waitq);
    mutex->owner = NULL;
}

bool mutex_is_locked(mutex_t *mutex)
{
    bool locked;
    spinlock_lock(&mutex->lock);
    locked = mutex->owner != NULL;
    spinlock_unlock(&mutex->lock);
    return locked;
}

bool mutex_trylock(mutex_t *mutex)
{
    spinlock_lock(&mutex->lock);

    if (mutex->owner) {
        spinlock_unlock(&mutex->lock);
        return false;
    }

    mutex->owner = task_current();
    spinlock_unlock(&mutex->lock);
    return true;
}

void mutex_lock(mutex_t *mutex)
{
    struct task *cur;

    spinlock_lock(&mutex->lock);
    cur = task_current();

    if (mutex->owner == cur)
        panic("mutex: deadlock detected");

    while (mutex->owner)
        waitq_sleep(&mutex->waitq, &mutex->lock);

    mutex->owner = cur;
    spinlock_unlock(&mutex->lock);
}

void mutex_unlock(mutex_t *mutex)
{
    struct task *cur;

    spinlock_lock(&mutex->lock);
    cur = task_current();

    if (!mutex->owner)
        panic("mutex: unbalanced unlock");
    if (mutex->owner != cur)
        panic("mutex: not owner");

    mutex->owner = NULL;
    waitq_wakeup(&mutex->waitq);
    spinlock_unlock(&mutex->lock);
}