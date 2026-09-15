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

#ifndef _EXT_PRINTF_H_
#define _EXT_PRINTF_H_ 1

#include <stdarg.h>
#include <stddef.h>

/* This is not part of ANSI/ISO C Standard header file, it is part of qlibc
   header extensions. This header just provide pure formatted output functions
   for kernel use. */

__BEGIN_DECLS

extern int sprintf(char *restrict buffer, const char *restrict format, ...);
extern int snprintf(char *restrict buffer, size_t bufsz,
                    const char *restrict format, ...);
extern int vsprintf(char *restrict buffer, const char *restrict format,
                    va_list vlist);
extern int vsnprintf(char *restrict buffer, size_t bufsz,
                     const char *restrict format, va_list vlist);

__END_DECLS

#endif