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

#include <kernel/atomic.h>
#include <kernel/cdev.h>
#include <kernel/errno.h>
#include <kernel/spinlock.h>

#define CDEV_MAX 16

struct cdev_table {
    struct cdev *devs[CDEV_MAX];
    size_t count;
    struct spinlock lock;
};

static struct cdev_table cdev_table = {
    .devs = {NULL},
    .count = 0,
    .lock = SPINLOCK_INIT("cdev_table"),
};

int cdev_register(struct cdev *dev)
{
    if (!dev || !dev->ops)
        return -EINVAL;

    spinlock_lock(&cdev_table.lock);

    for (size_t i = 0; i < cdev_table.count; i++) {
        if (cdev_table.devs[i]->major == dev->major &&
            cdev_table.devs[i]->minor == dev->minor) {
            spinlock_unlock(&cdev_table.lock);
            return -EEXIST;
        }
    }

    if (cdev_table.count >= CDEV_MAX) {
        spinlock_unlock(&cdev_table.lock);
        return -ENOMEM;
    }

    cdev_table.devs[cdev_table.count] = dev;
    cdev_table.count++;

    spinlock_unlock(&cdev_table.lock);
    return 0;
}

struct cdev *cdev_get(unsigned int major, unsigned int minor)
{
    spinlock_lock(&cdev_table.lock);

    for (size_t i = 0; i < cdev_table.count; i++) {
        if (cdev_table.devs[i]->major == major &&
            cdev_table.devs[i]->minor == minor) {
            spinlock_unlock(&cdev_table.lock);
            return cdev_table.devs[i];
        }
    }

    spinlock_unlock(&cdev_table.lock);
    return NULL;
}