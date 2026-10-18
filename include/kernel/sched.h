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

#ifndef _QUNIX_SCHED_H_
#define _QUNIX_SCHED_H_

#include <kernel/stdbool.h>

struct task;

struct sched_class {
    const char *name;

    void (*dequeue)(struct task *task);
    void (*enqueue)(struct task *task);
    struct task *(*pick_next)(void);
    bool (*tick)(void); /* tick, return true if need resched */
};

extern bool need_resched(void);
extern void sched_init(void);
extern void schedule(void);
extern void sched_yield(void);
extern void sched_tick(void);

#endif /* _QUNIX_SCHED_H_ */