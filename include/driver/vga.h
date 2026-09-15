/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef DRIVER_VGA_H
#define DRIVER_VGA_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFBASE 0xB8000

/* VGA colors */
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_YELLOW 14
#define VGA_COLOR_WHITE 15

/* Initialize vga driver and mount it to the tty adapter, the base address
 * specify the base vga buffer address by default is 0xB8000. */
extern void vga_init(void *base);

/* Remap the vga buffer to a new base address, this function should be called
 * after paging and virtual memory is enabled. */
extern void vga_remap(void *newbase);

#endif