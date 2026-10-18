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

#include <asm/task.h>

#include <kernel/interrupt.h>
#include <kernel/panic.h>
#include <kernel/preempt.h>
#include <kernel/sched.h>
#include <kernel/task.h>

struct sched {
    struct {
        struct task *current;
        struct task *idle;
    } tasks;
    struct sched_class *policy;
    volatile unsigned int need_resched;
};

static struct sched sched_core;

struct task *task_current(void) { return sched_core.tasks.current; }

void sched_init(void) { /* TODO */ }

void schedule(void) { /* TODO */ }

void sched_yield(void) { /* TODO */ }

void sched_tick(void) { /* TODO */ }