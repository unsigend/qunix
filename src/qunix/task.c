/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/kernel.h>
#include <qunix/log.h>
#include <qunix/mm/page.h>
#include <qunix/mm/pmm.h>
#include <qunix/task.h>
#include <sys/types.h>

static pid_t nextpid;

void task_init(void)
{
    nextpid = 1; /* next pid always start from 1 since 0 is reserved for the
                    init process */

    /* TODO: Set the current init task (boot task) to the task list */

    LOGM(LOG_LEVEL_INFO, "TASK", "Task system initialized successfully");
}

static pid_t get_nextpid(void)
{
    /* TODO: add lock here in later version */

    /* TODO: optimize this by circular check available pid */
    if (nextpid >= NTASKS)
        return -1;

    return nextpid++;
}

struct task_struct *task_create(void (*func)(void *), void *data)
{
    struct task_struct *task;
    struct pmm_page *phy_page;

    task = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    if (!task)
        return NULL;

    phy_page = pmm_alloc_page();
    if (!phy_page) {
        kfree(task);
        return NULL;
    }

    task->kstack =
        (char *)(pmm_page_to_virt(phy_page) + PAGE_SIZE); /* stack bottom */

    task->pid = get_nextpid();
    if (task->pid < 0) {
        kfree(task);
        pmm_free_page(phy_page);
        return NULL;
    }

    task->context = task_context_init(task->kstack, func, data);
    task->state = TASK_READY;

    return task;
}