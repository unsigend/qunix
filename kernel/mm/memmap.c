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
#include <kernel/spinlock.h>
#include <kernel/string.h>

#define MAX_REGIONS 64

/*
 * Physical memory map.
 *
 * Regions are kept in a fixed-size sorted disjoint array by start address.
 * Invariants, held on entry to and exit from every public function:
 *
 *  - Regions are half-open ranges [start, end) with start < end.
 *  - Regions are pairwise disjoint.
 *  - Neighbour regions of the same type are never adjacent, they are
 *    coalesced into one.
 *
 * A region of one type overlapping a region of another type is a firmware
 * or caller bug, and is fatal (panic).
 *
 * Mutators check capacity before changing anything, so -ENOMEM leaves the
 * map untouched.
 *
 * Indices into regions[] are invalidated by any insert or remove. The
 * coalesce helpers may remove entries, so callers must not reuse an index
 * after calling them.
 *
 * One spinlock covers every public function. Interrupt handlers do not
 * update the map, so the lock does not disable interrupts. Lookups copy
 * the region out before unlocking.
 */

static struct mem_region regions[MAX_REGIONS];
static size_t region_count;
static struct spinlock lock =
    SPINLOCK_INIT("memmap"); /* lock the regions array and region_count */

size_t memmap_count(void)
{
    size_t count;
    spinlock_lock(&lock);
    count = region_count;
    spinlock_unlock(&lock);
    return count;
}

int memmap_get_region(size_t index, struct mem_region *region)
{
    spinlock_lock(&lock);

    if (index >= region_count) {
        spinlock_unlock(&lock);
        return -ENOENT;
    }

    *region = regions[index];
    spinlock_unlock(&lock);
    return 0;
}

static const char *const mem_type_str[] = {
    [MEM_AVAILABLE] = "AVAILABLE",
    [MEM_RESERVED] = "RESERVED",
    [MEM_KERNEL] = "KERNEL",
    [MEM_ACPI] = "ACPI",
};

static inline void panic_region_conflict(phys_addr_t start1, phys_addr_t end1,
                                         enum mem_type type1,
                                         phys_addr_t start2, phys_addr_t end2,
                                         enum mem_type type2)
{
    panic("memmap: conflict between region [0x%lx, 0x%lx) type %s and region "
          "[0x%lx, 0x%lx) type %s",
          (unsigned long)start1, (unsigned long)end1, mem_type_str[type1],
          (unsigned long)start2, (unsigned long)end2, mem_type_str[type2]);
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

static inline void remove_at(size_t index)
{
    memmove(&regions[index], &regions[index + 1],
            (region_count - index - 1) * sizeof(struct mem_region));
    region_count--;
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

static inline int equals(const struct mem_region *r, phys_addr_t start,
                         phys_addr_t end, enum mem_type type)
{
    return r->start == start && r->end == end && r->type == type;
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

static void coalesce_left(size_t index)
{
    while (index && mergeable(&regions[index - 1], regions[index].start,
                              regions[index].end, regions[index].type)) {
        merge(&regions[index - 1], regions[index].start, regions[index].end);
        remove_at(index--);
    }
}

static void coalesce_right(size_t index)
{
    while (index + 1 < region_count &&
           mergeable(&regions[index], regions[index + 1].start,
                     regions[index + 1].end, regions[index + 1].type)) {
        merge(&regions[index], regions[index + 1].start,
              regions[index + 1].end);
        remove_at(index + 1);
    }
}

int memmap_find_region(phys_addr_t start, phys_addr_t end, enum mem_type type,
                       struct mem_region *region)
{
    spinlock_lock(&lock);

    for (size_t i = 0; i < region_count; i++) {
        if (equals(&regions[i], start, end, type)) {
            *region = regions[i];
            spinlock_unlock(&lock);
            return 0;
        }
    }
    spinlock_unlock(&lock);
    return -ENOENT;
}

int memmap_remove_region(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    if (end <= start)
        return -EINVAL;

    spinlock_lock(&lock);
    for (size_t i = 0; i < region_count; i++) {
        if (equals(&regions[i], start, end, type)) {
            remove_at(i);
            spinlock_unlock(&lock);
            return 0;
        }
    }

    spinlock_unlock(&lock);
    return -ENOENT;
}

int memmap_add_region(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    struct mem_region *r;
    int ret;

    if (end <= start)
        return -EINVAL;

    spinlock_lock(&lock);
    for (size_t i = 0; i < region_count; i++) {
        r = &regions[i];
        if (overlaps(r, start, end) && r->type != type)
            panic_region_conflict(r->start, r->end, r->type, start, end, type);
    }

    for (size_t i = 0; i < region_count; i++) {
        r = &regions[i];

        if (mergeable(r, start, end, type)) {
            merge(r, start, end);
            coalesce_right(i);
            spinlock_unlock(&lock);
            return 0;
        } else if (end <= r->start) {
            ret = insert(i, start, end, type);
            spinlock_unlock(&lock);
            return ret;
        }
    }

    ret = append(start, end, type);
    spinlock_unlock(&lock);
    return ret;
}

int memmap_punch_region(phys_addr_t start, phys_addr_t end, enum mem_type type)
{
    struct mem_region old;
    size_t i, need;

    if (end <= start)
        return -EINVAL;

    spinlock_lock(&lock);
    for (i = 0; i < region_count; i++)
        if (within(&regions[i], start, end))
            break;

    if (i == region_count) {
        spinlock_unlock(&lock);
        return -ENOENT;
    }

    old = regions[i];

    if (old.type == type) {
        spinlock_unlock(&lock);
        return 0;
    }

    need = (old.start != start) + (old.end != end);
    if (region_count + need > MAX_REGIONS) {
        spinlock_unlock(&lock);
        return -ENOMEM;
    }

    if (old.start != start)
        insert(i++, old.start, start, old.type);
    if (old.end != end)
        insert(i + 1, end, old.end, old.type);

    regions[i].type = type;
    regions[i].start = start;
    regions[i].end = end;

    /* Must coalesce the right side first, since the coalesce_left() call will
     * invalidate the current index pointer */
    if (old.end == end)
        coalesce_right(i);
    if (old.start == start)
        coalesce_left(i);

    spinlock_unlock(&lock);
    return 0;
}

phys_addr_t memmap_max_ram_addr(void)
{
    phys_addr_t max = 0;

    spinlock_lock(&lock);

    if (region_count == 0) {
        spinlock_unlock(&lock);
        return 0;
    }

    for (size_t i = 0; i < region_count; i++) {
        if (regions[i].type == MEM_AVAILABLE || regions[i].type == MEM_KERNEL ||
            regions[i].type == MEM_ACPI)
            max = MAX(max, regions[i].end);
    }

    spinlock_unlock(&lock);
    return max;
}

const char *memmap_type_to_str(enum mem_type type)
{
    return mem_type_str[type];
}