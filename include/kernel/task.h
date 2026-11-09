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

#ifndef _QUNIX_TASK_H_
#define _QUNIX_TASK_H_

#include <kernel/atomic.h>
#include <kernel/completion.h>
#include <kernel/list.h>

struct task_context;
struct proc;

enum task_state {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_ZOMBIE,
};

struct task {
    pid_t tid;
    const char *name;
    enum task_state state;
    uintptr_t kstack;
    struct task_context *context;
    struct list_head node;
    unsigned int slice;
    int exit_status;
    struct completion completion;
    atomic_t joined;
    void *fpu_state;
    struct proc *proc;
};

static inline int task_is_kernel(struct task *task);
static inline int task_is_user(struct task *task);
extern struct task *task_current(void);
extern struct task *task_create(void (*func)(void *), void *arg,
                                const char *name);
extern void task_exit(int status) __noreturn;
extern int task_join(struct task *task);
extern void task_context_switch(struct task *saved, struct task *restored);
extern void task_context_init(struct task *task, void (*func)(void *),
                              void *arg);

extern int task_fpu_init(struct task *task);
extern void task_fpu_free(struct task *task);

static inline int task_is_kernel(struct task *task)
{
    return task->proc == NULL;
}
static inline int task_is_user(struct task *task)
{
    return !task_is_kernel(task);
}

#endif /* _QUNIX_TASK_H_ */