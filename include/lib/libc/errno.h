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

#ifndef _ERRNO_H_
#define _ERRNO_H_ 1

#include <bits/errno.h>
#include <feature.h>

__BEGIN_DECLS

/* Get the current errno value. This is the actual function that returns the
   errno value. It is implemented-defined in the architecture-specific errno.h
   file. */
extern int *__errno(void);

/* Define the errno macro to the value returned by __errno. */
#define errno (*__errno())

__END_DECLS

#endif