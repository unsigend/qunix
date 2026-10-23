#include <kernel/ktest.h>
#include <kernel/sched.h>
#include <kernel/semaphore.h>
#include <kernel/task.h>

KTEST_SUITE(sem);

#define NBLOCKERS 4

KTEST_CASE(sem, uncontended)
{
    struct semaphore sem;

    semaphore_init(&sem, NBLOCKERS);

    for (int i = 0; i < NBLOCKERS; i++)
        semaphore_wait(&sem);

    for (int i = 0; i < NBLOCKERS; i++)
        semaphore_post(&sem);

    ASSERT_TRUE(1);
}

struct sem_block {
    struct semaphore *sem;
    int blocked;
};

static void sem_block(void *arg)
{
    struct sem_block *ctx = arg;

    ctx->blocked = 1;
    semaphore_wait(ctx->sem);
    ctx->blocked = 0;
}

KTEST_CASE(sem, block)
{
    struct semaphore sem;
    struct sem_block ctx[NBLOCKERS];
    struct task *tasks[NBLOCKERS];

    semaphore_init(&sem, 0);

    for (int i = 0; i < NBLOCKERS; i++) {
        ctx[i].sem = &sem;
        ctx[i].blocked = 0;
        tasks[i] = task_create(sem_block, &ctx[i], "sem-block");
        ASSERT_NOT_NULL(tasks[i]);
    }

    for (int i = 0; i < NBLOCKERS; i++) {
        while (ctx[i].blocked == 0)
            sched_yield();
    }

    for (int i = 0; i < NBLOCKERS; i++)
        ASSERT_EQ_INT(ctx[i].blocked, 1);

    for (int i = 0; i < NBLOCKERS; i++)
        semaphore_post(&sem);

    for (int i = 0; i < NBLOCKERS; i++)
        ASSERT_EQ_INT(task_join(tasks[i]), 0);

    for (int i = 0; i < NBLOCKERS; i++)
        ASSERT_EQ_INT(ctx[i].blocked, 0);
}