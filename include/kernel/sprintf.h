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

#ifndef _QUNIX_SPRINTF_H_
#define _QUNIX_SPRINTF_H_

#include <kernel/compiler.h>
#include <kernel/stdarg.h>
#include <kernel/stddef.h>

extern __printf(2, 3) int sprintf(char *restrict buf, const char *restrict fmt,
                                  ...);
extern __printf(3, 4) int snprintf(char *restrict buf, size_t bufsz,
                                   const char *restrict fmt, ...);
extern __printf(2, 0) int vsprintf(char *restrict buf, const char *restrict fmt,
                                   va_list vlist);
extern __printf(3, 0) int vsnprintf(char *restrict buf, size_t bufsz,
                                    const char *restrict fmt, va_list vlist);

#endif /* _QUNIX_SPRINTF_H_ */