#include <kernel/ktest.h>
#include <kernel/sched.h>
#include <kernel/stdint.h>
#include <kernel/task.h>

KTEST_SUITE(fpu);

#define FPU_ROUNDS 32

struct fpu_ctx {
    uint32_t val;
    int failed;
};

static void fpu_worker(void *arg) __attribute__((target("sse")));

static void fpu_worker(void *arg)
{
    struct fpu_ctx *ctx = arg;
    uint32_t v, x87, sse;
    int i;

    v = ctx->val;
    for (i = 0; i < FPU_ROUNDS; i++) {
        asm volatile("flds %0" : : "m"(v) : "st", "memory");
        asm volatile("movss %0, %%xmm0" : : "m"(v) : "xmm0", "memory");
        sched_yield();
        asm volatile("fstps %0" : "=m"(x87) : : "st", "memory");
        asm volatile("movss %%xmm0, %0" : "=m"(sse) : : "xmm0", "memory");
        if (x87 != v || sse != v) {
            ctx->failed = 1;
            return;
        }
    }
}

KTEST_CASE(fpu, switch_preserves_x87_sse)
{
    struct fpu_ctx a, b;
    struct task *ta, *tb;

    a.val = 0x3f800000; /* 1.0f */
    b.val = 0x40000000; /* 2.0f */
    a.failed = 0;
    b.failed = 0;

    ta = task_create(fpu_worker, &a, "fpu-a");
    tb = task_create(fpu_worker, &b, "fpu-b");
    ASSERT_NOT_NULL(ta);
    ASSERT_NOT_NULL(tb);

    ASSERT_EQ_INT(task_join(ta), 0);
    ASSERT_EQ_INT(task_join(tb), 0);
    ASSERT_EQ_INT(a.failed, 0);
    ASSERT_EQ_INT(b.failed, 0);
}
