#include <kernel/cdev.h>
#include <kernel/errno.h>
#include <kernel/ktest.h>

KTEST_SUITE(cdev);

static const struct cdev_ops dummy_ops;

KTEST_CASE(cdev, register_lookup)
{
    static struct cdev a = {.major = 240, .minor = 0, .ops = &dummy_ops};
    static struct cdev b = {.major = 240, .minor = 1, .ops = &dummy_ops};

    ASSERT_NULL(cdev_get(240, 0));
    ASSERT_EQ_INT(cdev_register(&a), 0);
    ASSERT_EQ_PTR(cdev_get(240, 0), &a);

    ASSERT_EQ_INT(cdev_register(&b), 0);
    ASSERT_EQ_PTR(cdev_get(240, 1), &b);
    ASSERT_EQ_PTR(cdev_get(240, 0), &a);
    ASSERT_NULL(cdev_get(240, 2));
}

KTEST_CASE(cdev, duplicate)
{
    static struct cdev a = {.major = 241, .minor = 0, .ops = &dummy_ops};
    static struct cdev b = {.major = 241, .minor = 0, .ops = &dummy_ops};

    ASSERT_EQ_INT(cdev_register(&a), 0);
    ASSERT_EQ_INT(cdev_register(&b), -EEXIST);
    ASSERT_EQ_PTR(cdev_get(241, 0), &a);
}

KTEST_CASE(cdev, invalid)
{
    struct cdev no_ops = {.major = 242, .minor = 0, .ops = NULL};

    ASSERT_EQ_INT(cdev_register(NULL), -EINVAL);
    ASSERT_EQ_INT(cdev_register(&no_ops), -EINVAL);
    ASSERT_NULL(cdev_get(242, 0));
}
