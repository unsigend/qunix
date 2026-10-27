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

#ifndef _QUNIX_RINGBUF_H_
#define _QUNIX_RINGBUF_H_

#include <kernel/stdbool.h>
#include <kernel/stddef.h>

struct ringbuf {
    void *buf;
    size_t cap; /* elements count */
    size_t size;
    size_t esize; /* size of each element */
    size_t head;
    size_t tail;
};

#define RINGBUF_INIT(p, count, elesz)                                          \
    {.buf = (p),                                                               \
     .cap = (count),                                                           \
     .size = 0,                                                                \
     .esize = (elesz),                                                         \
     .head = 0,                                                                \
     .tail = 0}

extern void ringbuf_init(struct ringbuf *rb, void *buf, size_t count,
                         size_t elesz);
static inline bool ringbuf_is_empty(const struct ringbuf *rb);
static inline bool ringbuf_is_full(const struct ringbuf *rb);
static inline size_t ringbuf_size(const struct ringbuf *rb);
static inline size_t ringbuf_capacity(const struct ringbuf *rb);
extern int ringbuf_push(struct ringbuf *rb, const void *data);
extern int ringbuf_pop(struct ringbuf *rb, void *data);

static inline bool ringbuf_is_empty(const struct ringbuf *rb)
{
    return rb->size == 0;
}
static inline bool ringbuf_is_full(const struct ringbuf *rb)
{
    return rb->size == rb->cap;
}

static inline size_t ringbuf_size(const struct ringbuf *rb) { return rb->size; }
static inline size_t ringbuf_capacity(const struct ringbuf *rb)
{
    return rb->cap;
}

#endif /* _QUNIX_RINGBUF_H_ */