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

#include <kernel/errno.h>
#include <kernel/ringbuf.h>
#include <kernel/string.h>

int ringbuf_init(struct ringbuf *rb, void *buf, size_t cap, size_t elesz)
{
    if (elesz == 0 || cap == 0)
        return -EINVAL;

    rb->buf = buf;
    rb->cap = cap;
    rb->len = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->elesz = elesz;

    return 0;
}

void ringbuf_clear(struct ringbuf *rb)
{
    rb->len = 0;
    rb->head = 0;
    rb->tail = 0;
}

int ringbuf_push(struct ringbuf *rb, const void *data)
{
    if (ringbuf_full(rb))
        return -ENOSPC;

    if (rb->elesz == 1)
        ((unsigned char *)rb->buf)[rb->tail] = ((unsigned char *)data)[0];
    else
        memcpy(((unsigned char *)rb->buf) + rb->tail * rb->elesz, data,
               rb->elesz);

    rb->tail = (rb->tail + 1) % rb->cap;
    rb->len++;
    return 0;
}

int ringbuf_pop(struct ringbuf *rb, void *data)
{
    if (ringbuf_empty(rb))
        return -EAGAIN;

    if (rb->elesz == 1)
        ((unsigned char *)data)[0] = ((unsigned char *)rb->buf)[rb->head];
    else
        memcpy(data, ((unsigned char *)rb->buf) + rb->head * rb->elesz,
               rb->elesz);

    rb->head = (rb->head + 1) % rb->cap;
    rb->len--;
    return 0;
}

int ringbuf_peek(struct ringbuf *rb, void *data)
{
    if (ringbuf_empty(rb))
        return -EAGAIN;

    if (rb->elesz == 1)
        ((unsigned char *)data)[0] = ((unsigned char *)rb->buf)[rb->head];
    else
        memcpy(data, ((unsigned char *)rb->buf) + rb->head * rb->elesz,
               rb->elesz);
    return 0;
}

void ringbuf_drop(struct ringbuf *rb, size_t n)
{
    if (n == 0 || ringbuf_empty(rb))
        return;

    if (n > ringbuf_len(rb))
        n = ringbuf_len(rb);

    rb->head = (rb->head + n) % rb->cap;
    rb->len -= n;
}