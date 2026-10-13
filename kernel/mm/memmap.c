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

#include <kernel/errno.h>
#include <kernel/macros.h>
#include <kernel/mm/memmap.h>
#include <kernel/panic.h>
#include <kernel/string.h>

#define MAX_REGIONS 64

static struct mem_region regions[MAX_REGIONS];
static size_t region_count;

size_t memmap_count(void) { return region_count; }
const struct mem_region *memmap_get_region(size_t index)
{
    if (index >= region_count)
        return NULL;
    return &regions[index];
}

static const char *const mem_type_str[] = {
    [MEM_AVAILABLE] = "AVAILABLE",
    [MEM_RESERVED] = "RESERVED",
    [MEM_KERNEL] = "KERNEL",
    [MEM_ACPI] = "ACPI",
};

static inline void panic_on_conflict(phys_addr_t start1, phys_addr_t end1,
                                     enum mem_type type1, phys_addr_t start2,
                                     phys_addr_t end2, enum mem_type type2)
{
    panic("memmap: conflict between region [0x%lx, 0x%lx) type %s and region "
          "[0x%lx, 0x%lx) type %s",
          (unsigned long)start1, (unsigned long)end1, mem_type_str[type1],
          (unsigned long)start2, (unsigned long)end2, mem_type_str[type2]);
}

static int append(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    if (region_count >= MAX_REGIONS)
        return -ENOMEM;

    regions[region_count].start = start;
    regions[region_count].end = end;
    regions[region_count].type = type;
    region_count++;
    return 0;
}

static int insert(size_t index, phys_addr_t start, phys_addr_t end,
                  enum mem_type type)
{
    if (region_count >= MAX_REGIONS)
        return -ENOMEM;
    if (index >= region_count)
        return append(start, end, type);

    memmove(&regions[index + 1], &regions[index],
            (region_count - index) * sizeof(struct mem_region));
    regions[index].start = start;
    regions[index].end = end;
    regions[index].type = type;
    region_count++;
    return 0;
}

static inline int overlaps(const struct mem_region *r, phys_addr_t start,
                           phys_addr_t end)
{
    return end > r->start && start < r->end;
}

static inline int adjacent(const struct mem_region *r, phys_addr_t start,
                           phys_addr_t end)
{
    return end == r->start || start == r->end;
}

static inline int mergeable(const struct mem_region *r, phys_addr_t start,
                            phys_addr_t end, enum mem_type type)
{
    return (overlaps(r, start, end) || adjacent(r, start, end)) &&
           r->type == type;
}

static inline int within(const struct mem_region *r, phys_addr_t start,
                         phys_addr_t end)
{
    return start >= r->start && end <= r->end;
}

static inline void merge(struct mem_region *r, phys_addr_t start,
                         phys_addr_t end)
{
    r->start = MIN(r->start, start);
    r->end = MAX(r->end, end);
}

/* Merge the current region to the previous region if they are mergeable.
 * WARNING: if you have a pointer to the index region, it might be invalidated
 * during the merge. So it is not safe to use the pointer after coalesce_left().
 */
static void coalesce_left(size_t index)
{
    struct mem_region *r;
    struct mem_region *prev;

    if (index == 0)
        return;

    r = &regions[index];
    prev = &regions[index - 1];

    while (index && mergeable(prev, r->start, r->end, r->type)) {
        merge(prev, r->start, r->end);
        if (index != region_count - 1) /* not last region */
            memmove(&regions[index], &regions[index + 1],
                    (region_count - index - 1) * sizeof(struct mem_region));
        index--;
        region_count--;
        if (index == 0)
            break;
        r = &regions[index];
        prev = &regions[index - 1];
    }
}

/* Merge the right region to the current region if they are mergeable. Holding a
 * pointer to the index region after coalesce_right() is safe. */
static void coalesce_right(size_t index)
{
    struct mem_region *r;
    struct mem_region *next;

    if (index == region_count - 1)
        return;

    r = &regions[index];
    next = &regions[index + 1];

    while (index != region_count - 1 &&
           mergeable(r, next->start, next->end, next->type)) {
        merge(r, next->start, next->end);
        if (index + 1 != region_count - 1) /* still region after next */
            memmove(&regions[index + 1], &regions[index + 2],
                    (region_count - index - 2) * sizeof(struct mem_region));
        region_count--;
        if (index == region_count - 1)
            break;
        next = &regions[index + 1];
    }
}

static void check_left(size_t index)
{
    struct mem_region *r;

    if (index == 0)
        return;

    r = &regions[index];

    for (size_t i = 0; i < index; i++) {
        if (overlaps(r, regions[i].start, regions[i].end) &&
            regions[i].type != r->type)
            panic_on_conflict(regions[i].start, regions[i].end, regions[i].type,
                              r->start, r->end, r->type);
    }
}

static void check_right(size_t index)
{
    struct mem_region *r;

    if (index == region_count - 1)
        return;

    r = &regions[index];

    for (size_t i = index + 1; i < region_count; i++) {
        if (overlaps(r, regions[i].start, regions[i].end) &&
            regions[i].type != r->type)
            panic_on_conflict(regions[i].start, regions[i].end, regions[i].type,
                              r->start, r->end, r->type);
    }
}

static inline int region_equals(const struct mem_region *r, phys_addr_t start,
                                phys_addr_t end, enum mem_type type)
{
    return r->start == start && r->end == end && r->type == type;
}

const struct mem_region *memmap_find_region(phys_addr_t start, phys_addr_t end,
                                            enum mem_type type)
{
    for (size_t i = 0; i < region_count; i++) {
        if (region_equals(&regions[i], start, end, type))
            return &regions[i];
    }
    return NULL;
}

static int find_region_by_index(phys_addr_t start, phys_addr_t end,
                                enum mem_type type)
{
    for (size_t i = 0; i < region_count; i++) {
        if (region_equals(&regions[i], start, end, type))
            return i;
    }
    return -1;
}

int memmap_remove_region(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    struct mem_region *r;

    if (end <= start)
        return -EINVAL;
    if (region_count == 0)
        return -ENOENT;

    for (size_t i = 0; i < region_count; i++) {
        r = &regions[i];

        if (region_equals(r, start, end, type)) {
            memmove(&regions[i], &regions[i + 1],
                    (region_count - i - 1) * sizeof(struct mem_region));
            region_count--;
            return 0;
        }
    }

    return -ENOENT;
}

int memmap_add_region(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    struct mem_region *r;

    if (end <= start)
        return -EINVAL;

    for (size_t i = 0; i < region_count; i++) {
        r = &regions[i];

        if (overlaps(r, start, end) && r->type != type)
            panic_on_conflict(r->start, r->end, r->type, start, end, type);

        if (mergeable(r, start, end, type)) {
            merge(r, start, end);
            coalesce_right(i);
            check_right(i);
            return 0;
        }

        else if (end <= r->start)
            return insert(i, start, end, type);
    }

    return append(start, end, type);
}

int memmap_punch_region(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    struct mem_region *r;
    struct mem_region old, copy;
    int err, added = 0;
    size_t i;
    int found = 0;

    if (end <= start)
        return -EINVAL;

    for (i = 0; i < region_count; i++) {
        r = &regions[i];
        if (within(r, start, end)) {
            if (r->type == type) /* quick path no-ops */
                return 0;
            found = 1;
            break;
        }
    }

    if (!found)
        return -ENOENT;

    old = *r; /* old store the old region */
    r->start = start;
    r->end = end;
    r->type = type;
    copy = *r; /* copy store the updated region */

    if (copy.start != old.start) /* split left */
    {
        err = memmap_add_region(old.start, copy.start, old.type);
        if (err) {
            i = find_region_by_index(copy.start, copy.end, copy.type);
            r = &regions[i];
            *r = old;
            return err;
        }
        added = 1;
        i = find_region_by_index(copy.start, copy.end, copy.type);
        r = &regions[i];
    }

    if (copy.end != old.end) /* split right */
    {
        err = memmap_add_region(copy.end, old.end, old.type);
        if (err) {
            if (added)
                memmap_remove_region(old.start, copy.start,
                                     old.type); /* remove will invalidate r */
            i = find_region_by_index(copy.start, copy.end, copy.type);
            r = &regions[i];
            *r = old;
            return err;
        }
    }

    i = find_region_by_index(copy.start, copy.end, copy.type);

    check_left(i);
    check_right(i);

    /* Must be done in this order since coalesce_left(i) might
     * invalidate the i */
    coalesce_right(i);
    coalesce_left(i);

    return 0;
}
