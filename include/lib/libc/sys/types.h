/* qlibc - A light-weight and portable C standard library
 * Copyright (C) 2025 Qiu Yixiang
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

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_ 1

#include <bits/types.h>

typedef __dev_t dev_t;
typedef __dev64_t dev64_t;

typedef __off_t off_t;
typedef __off64_t off64_t;

typedef __ssize_t ssize_t;
typedef __ssize64_t ssize64_t;

typedef __blksize_t blksize_t;
typedef __blksize64_t blksize64_t;

typedef __blkcnt_t blkcnt_t;
typedef __blkcnt64_t blkcnt64_t;

typedef __time_t time_t;
typedef __time64_t time64_t;

typedef __ino_t ino_t;
typedef __ino64_t ino64_t;

typedef __nlink_t nlink_t;
typedef __nlink64_t nlink64_t;

typedef __mode_t mode_t;
typedef __uid_t uid_t;
typedef __gid_t gid_t;
typedef __pid_t pid_t;

#endif