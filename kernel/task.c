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

#include <kernel/cpu.h>
#include <kernel/kmalloc.h>
#include <kernel/mm/pmm.h>
#include <kernel/panic.h>
#include <kernel/preempt.h>
#include <kernel/sched.h>
#include <kernel/task.h>

static pid_t next_tid = 2; /* improve later, use a better TID allocation method
                              to track the next available TID */
static struct spinlock tid_lock = SPINLOCK_INIT("tid");

/*
 * task_create - create a kernel thread that runs @func(@arg).
 *
 * The thread is left READY. Its first switch enables interrupts and calls
 * @func. If @func returns, the thread exits with status 0. @name must remain
 * valid for the life of the task.
 *
 * Returns the task, or NULL if the task or its stack cannot be allocated.
 */
struct task *task_create(void (*func)(void *), void *arg, const char *name)
{
    struct task *task;
    struct pmm_page *page;

    task = kmalloc(sizeof(struct task));
    if (!task)
        return NULL;

    page = pmm_alloc_page();
    if (!page) {
        kfree(task);
        return NULL;
    }

    spinlock_lock(&tid_lock);
    task->tid = next_tid++;
    spinlock_unlock(&tid_lock);

    task->name = name;
    task->state = TASK_READY;
    task->kstack = (uintptr_t)pmm_page_to_va(page);
    task_context_init(task, func, arg);
    list_init(&task->node);
    task->exit_status = 0;
    completion_init(&task->completion);
    atomic_store(&task->joined, 0);
    task->fpu_state = NULL;

    if (task_fpu_init(task)) {
        kfree(task);
        pmm_free_page(page);
        return NULL;
    }
    task->proc = NULL;

    sched_enqueue(task);

    return task;
}

/*
 * task_exit - terminate the current task with @status.
 *
 * Marks the task TASK_ZOMBIE and schedules away. The joiner frees the stack
 * and the task. Does not return.
 */
void task_exit(int status)
{
    struct task *task;

    preempt_disable();
    task = task_current();
    task->state = TASK_ZOMBIE;
    task->exit_status = status;

    complete_all(&task->completion);
    preempt_enable();
    schedule();

    cpu_halt();
}

/*
 * task_join - wait until @task is a zombie, then free it.
 *
 * May sleep. Must not be called from interrupt context or while preemption
 * is disabled. @task is freed before this returns.
 *
 * Returns @task's exit status.
 */
int task_join(struct task *task)
{
    int status;
    struct pmm_page *page;

    if (!task)
        panic("task_join: bad task");
    if (task == task_current())
        panic("task_join: self join");
    if (task == cpu_current()->task_idle)
        panic("task_join: join idle task");

    wait_for_completion(&task->completion);
    status = task->exit_status;

    if (atomic_xchg(&task->joined, 1) == 1)
        return status;
    page = pmm_va_to_page((virt_addr_t)task->kstack);
    task_fpu_free(task);
    pmm_free_page(page);
    kfree(task);

    return status;
}

struct task *task_current(void) { return cpu_current()->task_cur; }