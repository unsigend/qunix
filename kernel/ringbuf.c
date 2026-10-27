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

#include <kernel/assert.h>
#include <kernel/errno.h>
#include <kernel/ringbuf.h>
#include <kernel/string.h>

void ringbuf_init(struct ringbuf *rb, void *buf, size_t count, size_t elesz)
{
    assert(rb && buf && count && elesz);

    rb->buf = buf;
    rb->cap = count;
    rb->size = 0;
    rb->esize = elesz;
    rb->head = 0;
    rb->tail = 0;
}

int ringbuf_push(struct ringbuf *rb, const void *data)
{
    if (ringbuf_is_full(rb))
        return -ENOSPC;

    if (rb->esize == 1)
        ((unsigned char *)rb->buf)[rb->tail] = *(unsigned char *)data;
    else
        memcpy(rb->buf + rb->tail * rb->esize, data, rb->esize);

    rb->tail = (rb->tail + 1) % rb->cap;
    rb->size++;

    return 0;
}

int ringbuf_pop(struct ringbuf *rb, void *data)
{
    if (ringbuf_is_empty(rb))
        return -EAGAIN;

    if (rb->esize == 1 && data)
        *((unsigned char *)data) = ((unsigned char *)rb->buf)[rb->head];
    else if (data)
        memcpy(data, rb->buf + rb->head * rb->esize, rb->esize);

    rb->head = (rb->head + 1) % rb->cap;
    rb->size--;

    return 0;
}