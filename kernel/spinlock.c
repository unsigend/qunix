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

#include <kernel/cpu.h>
#include <kernel/panic.h>
#include <kernel/spinlock.h>
#include <kernel/task.h>

static unsigned long flags;
static unsigned long nested;

void spinlock_init(spinlock_t *lock)
{
    lock->locked = 0;
    lock->holder = NULL;
}

void spinlock_lock(spinlock_t *lock)
{
    struct task_struct *cur = task_get_current();

    if (lock->locked && lock->holder != cur)
        panic("spinlock already locked by another task");

    if (nested == 0)
        flags = cpu_save_interrupts();

    nested++;

    lock->locked = 1;
    lock->holder = cur;
}

void spinlock_unlock(spinlock_t *lock)
{
    struct task_struct *cur = task_get_current();

    if (lock->holder != cur)
        panic("spinlock not locked by current task");

    nested--;

    lock->locked = 0;
    lock->holder = NULL;

    if (nested == 0)
        cpu_restore_interrupts(flags);
}