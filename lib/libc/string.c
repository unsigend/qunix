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

#include <kernel/stddef.h>
#include <kernel/string.h>

void *memchr(const void *ptr, int ch, size_t count)
{
    const unsigned char *p = (const unsigned char *)ptr;

    for (size_t i = 0; i < count; i++)
        if (p[i] == (unsigned char)ch)
            return (void *)(p + i);
    return NULL;
}

int memcmp(const void *lhs, const void *rhs, size_t count)
{
    const unsigned char *lp = (const unsigned char *)lhs;
    const unsigned char *rp = (const unsigned char *)rhs;

    for (size_t i = 0; i < count; i++)
        if (lp[i] != rp[i])
            return lp[i] - rp[i];
    return 0;
}

void *memcpy(void *restrict dest, const void *restrict src, size_t count)
{
    unsigned char *wp = (unsigned char *)dest;
    const unsigned char *rp = (const unsigned char *)src;

    while (count--)
        *wp++ = *rp++;
    return dest;
}

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

void *memset(void *dest, int ch, size_t count)
{
    unsigned char *wp = (unsigned char *)dest;

    for (size_t i = 0; i < count; i++)
        wp[i] = (unsigned char)ch;

    return dest;
}

char *strcat(char *restrict dest, const char *restrict src)
{
    char *wp = dest;

    while (*dest)
        ++dest;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
    return wp;
}

char *strchr(const char *str, int ch)
{
    while (*str != '\0') {
        if (*str == (unsigned char)ch)
            return (char *)str;
        ++str;
    }
    if (ch == '\0')
        return (char *)str;

    return NULL;
}

int strcmp(const char *lhs, const char *rhs)
{
    while (*lhs && *lhs == *rhs)
        ++lhs, ++rhs;
    return (int)((unsigned char)*lhs - (unsigned char)*rhs);
}

char *strcpy(char *restrict dest, const char *restrict src)
{
    memcpy(dest, src, strlen(src) + 1);
    return dest;
}

size_t strcspn(const char *str, const char *charset)
{
    unsigned char map[256] = {0};
    size_t n = 0;

    for (; *charset; ++charset)
        map[(unsigned char)*charset] = 1;

    while (*str && map[*((unsigned char *)str)] != 1)
        ++n, ++str;

    return n;
}

size_t strlen(const char *str)
{
    size_t len = 0;

    while (*str++)
        ++len;
    return len;
}

char *strncat(char *restrict dest, const char *restrict src, size_t count)
{
    size_t wlen = strlen(dest);
    size_t rlen = strlen(src);

    if (rlen < count)
        memcpy(dest + wlen, src, rlen + 1);
    else {
        memcpy(dest + wlen, src, count);
        dest[wlen + count] = '\0';
    }

    return dest;
}

int strncmp(const char *lhs, const char *rhs, size_t count)
{
    while (count--) {
        if (*lhs != *rhs)
            return (int)((unsigned char)*lhs - (unsigned char)*rhs);
        if (*lhs == '\0')
            return 0;
        ++lhs;
        ++rhs;
    }
    return 0;
}

char *strncpy(char *restrict dest, const char *restrict src, size_t count)
{
    size_t len = strlen(src);

    if (len < count) {
        memcpy(dest, src, len);
        memset(dest + len, '\0', count - len);
    } else
        memcpy(dest, src, count);

    return dest;
}

char *strpbrk(const char *str, const char *charset)
{
    unsigned char map[256] = {0};

    if (*str == '\0' || *charset == '\0')
        return NULL;

    while (*charset)
        map[(unsigned char)*charset++] = 1;

    for (; *str; str++)
        if (map[(unsigned char)*str])
            return (char *)str;

    return NULL;
}

char *strrchr(const char *str, int ch)
{
    const char *last = NULL;

    while (*str != '\0') {
        if (*str == (unsigned char)ch)
            last = str;
        ++str;
    }
    if (ch == '\0')
        return (char *)str;
    return (char *)last;
}

size_t strspn(const char *str, const char *charset)
{
    unsigned char map[256] = {0};
    size_t n = 0;

    for (; *charset; ++charset)
        map[(unsigned char)*charset] = 1;

    while (map[*((unsigned char *)str++)])
        ++n;

    return n;
}

char *strstr(const char *str, const char *substr)
{
    const char *search = str;
    const char *pattern = substr;

    if (*substr == '\0')
        return (char *)str;

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
