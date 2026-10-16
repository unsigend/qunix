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

#ifndef _VGA_VGA_H_
#define _VGA_VGA_H_

#include <kernel/stdint.h>

#define VGA_CR_IDX 0x3D4  /* Control register index */
#define VGA_CR_DATA 0x3D5 /* Control register data */

/* Horizontal timing */

#define VGA_CR00 0x00 /* horizontal total register */
#define VGA_CR01 0x01 /* end horizontal display register */
#define VGA_CR02 0x02 /* start horizontal blanking register */
#define VGA_CR03 0x03 /* end horizontal blanking register */
#define VGA_CR04 0x04 /* start horizontal retrace register */
#define VGA_CR05 0x05 /* end horizontal retrace register */

/* Vertical timing */

#define VGA_CR06 0x06 /* vertical total register */
#define VGA_CR07 0x07 /* overflow register */
#define VGA_CR09 0x09 /* maximum scan line register */
#define VGA_CR10 0x10 /* vertical retrace start register */
#define VGA_CR11 0x11 /* vertical retrace end register */
#define VGA_CR12 0x12 /* vertical display end register */
#define VGA_CR15 0x15 /* vertical blanking start register */
#define VGA_CR16 0x16 /* vertical blanking end register */

/* Cursor registers */

#define VGA_CR0A 0x0A /* cursor start register */
#define VGA_CR0B 0x0B /* cursor end register */
#define VGA_CR0E 0x0E /* cursor location high */
#define VGA_CR0F 0x0F /* cursor location low */

#define VGA_SR_IDX 0x3C4  /* Sequencer register index */
#define VGA_SR_DATA 0x3C5 /* Sequencer register data */

#define VGA_GR_IDX 0x3CE  /* Graphics register index */
#define VGA_GR_DATA 0x3CF /* Graphics register data */

#define VGA_MISC_WRITE 0x3C2 /* Miscellaneous write register */
#define VGA_MISC_READ 0x3CC  /* Miscellaneous read register */

enum vga_color {
    VGA_COLOR_BLACK = 0x0,
    VGA_COLOR_BLUE = 0x1,
    VGA_COLOR_GREEN = 0x2,
    VGA_COLOR_CYAN = 0x3,
    VGA_COLOR_RED = 0x4,
    VGA_COLOR_MAGENTA = 0x5,
    VGA_COLOR_BROWN = 0x6,
    VGA_COLOR_LIGHT_GREY = 0x7,
    VGA_COLOR_DARK_GREY = 0x8,
    VGA_COLOR_LIGHT_BLUE = 0x9,
    VGA_COLOR_LIGHT_GREEN = 0xA,
    VGA_COLOR_LIGHT_CYAN = 0xB,
    VGA_COLOR_LIGHT_RED = 0xC,
    VGA_COLOR_LIGHT_MAGENTA = 0xD,
    VGA_COLOR_YELLOW = 0xE, /* light brown */
    VGA_COLOR_WHITE = 0xF,
};

/* qunix only supports 80x25 text mode, no graphics mode */
#define VGA_TEXT_BUFBASE 0xB8000
#define VGA_TEXT_WIDTH 80
#define VGA_TEXT_HEIGHT 25
#define VGA_TEXT_BUFSIZE (VGA_TEXT_WIDTH * VGA_TEXT_HEIGHT * sizeof(uint16_t))

struct vga_state {
    volatile uint16_t *buf;
    uint8_t color;
    struct {
        uint8_t row;
        uint8_t col;
    } cursor;
};

#endif /* _VGA_VGA_H_ */