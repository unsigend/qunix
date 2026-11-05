#include <asm/memlayout.h>
#include <asm/page.h>

#include <kernel/errno.h>
#include <kernel/ktest.h>
#include <kernel/mm/memlayout.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/string.h>

KTEST_SUITE(page);

static int kernel_half_eq(pagetable_t a, pagetable_t b)
{
    size_t i;

    for (i = pde_index(KERNEL_VIRT_START); i < PD_ENTRIES; i++) {
        if (*((pde_t *)a + i) != *((pde_t *)b + i))
            return 0;
    }
    return 1;
}

KTEST_CASE(page, user_map_copy_free)
{
    struct pmm_stats before, after;
    pagetable_t src, dest;
    struct pmm_page *p0, *p1;
    phys_addr_t pa0, pa1, got, copy0, copy1;
    virt_addr_t va0 = USER_VIRT_START + PAGE_SIZE;
    virt_addr_t va1 = USER_VIRT_START + (virt_addr_t)PT_ENTRIES * PAGE_SIZE;

    pmm_stats(&before);

    src = pagetable_create();
    ASSERT_TRUE(src);

    p0 = pmm_alloc_page();
    p1 = pmm_alloc_page();
    ASSERT_NOT_NULL(p0);
    ASSERT_NOT_NULL(p1);
    pa0 = pmm_page_to_pa(p0);
    pa1 = pmm_page_to_pa(p1);
    memset((void *)pa_to_va(pa0), 0xa5, PAGE_SIZE);
    memset((void *)pa_to_va(pa1), 0x5a, PAGE_SIZE);

    ASSERT_EQ_INT(page_map(src, pa0, va0, PAGEF_WRITE), 0);
    ASSERT_EQ_INT(page_map(src, pa1, va1, 0), 0);
    ASSERT_EQ_INT(page_lookup(src, va0, &got), 0);
    ASSERT_EQ_UINT((unsigned long)got, (unsigned long)pa0);
    ASSERT_EQ_INT(page_lookup(src, va1, &got), 0);
    ASSERT_EQ_UINT((unsigned long)got, (unsigned long)pa1);

    dest = pagetable_create();
    ASSERT_TRUE(dest);
    ASSERT_TRUE(kernel_half_eq(src, dest));

    ASSERT_EQ_INT(pagetable_copy(dest, src), 0);
    ASSERT_TRUE(kernel_half_eq(src, dest));

    ASSERT_EQ_INT(page_lookup(dest, va0, &copy0), 0);
    ASSERT_EQ_INT(page_lookup(dest, va1, &copy1), 0);
    ASSERT_NE_UINT((unsigned long)copy0, (unsigned long)pa0);
    ASSERT_NE_UINT((unsigned long)copy1, (unsigned long)pa1);
    ASSERT_EQ_INT(memcmp((void *)pa_to_va(copy0), (void *)pa_to_va(pa0),
                         PAGE_SIZE),
                  0);
    ASSERT_EQ_INT(memcmp((void *)pa_to_va(copy1), (void *)pa_to_va(pa1),
                         PAGE_SIZE),
                  0);

    ASSERT_EQ_INT(page_unmap(src, va0), 0);
    ASSERT_EQ_INT(page_unmap(src, va1), 0);
    ASSERT_EQ_INT(page_lookup(src, va0, &got), -ENOENT);

    pmm_free_page(p0);
    pmm_free_page(p1);
    pagetable_free(src);

    pmm_free_page(pmm_pa_to_page(copy0));
    pmm_free_page(pmm_pa_to_page(copy1));
    pagetable_free(dest);

    pmm_stats(&after);
    ASSERT_EQ_UINT(after.free_pages, before.free_pages);
}
