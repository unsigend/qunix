/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/tty.h>
#include <stdint.h>
#include <string.h>

static struct tty_struct tty_device;

void tty_register(const struct tty_ops *ops, void *data)
{
    memset(&tty_device, 0, sizeof(tty_device));

    tty_device.data = data;
    tty_device.ops = ops;
    tty_device.width = ops->width;
    tty_device.height = ops->height;

    tty_device.color.fg = TTY_COLOR_LGREY;
    tty_device.color.bg = TTY_COLOR_BLACK;
}

void tty_putc(char c)
{
    if (c == '\n') {
        if (tty_device.y == tty_device.height - 1)
            tty_device.ops->scroll(tty_device.data, 1, tty_device.color);
        else
            tty_device.y++;
        tty_device.x = 0;
    } else if (c == '\t') {
        uint32_t advance = 4 - (tty_device.x % 4);
        if (tty_device.x + advance > tty_device.width) {
            if (tty_device.y == tty_device.height - 1)
                tty_device.ops->scroll(tty_device.data, 1, tty_device.color);
            else
                tty_device.y++;
            tty_device.x = 0;
        } else
            tty_device.x += advance;
    } else if (c == '\r')
        tty_device.x = 0;
    else {
        if (tty_device.x == tty_device.width) {
            if (tty_device.y == tty_device.height - 1)
                tty_device.ops->scroll(tty_device.data, 1, tty_device.color);
            else
                tty_device.y++;
            tty_device.x = 0;
        }
        tty_device.ops->putc(tty_device.data, tty_device.x, tty_device.y,
                             tty_device.color, c);
        tty_device.x++;
    }
    if (tty_device.ops->update_cursor)
        tty_device.ops->update_cursor(tty_device.data, tty_device.x,
                                      tty_device.y);
}

void tty_puts(const char *s)
{
    while (*s)
        tty_putc(*s++);
}

void tty_clear(void)
{
    tty_device.x = 0;
    tty_device.y = 0;
    tty_device.ops->clear(tty_device.data, tty_device.color);
    if (tty_device.ops->update_cursor)
        tty_device.ops->update_cursor(tty_device.data, tty_device.x,
                                      tty_device.y);
}

void tty_set_color(struct tty_color_attr color) { tty_device.color = color; }