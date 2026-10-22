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

#include <kernel/backtrace.h>
#include <kernel/compiler.h>
#include <kernel/macros.h>
#include <kernel/sprintf.h>
#include <kernel/stdint.h>
#include <kernel/string.h>
#include <kernel/symtable.h>
#include <kernel/task.h>
#include <kernel/tty.h>

#define I386_STACK_ALIGNMENT sizeof(uint32_t)

struct bt_frame {
    struct bt_frame *next;
    uint32_t ret;
} __packed;

void backtrace(void)
{
    struct bt_frame *frame;
    unsigned int n;
    uintptr_t kstack;
    char buf[128];
    struct task *task;
    const struct ksym *sym;

    task = task_current();
    if (task)
        kstack = task->kstack;
    frame = (struct bt_frame *)__builtin_frame_address(0);

    for (n = 0; n < BT_MAX_DEPTH && frame; n++) {

        sym = ksym_lookup(frame->ret);

        if (sym)
            snprintf(buf, sizeof(buf), "%-13s +%.8lx %c\n", sym->name,
                     (unsigned long)(frame->ret - sym->addr), sym->type);
        else
            snprintf(buf, sizeof(buf), "0x%.8lx\n", (unsigned long)frame->ret);

        tty_write_raw(buf, strlen(buf));

        if ((task && (uintptr_t)frame->next < kstack) ||
            !IS_ALIGNED((uintptr_t)frame->next, I386_STACK_ALIGNMENT) ||
            frame->next <= frame)
            break;

        frame = frame->next;
    }
}