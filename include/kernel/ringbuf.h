/* collection - A high-performance C data structure library
 * Copyright (C) 2026-2027 Yixiang Qiu
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

#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#include <kernel/stdbool.h>
#include <kernel/stddef.h>

/* A simple ring buffer implementation, circular buffer without malloc or heap
 * allocation. */

struct ringbuf {
    void *buf;
    size_t cap; /* capacity */
    size_t len; /* current length */
    size_t head;
    size_t tail;
    size_t elesz;
};

#define ringbuf_len(rb) ((rb)->len)
#define ringbuf_cap(rb) ((rb)->cap)
#define ringbuf_buf(rb) ((rb)->buf)
#define ringbuf_elesz(rb) ((rb)->elesz)
#define ringbuf_empty(rb) ((rb)->len == 0)
#define ringbuf_full(rb) ((rb)->len == (rb)->cap)
#define ringbuf_avail(rb) ((rb)->cap - (rb)->len)

#define RINGBUF_INIT(p, n, sz)                                                 \
    {.buf = p, .cap = n, .len = 0, .head = 0, .tail = 0, .elesz = sz}

extern int ringbuf_init(struct ringbuf *rb, void *buf, size_t cap,
                        size_t elesz);
extern void ringbuf_clear(struct ringbuf *rb);
extern int ringbuf_push(struct ringbuf *rb, const void *data);
extern int ringbuf_pop(struct ringbuf *rb, void *data);
extern int ringbuf_peek(struct ringbuf *rb, void *data);
extern void ringbuf_drop(struct ringbuf *rb, size_t n);

#endif /* _RINGBUF_H_ */