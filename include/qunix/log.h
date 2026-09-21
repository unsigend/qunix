/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_LOG_H
#define QUNIX_LOG_H

#include <qunix/kernel.h>

#define LOG_LEVEL_DEBUG "[DEBUG  ]"
#define LOG_LEVEL_INFO "[INFO   ]"
#define LOG_LEVEL_WARN "[WARNING]"
#define LOG_LEVEL_FATAL "[FATAL  ]"

/* LOG is a macro that prints a log message with a given level and format. */
#define LOG(level, fmt, ...) printk(level " " fmt "\n", ##__VA_ARGS__)

/* LOGM is a macro that prints a log message with a given level, module name and
 * format. */
#define LOGM(level, m, fmt, ...)                                               \
    printk(level "[%-8s] " fmt "\n", m, ##__VA_ARGS__)

#endif