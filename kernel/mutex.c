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
#include <kernel/mutex.h>
#include <kernel/panic.h>
#include <kernel/task.h>

void mutex_init(mutex_t *mutex)
{
    mutex->locked = 0;
    mutex->holder = NULL;
    wait_queue_init(&mutex->wq);
}

void mutex_lock(mutex_t *mutex)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    while (mutex->locked)
        wait_queue_sleep(&mutex->wq);
    mutex->locked = 1;
    mutex->holder = task_get_current();
    cpu_restore_interrupts(flags);
}

void mutex_unlock(mutex_t *mutex)
{
    unsigned long flags;

    flags = cpu_save_interrupts();

    if (!mutex->locked)
        panic("mutex not locked");
    if (mutex->holder != task_get_current())
        panic("deadlock detected");

    mutex->locked = 0;
    mutex->holder = NULL;
    wait_queue_wakeup(&mutex->wq);
    cpu_restore_interrupts(flags);
}