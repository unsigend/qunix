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

#ifndef _KERNEL_PRINTK_H_
#define _KERNEL_PRINTK_H_

#include <kernel/compiler.h>

#define LOG_LEVEL_DEBUG "[DEBUG]"
#define LOG_LEVEL_INFO "[INFO ]"
#define LOG_LEVEL_WARN "[WARN ]"
#define LOG_LEVEL_ERROR "[ERROR]"
#define LOG_LEVEL_FATAL "[FATAL]"

#define LOG(level, fmt, ...) printk(level " " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) printk(LOG_LEVEL_DEBUG " " fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) printk(LOG_LEVEL_INFO " " fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) printk(LOG_LEVEL_WARN " " fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printk(LOG_LEVEL_ERROR " " fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) printk(LOG_LEVEL_FATAL " " fmt, ##__VA_ARGS__)

#define LOGM(level, m, fmt, ...)                                               \
    printk(level " [%-6s] " fmt "\n", m, ##__VA_ARGS__)
#define LOGM_DEBUG(m, fmt, ...) LOGM(LOG_LEVEL_DEBUG, m, fmt, ##__VA_ARGS__)
#define LOGM_INFO(m, fmt, ...) LOGM(LOG_LEVEL_INFO, m, fmt, ##__VA_ARGS__)
#define LOGM_WARN(m, fmt, ...) LOGM(LOG_LEVEL_WARN, m, fmt, ##__VA_ARGS__)
#define LOGM_ERROR(m, fmt, ...) LOGM(LOG_LEVEL_ERROR, m, fmt, ##__VA_ARGS__)
#define LOGM_FATAL(m, fmt, ...) LOGM(LOG_LEVEL_FATAL, m, fmt, ##__VA_ARGS__)

__printf(1, 2) extern void printk(const char *fmt, ...);

#endif /* _KERNEL_PRINTK_H_ */