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

#include <string.h>

char *strtok(char *restrict str, const char *restrict delim)
{
    static char *saved = NULL;
    if (str == NULL) {
        if (saved == NULL)
            return NULL;
        str = saved;
    }
    if (*str == '\0')
        return NULL;

    str += strspn(str, delim);
    if (*str == '\0')
        return NULL;

    char *token = str;
    str += strcspn(str, delim);
    if (*str == '\0') {
        saved = NULL;
        return token;
    }
    *str = '\0';
    saved = str + 1;
    return token;
}