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

#ifndef _QLIBC_FEATURE_H_
#define _QLIBC_FEATURE_H_

/* Version macros */
#define QLIBC_VERSION_MAJOR 0
#define QLIBC_VERSION_MINOR 1
#define QLIBC_VERSION_PATCH 0

/* ISO C Standard macros */
#define ISO_C_STANDARD_C99 199901L
#define ISO_C_STANDARD_C11 201112L
#define ISO_C_STANDARD_C17 201710L
#define ISO_C_STANDARD_C23 202311L

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= ISO_C_STANDARD_C99
#define __USE_ISO_C99 1
#else
#define __USE_ISO_C99 0
#endif
#if __STDC_VERSION__ >= ISO_C_STANDARD_C11
#define __USE_ISO_C11 1
#else
#define __USE_ISO_C11 0
#endif
#if __STDC_VERSION__ >= ISO_C_STANDARD_C17
#define __USE_ISO_C17 1
#else
#define __USE_ISO_C17 0
#endif
#if __STDC_VERSION__ >= ISO_C_STANDARD_C23
#define __USE_ISO_C23 1
#else
#define __USE_ISO_C23 0
#endif
#else
#error "qlibc: ISO C Standard is not defined"
#endif

/* Feature support macros, these macros are used to enable or disable features.
   If the feature is not supported in current qlibc version, the macro will be
   ignored. */
#define __SUPPORT_MB 0      /* Multi-byte string support */
#define __SUPPORT_FPT 0     /* Floating-point support */
#define __SUPPORT_THREADS 0 /* Multi-thread support */

#if defined(_GNU_SOURCE)
#define _QLIBC_SOURCE 1
#endif

/* C++ specific macros */
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#define __hidden __attribute__((visibility("hidden")))
#define __packed __attribute__((packed))

#endif