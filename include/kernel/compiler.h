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

#ifndef _QUNIX_COMPILER_H_
#define _QUNIX_COMPILER_H_

#define CONTRACT
#define UNUSED(x) (void)(x)

#define __packed __attribute__((packed))
#define __aligned(n) __attribute__((aligned(n)))
#define __noreturn __attribute__((noreturn))
#define __always_inline inline __attribute__((always_inline))

#define __export __attribute__((visibility("default")))
#define __hidden __attribute__((visibility("hidden")))

#define __printf(string, checks) __attribute__((format(printf, string, checks)))
#define __scanf(string, checks) __attribute__((format(scanf, string, checks)))

#endif /* _QUNIX_COMPILER_H_ */
