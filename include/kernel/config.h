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

/* Auto generated, don't modify */

#ifndef _QUNIX_CONFIG_H_
#define _QUNIX_CONFIG_H_

#define CONFIG_MAX_CPUS 1
#define CONFIG_SMP 0
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_SPINLOCK 0
#ifndef CONFIG_TEST
#define CONFIG_TEST 0
#endif
#ifndef CONFIG_TEST_QEMU_EXIT
#define CONFIG_TEST_QEMU_EXIT 0
#endif
#define CONFIG_VERSION_MAJOR 0
#define CONFIG_VERSION_MINOR 1
#define CONFIG_VERSION_PATCH 1
#define CONFIG_PAGESIZE 4096
#define CONFIG_PAGESHIFT 12
#define CONFIG_PMM_SIMPLE 1
#define CONFIG_PMM_BITMAP 0
#define CONFIG_PMM_BUDDY 0
#define CONFIG_CLOCK_HZ 1000
#define CONFIG_QEMU_DEBUG_PORT 0xf4

#endif /* _QUNIX_CONFIG_H_ */
