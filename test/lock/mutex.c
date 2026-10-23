#include <kernel/ktest.h>
#include <kernel/mutex.h>
#include <kernel/sched.h>
#include <kernel/task.h>

KTEST_SUITE(mutex);

KTEST_CASE(mutex, uncontended)
{
    struct mutex m;

    mutex_init(&m);

    ASSERT_FALSE(mutex_is_locked(&m));
    mutex_lock(&m);
    ASSERT_TRUE(mutex_is_locked(&m));
    mutex_unlock(&m);
    ASSERT_FALSE(mutex_is_locked(&m));
}

KTEST_CASE(mutex, trylock)
{
    struct mutex m;

    mutex_init(&m);

    ASSERT_TRUE(mutex_trylock(&m));
    ASSERT_FALSE(mutex_trylock(&m));
    ASSERT_TRUE(mutex_is_locked(&m));
    mutex_unlock(&m);
    ASSERT_FALSE(mutex_is_locked(&m));
}

struct mutex_contended {
    struct mutex *m;
    int acquired;
};

static void mutex_waiter(void *arg)
{
    struct mutex_contended *ctx = arg;

    mutex_lock(ctx->m);
    ctx->acquired = 1;
    mutex_unlock(ctx->m);
}

KTEST_CASE(mutex, contended)
{
    struct mutex m;
    struct task *t;
    struct mutex_contended ctx;

    mutex_init(&m);
    ctx.m = &m;
    ctx.acquired = 0;

    ASSERT_TRUE(mutex_trylock(&m));
    ASSERT_TRUE(mutex_is_locked(&m));
    ASSERT_FALSE(ctx.acquired);

    t = task_create(mutex_waiter, &ctx, "mutex-waiter");
    ASSERT_NOT_NULL(t);

    sched_yield();

    ASSERT_FALSE(ctx.acquired);
    ASSERT_TRUE(mutex_is_locked(&m));

    mutex_unlock(&m);
    ASSERT_EQ_INT(task_join(t), 0);
    ASSERT_EQ_INT(ctx.acquired, 1);
    ASSERT_FALSE(mutex_is_locked(&m));
}