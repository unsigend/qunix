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

#include <kernel/panic.h>
#include <kernel/rwlock.h>
#include <kernel/task.h>

void rwlock_init(rwlock_t *rwlock)
{
    spinlock_init(&rwlock->lock, RWLOCK_STR);
    waitq_init(&rwlock->readers);
    waitq_init(&rwlock->writers);
    rwlock->nreaders = 0;
    rwlock->nwriters = 0;
    rwlock->writer = NULL;
}

void rwlock_read_lock(rwlock_t *rwlock)
{
    spinlock_lock(&rwlock->lock);

    /* if there is a writer hold the lock or writers are waiting, the reader
     * must wait */
    while (rwlock->writer || rwlock->nwriters > 0)
        waitq_sleep(&rwlock->readers, &rwlock->lock);

    rwlock->nreaders++;

    spinlock_unlock(&rwlock->lock);
}

void rwlock_read_unlock(rwlock_t *rwlock)
{
    spinlock_lock(&rwlock->lock);

    if (rwlock->nreaders == 0)
        panic("rwlock: unbalance read unlock");

    rwlock->nreaders--;

    if (rwlock->nreaders == 0)
        waitq_wakeup(&rwlock->writers);

    spinlock_unlock(&rwlock->lock);
}

void rwlock_write_lock(rwlock_t *rwlock)
{
    struct task *cur;

    spinlock_lock(&rwlock->lock);

    cur = task_current();
    if (cur == rwlock->writer)
        panic("rwlock: deadlock detected");

    rwlock->nwriters++;
    while (rwlock->writer || rwlock->nreaders > 0)
        waitq_sleep(&rwlock->writers, &rwlock->lock);
    rwlock->nwriters--;

    rwlock->writer = cur;

    spinlock_unlock(&rwlock->lock);
}

void rwlock_write_unlock(rwlock_t *rwlock)
{
    struct task *cur;

    spinlock_lock(&rwlock->lock);

    cur = task_current();

    if (!rwlock->writer)
        panic("rwlock: unbalance write unlock");
    if (cur != rwlock->writer)
        panic("rwlock: not the writer");

    rwlock->writer = NULL;

    /* if there are writes and readers waiting, always wake up writes first to
     * avoid writer starvation */
    if (rwlock->nwriters == 0)
        waitq_wakeup_all(&rwlock->readers);
    else
        waitq_wakeup(&rwlock->writers);

    spinlock_unlock(&rwlock->lock);
}