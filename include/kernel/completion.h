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

#include <kernel/wait.h>

typedef struct completion {
    int done;
    struct wait_queue wq;
} completion_t;

extern void completion_init(completion_t *completion);
extern void wait_for_completion(completion_t *completion);
extern void complete(completion_t *completion);
extern void complete_all(completion_t *completion);

#endif /* _QUNIX_COMPLETION_H_ */