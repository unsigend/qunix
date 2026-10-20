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

#include <kernel/completion.h>
#include <kernel/limits.h>

void completion_init(completion_t *comp)
{
    spinlock_init(&comp->lock, COMPLETION_STR);
    waitq_init(&comp->waitq);
    comp->done = 0;
}

void wait_for_completion(completion_t *comp)
{
    spinlock_lock(&comp->lock);

    while (comp->done == 0 && comp->done != UINT_MAX)
        waitq_sleep(&comp->waitq, &comp->lock);

    if (comp->done != UINT_MAX)
        comp->done--;

    spinlock_unlock(&comp->lock);
}

void complete(completion_t *comp)
{
    spinlock_lock(&comp->lock);

    if (comp->done != UINT_MAX)
        comp->done++;

    waitq_wakeup(&comp->waitq);

    spinlock_unlock(&comp->lock);
}

void complete_all(completion_t *comp)
{
    spinlock_lock(&comp->lock);

    comp->done = UINT_MAX;
    waitq_wakeup_all(&comp->waitq);

    spinlock_unlock(&comp->lock);
}