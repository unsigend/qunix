#include <kernel/ktest.h>
#include <kernel/sched.h>
#include <kernel/spinlock.h>
#include <kernel/task.h>
#include <kernel/waitq.h>

KTEST_SUITE(waitq);

struct waitq_ctx {
    struct wait_queue *wq;
    struct spinlock *lock;
    int slept;
};

#define NSLEEPERS 3

static void waitq_sleeper(void *arg)
{
    struct waitq_ctx *ctx = arg;

    ctx->slept = 1;
    spinlock_lock(ctx->lock);
    waitq_sleep(ctx->wq, ctx->lock);
    spinlock_unlock(ctx->lock);
    ctx->slept = 0;
}

KTEST_CASE(waitq, wakeup)
{
    struct wait_queue wq;
    struct spinlock lock;
    struct waitq_ctx ctx;
    struct task *t;

    waitq_init(&wq);
    spinlock_init(&lock, "waitq-lock");
    ctx.wq = &wq;
    ctx.lock = &lock;
    ctx.slept = 0;

    t = task_create(waitq_sleeper, &ctx, "waitq-sleeper");
    ASSERT_NOT_NULL(t);

    sched_yield();

    ASSERT_FALSE(spinlock_is_locked(&lock));
    ASSERT_EQ_INT(ctx.slept, 1);

    waitq_wakeup(&wq);

    ASSERT_EQ_INT(task_join(t), 0);

    ASSERT_FALSE(spinlock_is_locked(&lock));
    ASSERT_EQ_INT(ctx.slept, 0);
}

KTEST_CASE(waitq, wakeup_all)
{
    struct wait_queue wq;
    struct spinlock lock;
    struct waitq_ctx ctx[NSLEEPERS];
    struct task *tasks[NSLEEPERS];

    waitq_init(&wq);
    spinlock_init(&lock, "waitq-lock");

    for (int i = 0; i < NSLEEPERS; i++) {
        ctx[i].wq = &wq;
        ctx[i].lock = &lock;
        ctx[i].slept = 0;
        tasks[i] = task_create(waitq_sleeper, &ctx[i], "waitq-sleeper");
        ASSERT_NOT_NULL(tasks[i]);
    }

    for (int i = 0; i < NSLEEPERS; i++) {
        while (ctx[i].slept == 0)
            sched_yield();
    }

    for (int i = 0; i < NSLEEPERS; i++)
        ASSERT_EQ_INT(ctx[i].slept, 1);
    ASSERT_FALSE(spinlock_is_locked(&lock));

    waitq_wakeup_all(&wq);

    for (int i = 0; i < NSLEEPERS; i++)
        ASSERT_EQ_INT(task_join(tasks[i]), 0);

    for (int i = 0; i < NSLEEPERS; i++)
        ASSERT_EQ_INT(ctx[i].slept, 0);

    ASSERT_FALSE(spinlock_is_locked(&lock));
}