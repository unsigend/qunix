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

#ifndef _QUNIX_CTYPE_H_
#define _QUNIX_CTYPE_H_

static inline int islower(int c) { return ((unsigned)c - 'a' < 26); }
static inline int isupper(int c) { return ((unsigned)c - 'A' < 26); }

static inline int isdigit(int c) { return ((unsigned)c - '0' < 10); }
static inline int isalpha(int c) { return (((unsigned)c | 32) - 'a' < 26); }
static inline int isalnum(int c) { return isalpha(c) || isdigit(c); }
static inline int isodigit(const char c) { return c >= '0' && c <= '7'; }

static inline int isblank(int c) { return c == ' ' || c == '\t'; }
static inline int iscntrl(int c) { return ((unsigned)c <= 0x1f || c == 0x7f); }

static inline int isgraph(int c)
{
    return ((unsigned)c <= 0x7e && (unsigned)c >= 0x21);
}

static inline int isspace(int c)
{
    return (c == ' ' || (unsigned)c - '\t' < 5);
}

static inline int isprint(int c)
{
    return ((unsigned)c <= 0x7e && (unsigned)c >= 0x20);
}

static inline int ispunct(int c) { return (isgraph(c) && !isalnum(c)); }

static inline int isxdigit(int c)
{
    return (isdigit(c) || ((unsigned)c | 32) - 'a' < 6);
}

static inline int tolower(int c)
{
    if (isupper(c))
        return c | 0x20;
    return c;
}

static inline int toupper(int c)
{
    if (islower(c))
        return c & ~0x20;
    return c;
}

#define isascii(c) (((unsigned char)(c)) <= 0x7f)
#define toascii(c) (((unsigned char)(c)) & 0x7f)

#endif /* _QUNIX_CTYPE_H_ */