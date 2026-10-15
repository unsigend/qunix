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

#ifndef _KERNEL_SIZES_H_
#define _KERNEL_SIZES_H_

#define BYTE (1ULL)
#define KB (1024ULL)
#define MB (1024ULL * KB)
#define GB (1024ULL * MB)
#define TB (1024ULL * GB)
#define PB (1024ULL * TB)

#define BYTE_N(n) ((n) * BYTE)
#define GB_N(n) ((n) * GB)
#define MB_N(n) ((n) * MB)
#define KB_N(n) ((n) * KB)
#define TB_N(n) ((n) * TB)
#define PB_N(n) ((n) * PB)

#endif /* _KERNEL_SIZES_H_ */