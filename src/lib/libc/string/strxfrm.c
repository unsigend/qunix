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

#include <stddef.h>
#include <string.h>

/* Only copy the string, don't support locale transformation yet. */
size_t strxfrm(char *restrict dest, const char *restrict src, size_t count)
{
    size_t n = 0;
    while (*src && n < count) {
        *((unsigned char *)dest++) = *((unsigned char *)src++);
        ++n;
    }
    *((unsigned char *)dest) = '\0';
    return n;
}