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

#ifndef _STDARG_H_
#define _STDARG_H_ 1

/* GNU C compatible macros */
#if defined(__GNUC__) || defined(_GNU_SOURCE)
typedef __builtin_va_list __gnuc_va_list;
#endif

/* QLIBC specific macros */
typedef __builtin_va_list __qlibc_va_list;
typedef __qlibc_va_list va_list;

/* Variable argument macros, the implementation is based on the GNU GCC
   built-in support. The va_start, va_end, va_arg, va_copy macros are defined
   as the GNU GCC built-in macros. */

#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_copy(d, s) __builtin_va_copy(d, s)

#endif