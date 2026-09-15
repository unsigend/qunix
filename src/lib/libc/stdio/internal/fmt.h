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

#ifndef _STDIO_IO_FMT_H_
#define _STDIO_IO_FMT_H_ 1

/* Length modifiers */
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

/* Specifiers */
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

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif