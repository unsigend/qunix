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

#ifndef _QUNIX_TTY_H_
#define _QUNIX_TTY_H_

#include <kernel/stdint.h>

enum tty_color {
    TTY_COLOR_BLACK = 0,
    TTY_COLOR_BLUE,
    TTY_COLOR_GREEN,
    TTY_COLOR_CYAN,
    TTY_COLOR_RED,
    TTY_COLOR_MAGENTA,
    TTY_COLOR_BROWN,
    TTY_COLOR_LGREY,
    TTY_COLOR_DGREY,
    TTY_COLOR_LBLUE,
    TTY_COLOR_LGREEN,
    TTY_COLOR_LCYAN,
    TTY_COLOR_LRED,
    TTY_COLOR_LMAG,
    TTY_COLOR_YELLOW,
    TTY_COLOR_WHITE,
};

struct tty_color_attr {
    enum tty_color fg; /* foreground color */
    enum tty_color bg; /* background color */
};

struct tty_ops {
    uint32_t width;  /* maximum width */
    uint32_t height; /* maximum height */

    void (*putc)(void *data, uint32_t x, uint32_t y,
                 struct tty_color_attr color, char c);
    void (*clear)(void *data, struct tty_color_attr color);
    void (*scroll)(void *data, uint32_t lines, struct tty_color_attr color);
    void (*update_cursor)(void *data, uint32_t x, uint32_t y); /* nullable */
};

struct tty_struct {
    uint32_t x;
    uint32_t y;
    uint32_t width, height; /* copy from ops */
    struct tty_color_attr color;
    void *data;
    const struct tty_ops *ops;
};

extern void tty_register(const struct tty_ops *ops, void *data);
extern void tty_putc(char c);
extern void tty_puts(const char *s);
extern void tty_clear(void);
extern void tty_set_color(struct tty_color_attr color);

#endif /* _QUNIX_TTY_H_ */