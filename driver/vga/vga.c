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

#include "vga.h"

#include <asm/io.h>

#include <kernel/mm/memlayout.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/stdint.h>
#include <kernel/string.h>
#include <kernel/tty.h>

#include <driver/vga.h>

static inline uint8_t make_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t make_entry(unsigned char c, uint8_t color);

static inline void cursor_disable(void);
static inline void cursor_enable(uint8_t start, uint8_t end);
static inline void cursor_update(uint8_t row, uint8_t col);
static inline void cursor_sync(void);

static void clear_lines(unsigned int line, unsigned int n);
static void clear_screen(void);
static void scrollup(unsigned int lines);

static void putc(char c);

static struct vga_state vga_device;

static struct tty_ops ops = {
    .putc = putc,
    .clear = clear_screen,
};

void vga_init(void)
{
    int err;

    vga_device.buf = (uint16_t *)pa_to_va(VGA_TEXT_BUFBASE);
    vga_device.cursor.row = 0;
    vga_device.cursor.col = 0;
    vga_device.color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    cursor_enable(14, 15); /* underline cursor */
    cursor_update(0, 0);

    clear_screen();
    if ((err = tty_register(&ops)))
        panic_error(err, "failed to register vga tty");

    LOGM_INFO("VGA", "VGA text console initialized");
}

static inline uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
    return ((bg & 0x7) << 4) | (fg & 0xF);
}

static inline uint16_t make_entry(unsigned char c, uint8_t color)
{
    return (uint16_t)c | ((uint16_t)color << 8);
}

__unused static inline void cursor_disable(void)
{
    outb(VGA_CR_IDX, VGA_CR0A);
    outb(VGA_CR_DATA, 0x20);
}

static inline void cursor_enable(uint8_t start, uint8_t end)
{
    uint8_t old;

    outb(VGA_CR_IDX, VGA_CR0A);
    old = inb(VGA_CR_DATA);
    outb(VGA_CR_DATA, (old & 0xC0) | start);

    outb(VGA_CR_IDX, VGA_CR0B);
    old = inb(VGA_CR_DATA);
    outb(VGA_CR_DATA, (old & 0xE0) | end);
}

static inline void cursor_update(uint8_t row, uint8_t col)
{
    uint16_t pos;

    pos = row * VGA_TEXT_WIDTH + col;
    outb(VGA_CR_IDX, VGA_CR0F);
    outb(VGA_CR_DATA, (uint8_t)(pos & 0xFF));
    outb(VGA_CR_IDX, VGA_CR0E);
    outb(VGA_CR_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

static inline void cursor_sync(void)
{
    cursor_update(vga_device.cursor.row, vga_device.cursor.col);
}

/*
 * clear_lines - blank @n rows starting at @line.
 *
 * Cells are filled with a space in the current color. @n is shortened so
 * the write stays on the screen. The cursor is left unchanged.
 *
 * A zero count, or a @line past the last row, does nothing.
 */
static void clear_lines(unsigned int line, unsigned int n)
{
    uint16_t entry;

    if (n == 0 || line >= VGA_TEXT_HEIGHT)
        return;
    if (line + n >= VGA_TEXT_HEIGHT)
        n = VGA_TEXT_HEIGHT - line;

    entry = make_entry(' ', vga_device.color);

    for (size_t l = line; l < line + n; l++)
        for (size_t i = 0; i < VGA_TEXT_WIDTH; i++)
            vga_device.buf[l * VGA_TEXT_WIDTH + i] = entry;
}

/*
 * scrollup - move the screen up by @lines rows.
 *
 * The vacated rows at the bottom are blanked in the current color. The
 * cursor row moves with the text and stops at row 0. The column stays.
 *
 * A count of the whole screen or more clears the screen and homes the
 * cursor.
 */
static inline void scrollup(unsigned int lines)
{
    if (lines >= VGA_TEXT_HEIGHT) {
        clear_screen();
        return;
    }

    memmove((void *)vga_device.buf,
            (void *)(vga_device.buf + lines * VGA_TEXT_WIDTH),
            (VGA_TEXT_HEIGHT - lines) * VGA_TEXT_WIDTH * sizeof(uint16_t));

    clear_lines(VGA_TEXT_HEIGHT - lines, lines);
    if (lines >= vga_device.cursor.row)
        vga_device.cursor.row = 0;
    else
        vga_device.cursor.row -= lines;
    cursor_sync();
}

static void clear_screen(void)
{
    clear_lines(0, VGA_TEXT_HEIGHT);
    vga_device.cursor.row = 0;
    vga_device.cursor.col = 0;
    cursor_sync();
}

static void putc(char c)
{
    uint16_t entry;

    if (c == '\n') {
        vga_device.cursor.col = 0;
        vga_device.cursor.row++;

        if (vga_device.cursor.row >= VGA_TEXT_HEIGHT)
            scrollup(1);
    } else if (c == '\t') {
        uint8_t tabstop = 8 - (vga_device.cursor.col % 8);
        if (vga_device.cursor.col + tabstop >= VGA_TEXT_WIDTH)
            tabstop = VGA_TEXT_WIDTH - vga_device.cursor.col;
        for (uint8_t i = 0; i < tabstop; i++)
            putc(' ');

    } else if (c == '\r')
        vga_device.cursor.col = 0;

    else /* printable character */
    {
        if (vga_device.cursor.col >= VGA_TEXT_WIDTH) {
            vga_device.cursor.col = 0;
            vga_device.cursor.row++;
            if (vga_device.cursor.row >= VGA_TEXT_HEIGHT)
                scrollup(1);
        }

        entry = make_entry(c, vga_device.color);
        vga_device.buf[vga_device.cursor.row * VGA_TEXT_WIDTH +
                       vga_device.cursor.col] = entry;
        vga_device.cursor.col++;
    }

    cursor_sync();
}