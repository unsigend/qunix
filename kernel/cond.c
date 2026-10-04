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
#include <kernel/cpu.h>
#include <kernel/panic.h>
#include <kernel/task.h>

void cond_init(cond_t *cond) { wait_queue_init(&cond->wq); }

void cond_wait(cond_t *cond, mutex_t *mutex)
{
    unsigned long flags;

    if (mutex->locked && mutex->holder != task_get_current())
        panic("wait not hold by current task");
    flags = cpu_save_interrupts();
    mutex_unlock(mutex);
    wait_queue_sleep(&cond->wq);
    cpu_restore_interrupts(flags);
    mutex_lock(mutex);
}

void cond_signal(cond_t *cond) { wait_queue_wakeup(&cond->wq); }
void cond_broadcast(cond_t *cond) { wait_queue_wakeup_all(&cond->wq); }