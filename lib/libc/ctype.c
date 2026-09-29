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

#include <kernel/ctype.h>

int isdigit(int c) { return (unsigned)c - '0' < 10; }
int isblank(int c) { return c == ' ' || c == '\t'; }
int isalpha(int c) { return ((unsigned)c | 32) - 'a' < 26; }
int isalnum(int c) { return isalpha(c) || isdigit(c); }
int iscntrl(int c) { return (unsigned)c <= 0x1f || c == 0x7f; }
int isgraph(int c) { return (unsigned)c <= 0x7e && (unsigned)c >= 0x21; }
int islower(int c) { return (unsigned)c - 'a' < 26; }
int isupper(int c) { return (unsigned)c - 'A' < 26; }
int isprint(int c) { return (unsigned)c <= 0x7e && (unsigned)c >= 0x20; }
int ispunct(int c) { return isgraph(c) && !isalnum(c); }
int isspace(int c) { return c == ' ' || (unsigned)c - '\t' < 5; }
int isxdigit(int c) { return isdigit(c) || ((unsigned)c | 32) - 'a' < 6; }

int tolower(int c)
{
    if (isupper(c))
        return c | 32;
    return c;
}

int toupper(int c)
{
    if (islower(c))
        return c & ~32;
    return c;
}