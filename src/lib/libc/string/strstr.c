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

char *strstr(const char *str, const char *substr)
{
    if (*substr == '\0')
        return (char *)str;

    const char *search = str;
    const char *pattern = substr;

    while (*search) {
        if (*search == *pattern) {
            const char *match = search;
            while (*match && *match == *pattern)
                ++match, ++pattern;

            if (*pattern == '\0')
                return (char *)search;
            pattern = substr;
        }
        ++search;
    }
    return NULL;
}