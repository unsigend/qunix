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

#include "fmt.h"

#include <kernel/limits.h>
#include <kernel/sprintf.h>
#include <kernel/stdarg.h>

int sprintf(char *restrict buf, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vsnprintf(buf, INT_MAX, fmt, ap);
    va_end(ap);
    return ret;
}

int snprintf(char *restrict buf, size_t bufsz, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vsnprintf(buf, bufsz, fmt, ap);
    va_end(ap);
    return ret;
}

int vsprintf(char *restrict buf, const char *restrict fmt, va_list vlist)
{
    return vsnprintf(buf, INT_MAX, fmt, vlist);
}

int vsnprintf(char *restrict buf, size_t bufsz, const char *restrict fmt,
              va_list vlist)
{
    return printf_core(buf, bufsz, fmt, vlist);
}