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

#ifndef _STRING_H_
#define _STRING_H_ 1

#include <stddef.h>

__BEGIN_DECLS

extern void *memchr(const void *ptr, int ch, size_t count);
extern int memcmp(const void *lhs, const void *rhs, size_t count);
extern void *memset(void *dest, int ch, size_t count);
extern void *memcpy(void *restrict dest, const void *restrict src,
                    size_t count);
extern void *memmove(void *dest, const void *src, size_t count);

extern size_t strlen(const char *str);
extern int strcmp(const char *lhs, const char *rhs);
extern int strncmp(const char *lhs, const char *rhs, size_t count);
extern int strcoll(const char *lhs, const char *rhs);
extern char *strchr(const char *str, int ch);
extern char *strrchr(const char *str, int ch);

extern size_t strspn(const char *str, const char *charset);
extern size_t strcspn(const char *str, const char *charset);
extern char *strpbrk(const char *str, const char *charset);
extern char *strstr(const char *str, const char *substr);
extern char *strtok(char *restrict str, const char *restrict delim);
extern char *strcpy(char *restrict dest, const char *restrict src);
extern char *strncpy(char *restrict dest, const char *restrict src,
                     size_t count);
extern char *strcat(char *restrict dest, const char *restrict src);
extern char *strncat(char *restrict dest, const char *restrict src,
                     size_t count);
extern size_t strxfrm(char *restrict dest, const char *restrict src,
                      size_t count);

extern char *strerror(int errnum);
extern char *strsignal(int sig);
extern void psignal(int sig, const char *msg);

__END_DECLS

#endif