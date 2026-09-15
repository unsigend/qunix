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

#ifndef _BITS_TYPES_H_
#define _BITS_TYPES_H_ 1

typedef unsigned long long __dev_t;
typedef __dev_t __dev64_t;

typedef long __off_t;
typedef long long __off64_t;

typedef long __ssize_t;
typedef long long __ssize64_t;

typedef long __blksize_t;
typedef long long __blksize64_t;

typedef long __blkcnt_t;
typedef long long __blkcnt64_t;

typedef long __time_t;
typedef long long __time64_t;

typedef unsigned long __ino_t;
typedef unsigned long long __ino64_t;

typedef unsigned long __nlink_t;
typedef unsigned long long __nlink64_t;

typedef int __mode_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int __pid_t;

/* An integer type that can be accessed as an atomic entity */
typedef int __sig_atomic_t;

/* A set of signals to be blocked, unblocked, or waited for.*/
typedef unsigned long long __sigset_t;

#endif