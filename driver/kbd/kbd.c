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

#include <asm/kbd.h>

#include <kernel/cdev.h>
#include <kernel/panic.h>
#include <kernel/types.h>

#include <driver/kbd.h>

static ssize_t kbd_cdev_read(struct cdev *cdev, void *buf, size_t count)
{
    char c;
    size_t n;
    UNUSED(cdev);

    for (n = 0; n < count; n++) {
        c = kbd_getc();
        ((char *)buf)[n] = c;
    }
    return n;
}

static struct cdev_ops kbd_cdev_ops = {
    .read = kbd_cdev_read,
};

static struct cdev kbd_cdev = {
    .major = 2,
    .minor = 0,
    .ops = &kbd_cdev_ops,
};

void kbd_cdev_init(void)
{
    int err;

    if ((err = cdev_register(&kbd_cdev)))
        panic_error(err, "failed to register kbd cdev");
}