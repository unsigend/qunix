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
#define STATIC_ASSERT(expr) _Static_assert(expr, #expr " check failed")

#define EXPORT_SYM(sym) __attribute__((visibility("default"))) sym
#define HIDDEN_SYM(sym) __attribute__((visibility("hidden"))) sym

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define unreachable() __builtin_unreachable()

#define __packed __attribute__((packed))
#define __aligned(n) __attribute__((aligned(n)))
#define __noreturn __attribute__((noreturn))
#define __unused __attribute__((unused))
#define __used __attribute__((used))
#define __section(s) __attribute__((section(s)))
#define __must_check __attribute__((warn_unused_result))
#define __cold __attribute__((cold))
#define __noinline __attribute__((noinline))
#define __always_inline inline __attribute__((always_inline))

#define __offsetof(type, member) __builtin_offsetof(type, member)

#define __export __attribute__((visibility("default")))
#define __hidden __attribute__((visibility("hidden")))

#define __printf(string, checks) __attribute__((format(printf, string, checks)))
#define __scanf(string, checks) __attribute__((format(scanf, string, checks)))

#endif /* _QUNIX_COMPILER_H_ */