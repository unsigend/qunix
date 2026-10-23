#include <kernel/completion.h>
#include <kernel/ktest.h>
#include <kernel/sched.h>
#include <kernel/task.h>

KTEST_SUITE(completion);

struct completion_ctx {
    struct completion *c;
    int started;
};

static void completion_waiter(void *arg)
{
    struct completion_ctx *ctx = arg;

    ctx->started = 1;
    wait_for_completion(ctx->c);
}

KTEST_CASE(completion, complete)
{
    struct completion c;
    struct task *t;
    struct completion_ctx ctx;

    completion_init(&c);

    ctx.c = &c;
    ctx.started = 0;

    t = task_create(completion_waiter, &ctx, "completion-waiter");
    ASSERT_NOT_NULL(t);

    while (!ctx.started)
        sched_yield();

    complete(&c);

    ASSERT_EQ_INT(task_join(t), 0);
}

#define NWAITERS 3

KTEST_CASE(completion, complete_all)
{
    struct completion c;
    struct completion_ctx ctx[NWAITERS];
    struct task *tasks[NWAITERS];

    completion_init(&c);

    for (int i = 0; i < NWAITERS; i++) {
        ctx[i].c = &c;
        ctx[i].started = 0;
        tasks[i] = task_create(completion_waiter, &ctx[i], "completion-waiter");
        ASSERT_NOT_NULL(tasks[i]);
    }

    for (int i = 0; i < NWAITERS; i++) {
        while (!ctx[i].started)
            sched_yield();
    }

    complete_all(&c);

    for (int i = 0; i < NWAITERS; i++)
        ASSERT_EQ_INT(task_join(tasks[i]), 0);

    wait_for_completion(&c);
}