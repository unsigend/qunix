#include <kernel/clock.h>
#include <kernel/ktest.h>
#include <kernel/sched.h>
#include <kernel/task.h>

KTEST_SUITE(ksleep);

#define NSLEEPERS 3

struct ksleep_ctx {
    unsigned long ticks;
    unsigned long start;
    unsigned long end;
    int started;
    int done;
    int id;
    int *seq;
    int *seq_at;
};

static void wait_flag(int *flag)
{
    while (!*flag)
        sched_yield();
}

static void ksleep_worker(void *arg)
{
    struct ksleep_ctx *ctx = arg;

    ctx->started = 1;
    ctx->start = jiffies;
    ksleep(ctx->ticks);
    ctx->end = jiffies;
    if (ctx->seq && ctx->seq_at)
        ctx->seq[(*ctx->seq_at)++] = ctx->id;
    ctx->done = 1;
}

KTEST_CASE(ksleep, duration)
{
    struct ksleep_ctx ctx;
    struct task *t;

    ctx.ticks = 8;
    ctx.started = 0;
    ctx.done = 0;
    ctx.seq = NULL;
    ctx.seq_at = NULL;

    t = task_create(ksleep_worker, &ctx, "ksleep-duration");
    ASSERT_NOT_NULL(t);

    wait_flag(&ctx.started);
    wait_flag(&ctx.done);
    ASSERT_EQ_INT(task_join(t), 0);
    ASSERT_GE_UINT(ctx.end - ctx.start, ctx.ticks);
}

KTEST_CASE(ksleep, wake_order)
{
    struct ksleep_ctx ctx[NSLEEPERS];
    struct task *tasks[NSLEEPERS];
    unsigned long ticks[NSLEEPERS] = {20, 10, 5};
    int seq[NSLEEPERS];
    int seq_at;
    int i;

    seq_at = 0;
    for (i = 0; i < NSLEEPERS; i++) {
        ctx[i].ticks = ticks[i];
        ctx[i].started = 0;
        ctx[i].done = 0;
        ctx[i].id = i;
        ctx[i].seq = seq;
        ctx[i].seq_at = &seq_at;
        tasks[i] = task_create(ksleep_worker, &ctx[i], "ksleep-order");
        ASSERT_NOT_NULL(tasks[i]);
    }

    for (i = 0; i < NSLEEPERS; i++)
        wait_flag(&ctx[i].started);

    for (i = 0; i < NSLEEPERS; i++) {
        wait_flag(&ctx[i].done);
        ASSERT_EQ_INT(task_join(tasks[i]), 0);
        ASSERT_GE_UINT(ctx[i].end - ctx[i].start, ctx[i].ticks);
    }

    ASSERT_EQ_INT(seq_at, NSLEEPERS);
    ASSERT_EQ_INT(seq[0], 2);
    ASSERT_EQ_INT(seq[1], 1);
    ASSERT_EQ_INT(seq[2], 0);
}

KTEST_CASE(ksleep, many)
{
    struct ksleep_ctx ctx[NSLEEPERS];
    struct task *tasks[NSLEEPERS];
    int i;

    for (i = 0; i < NSLEEPERS; i++) {
        ctx[i].ticks = 6;
        ctx[i].started = 0;
        ctx[i].done = 0;
        ctx[i].seq = NULL;
        ctx[i].seq_at = NULL;
        tasks[i] = task_create(ksleep_worker, &ctx[i], "ksleep-many");
        ASSERT_NOT_NULL(tasks[i]);
    }

    for (i = 0; i < NSLEEPERS; i++)
        wait_flag(&ctx[i].started);

    for (i = 0; i < NSLEEPERS; i++) {
        wait_flag(&ctx[i].done);
        ASSERT_EQ_INT(task_join(tasks[i]), 0);
        ASSERT_GE_UINT(ctx[i].end - ctx[i].start, ctx[i].ticks);
    }
}
