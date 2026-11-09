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

#ifndef _QUNIX_PROC_H_
#define _QUNIX_PROC_H_

#include <kernel/list.h>
#include <kernel/mm/page.h>
#include <kernel/types.h>

struct task;

enum proc_state {
    PROC_RUNNING,
    PROC_ZOMBIE,
};

struct proc {
    pid_t pid;
    const char *name;
    struct proc *parent;
    struct list_head children;
    struct list_head siblings;
    pagetable_t pagetable;
    int exit_status;
    enum proc_state state;
    struct task *task;
    struct list_head node;
    unsigned int trace;
};

extern pid_t pid_alloc(void);

extern struct proc *proc_find(pid_t pid);
extern void proc_destroy(struct proc *proc);
extern void proc_exit(int status) __noreturn;
extern void proc_init(void);

#endif /* _QUNIX_PROC_H_ */