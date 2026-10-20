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
#include <kernel/sched.h>
#include <kernel/task.h>

static pid_t next_pid = 2; /* improve later, use a better PID allocation method
                              to track the next available PID */

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

    task->pid = next_pid++;
    task->name = name;
    task->state = TASK_READY;
    task->kstack = (uintptr_t)pmm_page_to_va(page);
    task_context_init(task, func, arg);
    list_init(&task->node);
    task->exit = 0;
    completion_init(&task->completion);

    sched_enqueue(task);

    return task;
}

void task_exit(int status)
{
    struct task *task;

    task = task_current();
    task->state = TASK_ZOMBIE;
    task->exit = status;

    complete_all(&task->completion);
    schedule();

    cpu_halt();
}

int task_join(struct task *task)
{
    int status;
    struct pmm_page *page;

    wait_for_completion(&task->completion);
    status = task->exit;
    page = pmm_va_to_page((virt_addr_t)task->kstack);
    pmm_free_page(page);
    kfree(task);

    return status;
}