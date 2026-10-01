/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <kernel/compiler.h>
#include <kernel/kmalloc.h>
#include <kernel/macros.h>
#include <kernel/spinlock.h>
#include <kernel/stdalign.h>
#include <kernel/stdbool.h>
#include <kernel/stddef.h>
#include <kernel/stdint.h>
#include <kernel/string.h>

#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

/* kmalloc is a simple kernel memory allocator, it is based on segregated best
 * fit implementation and simplified for kernel use with pmm. the current
 * implementation is migrated from qlibc v0.1.1 which only supports manage
 * objects that less than 4KB page size. For larger objects and allocations use
 * pmm_alloc_page instead. */

#define ALIGNMENT alignof(max_align_t) /* alignment size */
#define BUCKET_COUNT 64                /* bucket count */
#define ALIGN_PAGE(size)                                                       \
    (((size) + (PAGE_SIZE - 1)) &                                              \
     ~(PAGE_SIZE - 1)) /* rounds up to the nearest multiple of PAGE_SIZE */
#define MINIMUM_BLOCKSZ                                                        \
    (sizeof(header_t) + sizeof(footer_t) +                                     \
     2 * sizeof(uintptr_t)) /* minimum threshold of the block size */
#define GET_HEADER(block) (header_t *)((char *)block)
#define GET_FOOTER(block)                                                      \
    (footer_t *)((char *)block + (block)->header.size - sizeof(footer_t))
#define IS_ALLOC(block) ((block)->header.alloc)
#define CALC_BLOCKSZ(size)                                                     \
    MAX(ALIGN(size + sizeof(header_t) + sizeof(footer_t), ALIGNMENT),          \
        ALIGN(MINIMUM_BLOCKSZ,                                                 \
              ALIGNMENT)) /* calculates the required size of the block with    \
                             meta data after alignment */
#define MAX_ALLOCSZ PAGE_SIZE /* maximum allocation size */

typedef struct meta {
    uint32_t alloc : 1;  /* 0 free, 1 allocated  */
    uint32_t unused : 2; /* unused bits */
    uint32_t size : 29;  /* whole block size, max 4KB */
} __aligned(ALIGNMENT) meta_t;

typedef meta_t header_t;
typedef meta_t footer_t;

typedef struct block {
    header_t header;         /* header */
    unsigned char payload[]; /* payload */
} block_t;

typedef struct free_block {
    header_t header;         /* header */
    struct free_block *next; /* next free block */
    struct free_block *prev; /* previous free block */
    unsigned char payload[]; /* payload */
} free_block_t;

static free_block_t *buckets[BUCKET_COUNT];
static spinlock_t lock = SPINLOCK_INIT;

/* Bucket size lookup table. Base thresholds: MINIMUM_BLOCKSZ 24 on 32-bit, 48
 * on 64-bit */
static const size_t slots[BUCKET_COUNT] = {
    /* Tiny: 8-byte increments */
    MINIMUM_BLOCKSZ, 32, 40, 48, 56, 64, 72, 80,
    /* Small: 16-byte increments */
    96, 112, 128, 144, 160, 192, 224, 256,
    /* Medium: Fibonacci-like */
    288, 320, 384, 448, 512, 576, 640, 768, 896, 1024, 1280, 1536,
    /* Large: 512-1024 byte steps */
    2048, 2560, 3072, MAX_ALLOCSZ};

static void write_meta(block_t *block, size_t size, bool alloc)
{
    footer_t *footer;

    block->header.size = size;
    block->header.alloc = alloc;
    footer = GET_FOOTER(block);
    *footer = block->header;
}

static size_t get_bucket_index(size_t size)
{
    for (size_t i = 0; i < BUCKET_COUNT; ++i)
        if (size <= slots[i])
            return i;

    return BUCKET_COUNT - 1;
}

static void insert_block(free_block_t *block, size_t index)
{
    free_block_t *freehead;

    /* Based on LIFO strategy, the new block is inserted at the head of the
     * bucket. */
    freehead = buckets[index];
    block->next = freehead;
    block->prev = NULL;
    if (freehead)
        freehead->prev = block;
    buckets[index] = block;
}

static free_block_t *remove_block(free_block_t *block, size_t index)
{
    free_block_t *prev = block->prev;
    free_block_t *next = block->next;

    if (prev)
        prev->next = next;
    else
        buckets[index] = next;
    if (next)
        next->prev = prev;

    block->next = NULL;
    block->prev = NULL;
    return block;
}

static void *slice_block(free_block_t *block, size_t size)
{
    size_t blocksz;
    size_t leftsz;
    free_block_t *remain;

    blocksz = block->header.size;
    leftsz = blocksz - size;

    /* If the left size is less than the minimum block size, return the whole
     * block, since it is not worth splitting and acceptable internal
     * fragmentation. */
    if (leftsz < MINIMUM_BLOCKSZ) {
        write_meta((block_t *)block, blocksz, true);
        return (void *)((unsigned char *)block + sizeof(header_t));
    }

    /* Otherwise, split the block into two parts: the first part is the
     * requested size, the second part is the remaining block. */
    remain = (free_block_t *)((unsigned char *)block + size);
    write_meta((block_t *)block, size, true);
    write_meta((block_t *)remain, leftsz, false);
    insert_block(remain, get_bucket_index(leftsz));

    return (void *)((unsigned char *)block + sizeof(header_t));
}

static void *bestfit(size_t size, size_t index)
{
    free_block_t *freeblk = buckets[index];
    free_block_t *bestfreeblk = NULL;
    void *p;

    while (freeblk) {
        if (freeblk->header.size >= size) {
            if (!bestfreeblk ||
                freeblk->header.size < bestfreeblk->header.size) {
                bestfreeblk = freeblk;
            }
        }
        freeblk = freeblk->next;
    }

    if (bestfreeblk) {
        remove_block(bestfreeblk, index);
        p = slice_block(bestfreeblk, size);
        return p;
    }
    return NULL;
}

static void coalescing_mid(free_block_t *prev, free_block_t *block,
                           free_block_t *next)
{
    size_t newsz;

    if (prev->header.alloc && next->header.alloc) /* no coalescing */
    {
        write_meta((block_t *)block, block->header.size, false);
        insert_block(block, get_bucket_index(block->header.size));
        return;
    }

    if (prev->header.alloc && !next->header.alloc) /* next */
    {
        newsz = block->header.size + next->header.size;
        remove_block(next, get_bucket_index(next->header.size));
        write_meta((block_t *)block, newsz, false);
        insert_block(block, get_bucket_index(newsz));
        return;
    }

    if (!prev->header.alloc && next->header.alloc) /* previous */
    {
        newsz = block->header.size + prev->header.size;
        remove_block(prev, get_bucket_index(prev->header.size));
        write_meta((block_t *)prev, newsz, false);
        insert_block(prev, get_bucket_index(newsz));
        return;
    }

    else /* previous and next */
    {
        newsz = prev->header.size + block->header.size + next->header.size;
        remove_block(next, get_bucket_index(next->header.size));
        remove_block(prev, get_bucket_index(prev->header.size));
        write_meta((block_t *)prev, newsz, false);
        insert_block(prev, get_bucket_index(newsz));
        return;
    }
}

static void coalescing(free_block_t *block)
{
    bool isfirst;
    bool islast;
    uintptr_t pagebase;
    size_t newsz;
    block_t *next, *prev;
    footer_t *prevfooter;

    /* determine if the current block is the first or last block in the page,
     * since current implementation can only manage one page for each block */
    pagebase = ROUND_DOWN((uintptr_t)block, PAGE_SIZE);
    isfirst = (uintptr_t)block == pagebase;
    islast = ((uintptr_t)block + block->header.size) == (pagebase + PAGE_SIZE);

    /* if the current block is the first block in the heap, coalescing the next
     * block is needed. */
    if (isfirst) {
        next = (block_t *)((unsigned char *)block + block->header.size);
        if (!islast && !next->header.alloc) {
            newsz = block->header.size + next->header.size;
            remove_block((free_block_t *)next,
                         get_bucket_index(next->header.size));
            write_meta((block_t *)block, newsz, false);
            insert_block(block, get_bucket_index(newsz));
        } else {
            write_meta((block_t *)block, block->header.size, false);
            insert_block(block, get_bucket_index(block->header.size));
        }
        return;
    }

    prevfooter = (footer_t *)((unsigned char *)block - sizeof(footer_t));
    prev = (block_t *)((unsigned char *)block - prevfooter->size);

    /* if the current block is the last block in the heap, coalescing the
     * previous block is needed. */
    if (islast) {
        if (!prev->header.alloc) {
            newsz = block->header.size + prev->header.size;
            remove_block((free_block_t *)prev,
                         get_bucket_index(prev->header.size));
            write_meta((block_t *)prev, newsz, false);
            insert_block((free_block_t *)prev, get_bucket_index(newsz));
        } else {
            write_meta((block_t *)block, block->header.size, false);
            insert_block(block, get_bucket_index(block->header.size));
        }
        return;
    }

    next = (block_t *)((unsigned char *)block + block->header.size);
    coalescing_mid((free_block_t *)prev, block, (free_block_t *)next);
}

/* Refill allocate a new free physical page into the free buckets pool, return 0
 * on success, -1 on failure. */
static inline int refill(void)
{
    struct pmm_page *phy_page;
    virt_addr_t pagebase;
    free_block_t *free;

    phy_page = pmm_alloc_page();
    if (!phy_page)
        return -1;

    pagebase = pmm_page_to_virt(phy_page);

    free = (free_block_t *)pagebase;
    write_meta((block_t *)free, PAGE_SIZE, false);
    insert_block(free, get_bucket_index(PAGE_SIZE));
    return 0;
}

void *kmalloc(size_t size)
{
    size_t index;
    size_t blocksz;
    void *p;

    if (!size || size > MAX_ALLOCSZ)
        return NULL;

    blocksz = CALC_BLOCKSZ(size);
    index = get_bucket_index(blocksz);

    if (blocksz > MAX_ALLOCSZ)
        return NULL;

    spinlock_lock(&lock);

    while (index < BUCKET_COUNT) {
        p = bestfit(blocksz, index);
        if (p) {
            spinlock_unlock(&lock);
            return p;
        }
        ++index;
    }

    if (refill() == -1) {
        spinlock_unlock(&lock);
        return NULL;
    }

    p = bestfit(blocksz, get_bucket_index(PAGE_SIZE));
    spinlock_unlock(&lock);
    return p;
}

void kfree(void *p)
{
    block_t *block;

    if (!p)
        return;

    block = (block_t *)((unsigned char *)p - sizeof(header_t));

    spinlock_lock(&lock);
    coalescing((free_block_t *)block);
    spinlock_unlock(&lock);
}

void *kcalloc(size_t num, size_t size)
{
    size_t bytes;
    void *p;

    if (!num || !size)
        return NULL;

    /* Calculate the size of the memory requested in bytes, and detect if there
     * is overflow */
    if (__builtin_mul_overflow(num, size, &bytes))
        return NULL;

    p = kmalloc(bytes);

    if (p)
        memset(p, 0, bytes);

    return p;
}