/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm.h>
#include <driver/vga.h>
#include <qunix/tty.h>
#include <stdint.h>
#include <string.h>

#define VGA_IDX_REG 0x3D4  /* Register for selecting port */
#define VGA_DATA_REG 0x3D5 /* Register for write/read data */

/* VGA registers */
#define VGA_CURSOR_LOC_HIGH 0x0E /* Cursor location high byte */
#define VGA_CURSOR_LOC_LOW 0x0F  /* Cursor location low byte */
#define VGA_CURSOR_START 0x0A    /* Cursor start scan line */
#define VGA_CURSOR_END 0x0B      /* Cursor end scan line */

#define VGA_CURSOR_BLOCK 0x000F
#define VGA_CURSOR_UNDERLINE 0x0D0F

struct vga_dev {
    uint16_t *buf; /* vga cell is 2 bytes */
};

static struct vga_dev vga_dev;

static void update_cursor(void *data, uint32_t x, uint32_t y)
{
    (void)data;
    uint16_t pos = y * VGA_WIDTH + x;

    outb(VGA_IDX_REG, VGA_CURSOR_LOC_LOW);
    outb(VGA_DATA_REG, (uint8_t)(pos & 0xFF));

    outb(VGA_IDX_REG, VGA_CURSOR_LOC_HIGH);
    outb(VGA_DATA_REG, (uint8_t)((pos >> 8) & 0xFF));
}

static void putc(void *data, uint32_t x, uint32_t y, struct tty_color color,
                 char c)
{
    uint8_t attr = color.fg | color.bg << 4;
    struct vga_dev *dev = (struct vga_dev *)data;
    dev->buf[y * VGA_WIDTH + x] = (uint16_t)(uint8_t)c | ((uint16_t)attr << 8);
}

static void clear(void *data, struct tty_color color)
{
    for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        putc(data, i % VGA_WIDTH, i / VGA_WIDTH, color, ' ');
}

static void scroll(void *data, uint32_t lines, struct tty_color color)
{
    if (lines == 0)
        return;
    if (lines >= VGA_HEIGHT) {
        clear(data, color);
        return;
    }
    struct vga_dev *dev = (struct vga_dev *)data;
    memmove(dev->buf, dev->buf + lines * VGA_WIDTH,
            (VGA_HEIGHT - lines) * VGA_WIDTH * sizeof(uint16_t));
    /* fill the remaining lines with the color */
    for (uint32_t i = (VGA_HEIGHT - lines); i < VGA_HEIGHT; i++)
        for (uint32_t j = 0; j < VGA_WIDTH; j++)
            putc(data, j, i, color, ' ');
}

static void enable_cursor(uint16_t style)
{
    uint8_t start = (style >> 8) & 0xFF;
    uint8_t end = style & 0xFF;

    outb(VGA_IDX_REG, VGA_CURSOR_START);
    outb(VGA_DATA_REG, (inb(VGA_DATA_REG) & 0xC0) | start);

    outb(VGA_IDX_REG, VGA_CURSOR_END);
    outb(VGA_DATA_REG, (inb(VGA_DATA_REG) & 0xE0) | end);
}

__attribute__((unused)) static void disable_cursor(void)
{
    outb(VGA_IDX_REG, VGA_CURSOR_START);
    outb(VGA_DATA_REG, 0x20);
}

static struct tty_ops vga_ops = {
    .width = VGA_WIDTH,
    .height = VGA_HEIGHT,

    .putc = putc,
    .clear = clear,
    .scroll = scroll,
    .update_cursor = update_cursor,
};

void vga_init(void *base)
{
    vga_dev.buf = base;
    enable_cursor(VGA_CURSOR_UNDERLINE);
    tty_register(&vga_ops, (void *)&vga_dev);
}

void vga_remap(void *newbase) { vga_dev.buf = newbase; }