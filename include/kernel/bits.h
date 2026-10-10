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

#ifndef _QUNIX_BITS_H_
#define _QUNIX_BITS_H_

#define BITS_PER_BYTE __CHAR_BIT__
#define BITS_PER_LONG (__SIZEOF_LONG__ * BITS_PER_BYTE)

#define WORD_SIZE __SIZEOF_LONG__
#define WORD_BITS (BITS_PER_BYTE * WORD_SIZE)

#endif /* _QUNIX_BITS_H_ */