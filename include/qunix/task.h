/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_TASK_H
#define QUNIX_TASK_H

#include <qunix/macros.h>
#include <sys/types.h>

enum task_state {
    TASK_READY,   /* ready to run */
    TASK_RUNNING, /* running task */
};

#define NTASKS 64 /* maximum number of tasks */

CONTRACT struct task_context; /* task context */

/* Context switch between two tasks, save the context of the current task and
 * restore the context of the next task */
CONTRACT extern void context_switch(struct task_context **saved,
                                    struct task_context *restored);

/* Build the initial task context for the new task in a kernel stack */
CONTRACT extern struct task_context *
task_context_init(char *kstack, void (*func)(void *), void *data);

/* Get the address of the boot task's kernel stack address */
CONTRACT extern char *task_boot_kstack(void);

/* In v0.1 the task struct is only used for kernel threads, no user space
 * tasks related */
struct task_struct {
    pid_t pid;
    enum task_state state;
    struct task_context *context;
    char *kstack; /* kernel stack bottom address */
};

/* current running task */
extern struct task_struct *task_current;

extern void task_init(void);

/* Create a new kernel thread without executing or scheduling it, and return a
 * pointer to it which allocate by kmalloc. Return NULL if failed. */
extern struct task_struct *task_create(void (*func)(void *), void *data);

extern void task_schedule(void);
extern void task_tick(void);
extern void
task_preempt(void); /* called only in interrupt context (ASM code) */

#endif