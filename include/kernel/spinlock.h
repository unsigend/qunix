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

#ifndef _QUNIX_SPINLOCK_H_
#define _QUNIX_SPINLOCK_H_

#include <kernel/task.h>

typedef struct spinlock {
    unsigned int locked;
    struct task_struct *holder;
} spinlock_t;

#define SPINLOCK_INIT {.locked = 0, .holder = NULL}

extern void spinlock_init(spinlock_t *lock);
extern void spinlock_lock(spinlock_t *lock);
extern void spinlock_unlock(spinlock_t *lock);

#endif /* _QUNIX_SPINLOCK_H_ */