#include <kernel/kmalloc.h>
#include <kernel/ktest.h>
#include <kernel/mm/pmm.h>
#include <kernel/proc.h>
#include <kernel/string.h>

KTEST_SUITE(proc);

static struct proc *make_empty_zombie(void)
{
    struct proc *proc;
    pagetable_t pt;

    proc = kcalloc(1, sizeof(*proc));
    if (!proc)
        return NULL;

    pt = pagetable_create();
    if (!pt) {
        kfree(proc);
        return NULL;
    }

    proc->pid = pid_alloc();
    proc->name = "ktest-proc";
    proc->pagetable = pt;
    proc->state = PROC_ZOMBIE;
    list_init(&proc->children);
    list_init(&proc->siblings);
    list_init(&proc->node);

    return proc;
}

KTEST_CASE(proc, empty_create_destroy)
{
    struct pmm_stats before, after;
    struct proc *proc;
    pid_t pid;

    pmm_stats(&before);

    proc = make_empty_zombie();
    ASSERT_NOT_NULL(proc);
    ASSERT_NE_INT(proc->pid, 0);
    ASSERT_TRUE(proc->pagetable);

    pid = proc->pid;
    proc_destroy(proc);

    ASSERT_NULL(proc_find(pid));

    pmm_stats(&after);
    ASSERT_EQ_UINT(after.free_pages, before.free_pages);
}
