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

int vsprintf(char *restrict buffer, const char *restrict format, va_list vlist)
{
    return vsnprintf(buffer, INT_MAX, format, vlist);
}

int vsnprintf(char *restrict buffer, size_t bufsz, const char *restrict format,
              va_list vlist)
{
    return printf_core(buffer, bufsz, format, vlist);
}

int sprintf(char *restrict buffer, const char *restrict format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = vsnprintf(buffer, INT_MAX, format, ap);
    va_end(ap);
    return ret;
}

int snprintf(char *restrict buffer, size_t bufsz, const char *restrict format,
             ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = vsnprintf(buffer, bufsz, format, ap);
    va_end(ap);
    return ret;
}