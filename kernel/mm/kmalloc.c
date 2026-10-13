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
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>
#include <kernel/stdbool.h>
#include <kernel/stddef.h>
#include <kernel/stdint.h>
#include <kernel/string.h>

/*
 * Kernel memory allocator.
 *
 * A simple segregated best fit implementation migrated from qlibc. Each block
 * carries a header and a matching footer. The size is the whole block, and one
 * bit records whether it is allocated. Free blocks are kept in size-class
 * buckets, largest class one page, and inserted at the head. The link pointers
 * live in the free payload.
 *
 * Invariants:
 *
 *  - A block is a half-open range that stays inside the page it was taken
 *    from. Neighbour of a free block are coalesced only within that page.
 *  - The footer repeats the header. Allocated blocks are not on a bucket.
 *
 * Memory comes from the page allocator, one page at a time, and is not
 * returned there when the page becomes free. A zero-size request, or one
 * whose block does not fit in a page, returns NULL. No locking.
 */

#define ALIGNMENT _Alignof(max_align_t) /* alignment size */
#define BUCKET_COUNT 64                 /* bucket count */

/* Meta data type: used for both header and footer. 32 bits are used on both
 * widths. 29 bits hold the block size, the low bits hold the allocation status.
 * The allocator itself never builds a block larger than one page. */
typedef uint32_t nmeta_t;

/* Design of the meta data: The first 29 bits are used to store the size of the
 * block: payload + meta data (header + footer ). The LSB is used to store the
 * allocation status. 0 means free, 1 means allocated.
 *
 * After alignment, the current implementation has some extra metadata overhead,
 * since needs add padding to the meta data to confirms the ANSI/ISO C standard.
 */
typedef struct meta {
    nmeta_t alloc : 1;  /* allocation status */
    nmeta_t unused : 2; /* unused bits */
    nmeta_t sz : 29;    /* size of the whole block */
} __aligned(ALIGNMENT) meta_t;

typedef meta_t header_t;
typedef meta_t footer_t;

typedef struct block {
    header_t header;         /* header of the block */
    unsigned char payload[]; /* payload of the block */
} block_t;

typedef struct free_block {
    header_t header;         /* header of the free block */
    struct free_block *next; /* next free block */
    struct free_block *prev; /* previous free block */
    unsigned char payload[]; /* payload of the block */
} free_block_t;

/* Minimum threshold of the block size */
#define MINIMUM_THRESHOLD                                                      \
    (sizeof(header_t) + sizeof(footer_t) + 2 * sizeof(uintptr_t))

static free_block_t *buckets[BUCKET_COUNT]; /* free buckets */

/* Bucket size lookup table. Base thresholds: MINIMUM_THRESHOLD  24 on 32-bit,
   48 on 64-bit */
static const size_t slots[BUCKET_COUNT] = {
    /* Tiny: 8-byte increments */
    MINIMUM_THRESHOLD, 32, 40, 48, 56, 64, 72, 80,
    /* Small: 16-byte increments */
    96, 112, 128, 144, 160, 192, 224, 256,
    /* Medium: Fibonacci-like */
    288, 320, 384, 448, 512, 576, 640, 768, 896, 1024, 1280, 1536,
    /* Large: 512-1024 byte steps */
    2048, 2560, 3072, PAGE_SIZE};

/* Calculate the required size of the block with meta data after alignment. */
static inline size_t calc_needed_size(size_t sz)
{
    return MAX(ALIGN(sz + sizeof(header_t) + sizeof(footer_t), ALIGNMENT),
               ALIGN(MINIMUM_THRESHOLD, ALIGNMENT));
}

static inline footer_t *get_footer(block_t *block)
{
    return (footer_t *)((char *)block + (block)->header.sz - sizeof(footer_t));
}

static void write_meta(block_t *block, size_t sz, bool alloc)
{
    footer_t *footer;

    block->header.sz = sz;
    block->header.alloc = alloc;
    footer = get_footer(block);
    *footer = block->header;
}

static size_t get_bucket_index(size_t sz)
{
    for (size_t i = 0; i < BUCKET_COUNT; ++i) {
        if (sz <= slots[i])
            return i;
    }
    return BUCKET_COUNT - 1;
}

/* Insert a new block into the free buckets based on LIFO strategy. */
static void insert(free_block_t *block, size_t index)
{
    free_block_t *freehead;

    freehead = buckets[index];
    block->next = freehead;
    block->prev = NULL;
    if (freehead)
        freehead->prev = block;

    buckets[index] = block;
}

static free_block_t *remove_at(free_block_t *block, size_t index)
{
    free_block_t *prevblk, *nextblk;

    prevblk = block->prev;
    nextblk = block->next;
    if (prevblk)
        prevblk->next = nextblk;
    else
        buckets[index] = nextblk;

    if (nextblk)
        nextblk->prev = prevblk;

    block->next = NULL;
    block->prev = NULL;
    return block;
}

static void *slice(free_block_t *block, size_t sz)
{
    size_t blocksz, leftsz;
    free_block_t *remain;

    blocksz = block->header.sz;
    leftsz = blocksz - sz;

    /* If the left size is less than the minimum block size, return the whole
     * block, since it is not worth splitting and acceptable internal
     * fragmentation. */
    if (leftsz < MINIMUM_THRESHOLD) {
        write_meta((block_t *)block, blocksz, true);
        return (void *)((unsigned char *)block + sizeof(header_t));
    }

    /* Otherwise, split the block into two parts: the first part is the
     * requested size, the second part is the remaining block. */
    remain = (free_block_t *)((unsigned char *)block + sz);
    write_meta((block_t *)block, sz, true);
    write_meta((block_t *)remain, leftsz, false);
    insert(remain, get_bucket_index(leftsz));

    return (void *)((unsigned char *)block + sizeof(header_t));
}

static void coalescing_mid(free_block_t *prev, free_block_t *block,
                           free_block_t *next)
{
    size_t newsz;

    if (prev->header.alloc && next->header.alloc) /* no coalescing needed */
    {
        write_meta((block_t *)block, block->header.sz, false);
        insert(block, get_bucket_index(block->header.sz));
        return;
    }

    if (prev->header.alloc &&
        !next->header.alloc) /* coalescing the next block */
    {
        newsz = block->header.sz + next->header.sz;
        remove_at(next, get_bucket_index(next->header.sz));
        write_meta((block_t *)block, newsz, false);
        insert(block, get_bucket_index(newsz));
        return;
    }

    if (!prev->header.alloc &&
        next->header.alloc) /* coalescing the previous block */
    {
        newsz = block->header.sz + prev->header.sz;
        remove_at(prev, get_bucket_index(prev->header.sz));
        write_meta((block_t *)prev, newsz, false);
        insert(prev, get_bucket_index(newsz));
        return;
    }

    else /* coalescing the previous and next blocks */
    {
        newsz = prev->header.sz + block->header.sz + next->header.sz;
        remove_at(next, get_bucket_index(next->header.sz));
        remove_at(prev, get_bucket_index(prev->header.sz));
        write_meta((block_t *)prev, newsz, false);
        insert(prev, get_bucket_index(newsz));
        return;
    }
}

static void coalescing(free_block_t *block)
{
    bool isfirst, islast;
    block_t *next, *prev;
    footer_t *prevfooter;
    uintptr_t pagebase;

    pagebase = PAGE_ALIGN_DOWN((uintptr_t)block);
    isfirst = (pagebase == (uintptr_t)block);
    islast = (pagebase + PAGE_SIZE == (uintptr_t)block + block->header.sz);

    /* If the current block is the first block in the heap, coalescing the
    next block is needed.*/
    if (isfirst) {
        next = (block_t *)((unsigned char *)block + block->header.sz);
        if (!islast && !next->header.alloc) {
            size_t newsz = block->header.sz + next->header.sz;
            remove_at((free_block_t *)next, get_bucket_index(next->header.sz));
            write_meta((block_t *)block, newsz, false);
            insert(block, get_bucket_index(newsz));
        } else {
            write_meta((block_t *)block, block->header.sz, false);
            insert(block, get_bucket_index(block->header.sz));
        }
        return;
    }

    prevfooter = (footer_t *)((unsigned char *)block - sizeof(footer_t));
    prev = (block_t *)((unsigned char *)block - prevfooter->sz);

    /* If the current block is the last block in the heap, coalescing the
    previous block is needed.*/
    if (islast) {
        if (!prev->header.alloc) {
            size_t newsz = block->header.sz + prev->header.sz;
            remove_at((free_block_t *)prev, get_bucket_index(prev->header.sz));
            write_meta((block_t *)prev, newsz, false);
            insert((free_block_t *)prev, get_bucket_index(newsz));
        } else {
            write_meta((block_t *)block, block->header.sz, false);
            insert(block, get_bucket_index(block->header.sz));
        }
        return;
    }

    next = (block_t *)((unsigned char *)block + block->header.sz);
    coalescing_mid((free_block_t *)prev, block, (free_block_t *)next);
}

__unused static void *firstfit(size_t sz, size_t index)
{
    free_block_t *block;
    void *p;

    block = buckets[index];

    while (block) {
        if (block->header.sz >= sz) {
            remove_at(block, index);
            p = slice(block, sz);
            return p;
        }
        block = block->next;
    }
    return NULL;
}

static void *bestfit(size_t sz, size_t index)
{
    free_block_t *block;
    free_block_t *bestblock;
    void *p;

    block = buckets[index];
    bestblock = NULL;

    while (block) {
        if (block->header.sz >= sz) {
            if (!bestblock || block->header.sz < bestblock->header.sz)
                bestblock = block;
        }
        block = block->next;
    }
    if (bestblock) {
        remove_at(bestblock, index);
        p = slice(bestblock, sz);
        return p;
    }
    return NULL;
}

/* Refill a new page into the free buckets. Return 0 if successful, -1 if
 * failed. */
static inline int refill(void)
{
    struct pmm_page *page;
    virt_addr_t va;
    free_block_t *block;

    page = pmm_alloc_page();
    if (!page)
        return -1;

    va = pmm_page_to_va(page);
    block = (free_block_t *)va;
    write_meta((block_t *)block, PAGE_SIZE, false);
    insert(block, get_bucket_index(PAGE_SIZE));

    return 0;
}

void *kmalloc(size_t size)
{
    size_t needed;
    size_t index;
    void *p;

    if (!size)
        return NULL;
    if (size > PAGE_SIZE)
        return NULL;

    needed = calc_needed_size(size);
    if (needed > PAGE_SIZE)
        return NULL;
    index = get_bucket_index(needed);

    while (index < BUCKET_COUNT) {
        p = bestfit(needed, index);
        if (p)
            return p;
        ++index;
    }

    if (refill() == -1)
        return NULL;

    return bestfit(needed, get_bucket_index(PAGE_SIZE));
}

void kfree(void *ptr)
{
    block_t *block;

    if (!ptr)
        return;

    block = (block_t *)((unsigned char *)ptr - sizeof(header_t));
    coalescing((free_block_t *)block);
}

void *kcalloc(size_t num, size_t size)
{
    size_t bytes;
    void *p;

    if (!num || !size)
        return NULL;

    if (__builtin_mul_overflow(num, size, &bytes))
        return NULL;

    p = kmalloc(bytes);
    if (!p)
        return NULL;

    memset(p, 0, bytes);
    return p;
}