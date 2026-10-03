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

#ifndef _QUNIX_COND_H_
#define _QUNIX_COND_H_

#include <kernel/mutex.h>
#include <kernel/wait.h>

typedef struct cond {
    struct wait_queue wq;
} cond_t;

extern void cond_init(cond_t *cond);
extern void cond_wait(cond_t *cond, mutex_t *mutex);
extern void cond_signal(cond_t *cond);
extern void cond_broadcast(cond_t *cond);

#endif /* _QUNIX_COND_H_ */