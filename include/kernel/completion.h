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

#ifndef _QUNIX_COMPLETION_H_
#define _QUNIX_COMPLETION_H_

#include <kernel/spinlock.h>
#include <kernel/waitq.h>

#define COMPLETION_STR "completion"

struct completion {
    wait_queue_t waitq;
    unsigned int done;
    struct spinlock lock;
};

typedef struct completion completion_t;

#define COMPLETION_INIT(name)                                                  \
    {.waitq = WAITQ_INIT(name.waitq),                                          \
     .done = 0,                                                                \
     .lock = SPINLOCK_INIT(COMPLETION_STR)}

extern void completion_init(completion_t *comp);
extern void wait_for_completion(completion_t *comp);
extern void complete(completion_t *comp);
extern void complete_all(completion_t *comp);

#endif /* _QUNIX_COMPLETION_H_ */