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

#ifndef _QUNIX_CMDLINE_H_
#define _QUNIX_CMDLINE_H_

/*
 * cmdline_set - store one command-line key/value pair.
 *
 * Boot-only. Copies @key and @value into static storage. NULL @value
 * stores "". A second set of the same key overwrites the value.
 *
 * Returns 0 on success, -EINVAL if key is NULL or empty,
 * -ENOSPC if the table is full, -ENAMETOOLONG if a copy does not fit.
 */
extern int cmdline_set(const char *key, const char *value);

/*
 * cmdline_get - look up a command-line key.
 *
 * Returns the stored value, or NULL if the key is absent.
 * The pointer remains valid for the life of the kernel.
 */
extern const char *cmdline_get(const char *key);

#endif /* _QUNIX_CMDLINE_H_ */