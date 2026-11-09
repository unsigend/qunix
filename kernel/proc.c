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

#include <kernel/kmalloc.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/proc.h>
#include <kernel/spinlock.h>
#include <kernel/task.h>
#include <kernel/waitq.h>

#define START_PID 1

static struct spinlock pid_lock = SPINLOCK_INIT("pid");
static pid_t next_pid = START_PID;

static struct list_head proc_list = LIST_HEAD_INIT(proc_list);
static struct spinlock proc_lock = SPINLOCK_INIT("proc");
static struct wait_queue proc_waitq = WAITQ_INIT(proc_waitq);

pid_t pid_alloc(void)
{
    pid_t pid;
    spinlock_lock(&pid_lock);
    pid = next_pid++;
    spinlock_unlock(&pid_lock);
    return pid;
}

struct proc *proc_find(pid_t pid)
{
    struct proc *proc;

    spinlock_lock(&proc_lock);
    list_for_each_entry(&proc_list, proc, struct proc, node)
    {
        if (proc->pid == pid) {
            spinlock_unlock(&proc_lock);
            return proc;
        }
    }
    spinlock_unlock(&proc_lock);
    return NULL;
}

/*
 * proc_destroy - free a zombie process.
 *
 * Called from the reaper, not from the dying thread. Joins the main
 * task if one exists (it must already have exited). Unlinks from the
 * global list, switches to the kernel pagetable so this table is not
 * loaded, then frees the pagetable and the proc.
 *
 * Panics if @proc is NULL or not PROC_ZOMBIE.
 */
void proc_destroy(struct proc *proc)
{
    if (!proc)
        panic("proc: bad proc");
    if (proc->state != PROC_ZOMBIE)
        panic("proc: proc is not zombie");

    if (proc->task)
        task_join(proc->task);

    spinlock_lock(&proc_lock);
    list_del(&proc->node);
    spinlock_unlock(&proc_lock);

    pagetable_switch_kernel();
    pagetable_free(proc->pagetable);
    kfree(proc);
}

/*
 * proc_exit - kill the current user process.
 *
 * Stores @status, marks the process PROC_ZOMBIE, and wakes the reaper
 * under proc_lock so a scan+sleep cannot miss the zombie. Then
 * task_exit. Does not call proc_destroy (the reaper does). Kernel
 * threads must not call this.
 *
 * Does not return.
 */
void proc_exit(int status)
{
    struct proc *proc;

    if (task_is_kernel(task_current()))
        panic("proc: cannot exit kernel task");

    proc = task_current()->proc;

    spinlock_lock(&proc_lock);
    proc->exit_status = status;
    proc->state = PROC_ZOMBIE;
    waitq_wakeup(&proc_waitq);
    spinlock_unlock(&proc_lock);

    task_exit(status);
}

static void proc_reaper(void *arg)
{
    UNUSED(arg);
    int found = 0;
    struct proc *proc;

    while (1) {
        spinlock_lock(&proc_lock);
        list_for_each_entry(&proc_list, proc, struct proc, node)
        {
            if (proc->state == PROC_ZOMBIE) {
                found = 1;
                break;
            }
        }
        if (!found)
            waitq_sleep(&proc_waitq, &proc_lock);
        spinlock_unlock(&proc_lock);

        if (found) {
            printk("proc: reaping process %d status %d\n", proc->pid,
                   proc->exit_status);
            proc_destroy(proc);
            found = 0;
        }
    }
}

void proc_init(void)
{
    struct task *task;

    task = task_create(proc_reaper, NULL, "proc_reaper");
    if (!task)
        panic("proc: failed to create proc reaper");

    LOGM_INFO("PROC", "Process module initialized successfully");
}