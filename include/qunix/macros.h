/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_MACROS_H
#define QUNIX_MACROS_H

/* CONTRACT is a symbol macro that does nothing but specify that the function
 * must be implemented or provided by specific hardware. Namely a sign for
 * hardware/architecture specific implementation. */
#define CONTRACT

#define ALIGN(x, align) (((x) + (align - 1)) & ~(align - 1))
#define IS_ALIGNED(x, align) (((x) & (align - 1)) == 0)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif