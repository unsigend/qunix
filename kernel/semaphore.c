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
#include <kernel/semaphore.h>

void semaphore_init(semaphore_t *sem, int count)
{
    sem->count = count;
    wait_queue_init(&sem->wq);
}

void semaphore_wait(semaphore_t *sem)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    while (sem->count == 0)
        wait_queue_sleep(&sem->wq);
    sem->count--;
    cpu_restore_interrupts(flags);
}

void semaphore_post(semaphore_t *sem)
{
    unsigned long flags;

    flags = cpu_save_interrupts();
    sem->count++;
    wait_queue_wakeup(&sem->wq);
    cpu_restore_interrupts(flags);
}