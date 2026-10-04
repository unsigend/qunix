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
#include <kernel/rwlock.h>
#include <kernel/task.h>

void rwlock_init(rwlock_t *lock)
{
    lock->readers = 0;
    lock->writers_waiting = 0;
    lock->writer = NULL;
    wait_queue_init(&lock->wq_readers);
    wait_queue_init(&lock->wq_writers);
}

void rwlock_read_lock(rwlock_t *lock)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    while (lock->writer || lock->writers_waiting > 0)
        wait_queue_sleep(&lock->wq_readers);
    lock->readers++;
    cpu_restore_interrupts(flags);
}

void rwlock_read_unlock(rwlock_t *lock)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    lock->readers--;
    if (lock->readers == 0)
        wait_queue_wakeup(&lock->wq_writers);
    cpu_restore_interrupts(flags);
}

void rwlock_write_lock(rwlock_t *lock)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    lock->writers_waiting++;
    while (lock->readers > 0 || lock->writer)
        wait_queue_sleep(&lock->wq_writers);
    lock->writers_waiting--;
    lock->writer = task_get_current();
    cpu_restore_interrupts(flags);
}

void rwlock_write_unlock(rwlock_t *lock)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    if (lock->writer != task_get_current())
        panic("rwlock_write_unlock: not the writer");
    lock->writer = NULL;
    wait_queue_wakeup_all(&lock->wq_readers);
    wait_queue_wakeup(&lock->wq_writers);
    cpu_restore_interrupts(flags);
}