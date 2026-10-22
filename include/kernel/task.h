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

enum task_state {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_ZOMBIE,
};

struct task {
    pid_t pid;
    const char *name;
    enum task_state state;
    uintptr_t kstack;             /* stack top (start address ) */
    struct task_context *context; /* context struct store on the kernel stack */
    struct list_head node;
    unsigned int slice;
    int exit;
    struct completion completion;
    atomic_t joined;
};

/*
 * task_current - the task running on this CPU.
 *
 * Valid once scheduling has started.
 *
 * Returns the current task.
 */
extern struct task *task_current(void);

/*
 * task_create - create a kernel thread that runs @func(@arg).
 *
 * The thread is left READY. Its first switch enables interrupts and calls
 * @func. If @func returns, the thread exits with status 0. @name must remain
 * valid for the life of the task.
 *
 * Returns the task, or NULL if the task or its stack cannot be allocated.
 */
extern struct task *task_create(void (*func)(void *), void *arg,
                                const char *name);

/*
 * task_exit - terminate the current task with @status.
 *
 * Marks the task TASK_ZOMBIE and schedules away. The joiner frees the stack
 * and the task. Does not return.
 */
extern void task_exit(int status) __noreturn;

/*
 * task_join - wait until @task is a zombie, then free it.
 *
 * May sleep. Must not be called from interrupt context or while preemption
 * is disabled. @task is freed before this returns.
 *
 * Returns @task's exit status.
 */
extern int task_join(struct task *task);

/*
 * task_context_switch - switch from @saved to @restored.
 *
 * Called by schedule() only. @saved's context is stored and @restored's
 * is loaded. Returns in @restored, as if its own switch had returned.
 */
CONTRACT extern void task_context_switch(struct task *saved,
                                         struct task *restored);

/*
 * task_context_init - build @task's initial kernel stack.
 *
 * The stack is already allocated. The first switch to @task enters @func(@arg)
 * with interrupts enabled. If @func returns, the trampoline calls task_exit(0).
 */
CONTRACT extern void task_context_init(struct task *task, void (*func)(void *),
                                       void *arg);
#endif /* _QUNIX_TASK_H_ */