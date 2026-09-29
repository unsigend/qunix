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

#include <kernel/compiler.h>
#include <kernel/list.h>
#include <kernel/types.h>

enum task_state {
    TASK_READY,   /* ready to run */
    TASK_RUNNING, /* running task */
};

#define NTASKS 64 /* maximum number of tasks */

CONTRACT struct task_context; /* task context */

/* The functions below will be either dropped or hidden in implementation
 * details for next version */
CONTRACT extern void context_switch(struct task_context **saved,
                                    struct task_context *restored);
CONTRACT extern struct task_context *
task_context_init(char *kstack, void (*func)(void *), void *data);
CONTRACT extern char *task_boot_kstack(void);
extern void task_tick(void);
extern void task_preempt(void);

/* In v0.1 the task struct is only used for kernel threads, no user space
 * tasks related */
struct task_struct {
    pid_t pid;
    enum task_state state;
    struct task_context *context;
    char *kstack; /* kernel stack bottom address */
    struct list_head node;
};

extern void task_init(void);

/* Create a new kernel thread without executing or scheduling it, and return a
 * pointer to it which allocate by kmalloc. Return NULL if failed. */
extern struct task_struct *task_create(void (*func)(void *), void *data);

extern void task_schedule(void);

#endif /* _QUNIX_TASK_H_ */