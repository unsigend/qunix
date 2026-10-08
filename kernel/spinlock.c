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

#include <kernel/interrupt.h>
#include <kernel/panic.h>
#include <kernel/spinlock.h>
#include <kernel/task.h>

/* This implementation of spinlock is designed for single core only. So no need
 * spin since if can't acquire the lock means the deadlock happens. For the
 * flags, it is safe because a spinlock is never held across a context switch.
 * So nested/flags only ever belong to one critical section at a time. */

static unsigned long
    nested; /* counts how many spinlock_lock calls on this CPU are not yet
               matched by an unlock, across every lock */
static unsigned long flags;

void spinlock_init(spinlock_t *lock)
{
    lock->locked = 0;
    lock->holder = NULL;
}

void spinlock_lock(spinlock_t *lock)
{
    if (lock->locked) /* already locked, since in single core, only forget to
                         unlock is possible. */
        panic("deadlock detected");

    if (nested == 0)
        flags = irq_save();

    lock->locked = 1;
    lock->holder = task_get_current();

    nested++;
}

void spinlock_unlock(spinlock_t *lock)
{
    if (!lock->locked)
        panic("spinlock not locked");

    if (lock->holder != task_get_current())
        panic("deadlock detected");

    if (nested == 0)
        panic("unbalanced unlock");

    nested--;

    lock->locked = 0;
    lock->holder = NULL;

    if (nested == 0)
        irq_restore(flags);
}