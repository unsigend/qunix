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

#ifndef _QUNIX_CDEV_H_
#define _QUNIX_CDEV_H_

#include <kernel/stddef.h>
#include <kernel/types.h>

struct cdev;

struct cdev_ops {
    int (*open)(struct cdev *dev);
    ssize_t (*read)(struct cdev *dev, void *buf, size_t count);
    ssize_t (*write)(struct cdev *dev, const void *buf, size_t count);
    int (*ioctl)(struct cdev *dev, unsigned long cmd, unsigned long arg);
};

struct cdev {
    unsigned int major;
    unsigned int minor;
    const struct cdev_ops *ops;
};

extern int cdev_register(struct cdev *dev);
extern struct cdev *cdev_get(unsigned int major, unsigned int minor);

#endif /* _QUNIX_CDEV_H_ */