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

#ifndef _KERNEL_TTY_H_
#define _KERNEL_TTY_H_

#include <kernel/stddef.h>

struct tty_ops {
    void (*putc)(char c);
    void (*clear)(void);
};

extern int tty_register(struct tty_ops *ops);
extern void tty_putc(char c);
extern void tty_write(const char *s, size_t len);
extern void tty_write_raw(const char *s, size_t len); /* no lock */
extern void tty_clear(void);
extern void tty_read(char *buf, size_t len);
extern void tty_input(char c);

#endif /* _KERNEL_TTY_H_ */