#include <kernel/cond.h>
#include <kernel/ktest.h>
#include <kernel/mutex.h>
#include <kernel/sched.h>
#include <kernel/task.h>

KTEST_SUITE(cond);

struct cond_ctx {
    struct cond *c;
    struct mutex *m;
    int flag;
    int startwait;
};

static void cond_waiter(void *arg)
{
    struct cond_ctx *ctx = arg;
    mutex_lock(ctx->m);
    ctx->startwait = 1;
    while (!ctx->flag)
        cond_wait(ctx->c, ctx->m);
    mutex_unlock(ctx->m);
}

KTEST_CASE(cond, signal)
{
    struct cond c;
    struct mutex m;
    struct cond_ctx ctx;
    struct task *t;

    cond_init(&c);
    mutex_init(&m);
    ctx.c = &c;
    ctx.m = &m;
    ctx.flag = 0;
    ctx.startwait = 0;

    t = task_create(cond_waiter, &ctx, "cond-waiter");
    ASSERT_NOT_NULL(t);

    while (!ctx.startwait)
        sched_yield();
    while (mutex_is_locked(&m))
        sched_yield();

    ASSERT_EQ_INT(ctx.flag, 0);
    ASSERT_FALSE(mutex_is_locked(&m));

    mutex_lock(&m);
    ctx.flag = 1;
    cond_signal(&c);
    mutex_unlock(&m);

    ASSERT_EQ_INT(task_join(t), 0);
}

#define NWAITERS 3

KTEST_CASE(cond, broadcast)
{
    struct cond c;
    struct mutex m;
    struct cond_ctx ctx[NWAITERS];
    struct task *tasks[NWAITERS];

    cond_init(&c);
    mutex_init(&m);

    for (int i = 0; i < NWAITERS; i++) {
        ctx[i].c = &c;
        ctx[i].m = &m;
        ctx[i].flag = 0;
        ctx[i].startwait = 0;
        tasks[i] = task_create(cond_waiter, &ctx[i], "cond-waiter");
        ASSERT_NOT_NULL(tasks[i]);
    }

    for (int i = 0; i < NWAITERS; i++) {
        while (!ctx[i].startwait)
            sched_yield();
        while (mutex_is_locked(&m))
            sched_yield();
    }

    for (int i = 0; i < NWAITERS; i++)
        ASSERT_EQ_INT(ctx[i].flag, 0);
    ASSERT_FALSE(mutex_is_locked(&m));

    mutex_lock(&m);
    for (int i = 0; i < NWAITERS; i++)
        ctx[i].flag = 1;
    cond_broadcast(&c);
    mutex_unlock(&m);

    for (int i = 0; i < NWAITERS; i++)
        ASSERT_EQ_INT(task_join(tasks[i]), 0);
}