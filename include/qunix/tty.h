/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_TTY_H
#define QUNIX_TTY_H

#include <stdint.h>

struct tty_color {
    uint8_t fg; /* foreground color */
    uint8_t bg; /* background color */
};

/* TTY operations : All the vtable functions should be implemented by the
 * hardware driver. And register to the tty device through tty_register(). */
struct tty_ops {
    uint32_t width;  /* maximum width of the terminal */
    uint32_t height; /* maximum height of the terminal */

    void (*putc)(void *data, uint32_t x, uint32_t y, struct tty_color color,
                 char c);
    void (*clear)(void *data, struct tty_color color);
    void (*scroll)(void *data, uint32_t lines, struct tty_color color);
    void (*update_cursor)(void *data, uint32_t x, uint32_t y); /* nullable */
};

struct tty_struct {
    uint32_t x;
    uint32_t y;
    uint32_t width, height; /* copy from ops */
    struct tty_color color;
    void *data;
    const struct tty_ops *ops;
};

/* Register a new tty device, this function should be called by the hardware
 * driver which will mount the hardware to the tty device. */
extern void tty_register(const struct tty_ops *ops, void *data);

extern void tty_putc(char c);
extern void tty_puts(const char *s);
extern void tty_clear(void);
extern void tty_set_attr(struct tty_color color);

#endif