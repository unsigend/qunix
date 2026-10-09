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

#ifndef _FMT_H_
#define _FMT_H_

#include <kernel/compiler.h>
#include <kernel/stdarg.h>
#include <kernel/stddef.h>

enum {
    LEN_NONE = 0,
    LEN_H,
    LEN_HH,
    LEN_L,
    LEN_LL,
    LEN_Z,
    LEN_PTR,
};

/* These enum values are used to pop arguments from the stack */
enum {
    ARG_PTR,
    ARG_SHORT,
    ARG_USHORT,
    ARG_CHAR,
    ARG_UCHAR,
    ARG_INT,
    ARG_UINT,
    ARG_LONG,
    ARG_ULONG,
    ARG_LLONG,
    ARG_ULLONG,
    ARG_IMAX,
    ARG_UMAX,
    ARG_SIZE_T,
    ARG_SSIZE_T,
};

enum {
    SPEC_C = 'c',
    SPEC_S = 's',
    SPEC_I = 'i',
    SPEC_D = 'd',
    SPEC_O = 'o',
    SPEC_X = 'x',
    SPEC_XX = 'X',
    SPEC_U = 'u',
    SPEC_P = 'p',
};

/* State machine for the parser */
enum {
    STATE_NORMAL, /* Normal state, copy characters from input to buffer */
    STATE_FORMAT, /* Format state, parse the format string */
};

/* Formatted core output function */
__printf(3, 0) extern int printf_core(char *restrict buf, size_t bufsz,
                                      const char *restrict fmt, va_list vlist);

#endif /* _FMT_H_ */