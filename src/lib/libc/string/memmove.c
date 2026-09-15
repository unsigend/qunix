/* qlibc - A light-weight and portable C standard library
 * Copyright (C) 2025 Qiu Yixiang
 *
 * This program is free software: you can redistribute it and and/or modify
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

void *memmove(void *dest, const void *src, size_t count)
{
    unsigned char *wp = (unsigned char *)dest;
    const unsigned char *rp = (const unsigned char *)src;

    if (wp == rp || count == 0)
        return dest;

    if (wp < rp || rp + count <= wp)
        return memcpy(dest, src, count);
    else {
        wp += count, rp += count;
        while (count--) {
            wp--, rp--;
            *wp = *rp;
        }
    }
    return dest;
}