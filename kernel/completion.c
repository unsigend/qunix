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
#include <kernel/interrupt.h>
#include <kernel/limits.h>

void completion_init(completion_t *completion)
{
    completion->done = 0;
    wait_queue_init(&completion->wq);
}

void wait_for_completion(completion_t *completion)
{
    unsigned long flags;

    flags = irq_save();
    while (!completion->done)
        wait_queue_sleep(&completion->wq);
    if (completion->done != INT_MAX)
        completion->done--;
    irq_restore(flags);
}

void complete(completion_t *completion)
{
    unsigned long flags;

    flags = irq_save();
    if (completion->done != INT_MAX)
        completion->done++;
    wait_queue_wakeup(&completion->wq);
    irq_restore(flags);
}

void complete_all(completion_t *completion)
{
    unsigned long flags;

    flags = irq_save();
    completion->done = INT_MAX; /* sentinel value */
    wait_queue_wakeup_all(&completion->wq);
    irq_restore(flags);
}