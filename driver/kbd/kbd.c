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

#include "i8042.h"
#include "scancode.h"

#include <asm/io.h>
#include <asm/irq.h>

#include <kernel/compiler.h>
#include <kernel/irq.h>
#include <kernel/panic.h>
#include <kernel/stdint.h>
#include <kernel/tty.h>

#include <driver/kbd.h>

static const char sc1_ascii_map[128] = {
    [SC1_ESC] = '\033', [SC1_1] = '1',        [SC1_2] = '2',
    [SC1_3] = '3',      [SC1_4] = '4',        [SC1_5] = '5',
    [SC1_6] = '6',      [SC1_7] = '7',        [SC1_8] = '8',
    [SC1_9] = '9',      [SC1_0] = '0',        [SC1_MINUS] = '-',
    [SC1_EQUAL] = '=',  [SC1_BS] = '\b',      [SC1_TAB] = '\t',
    [SC1_Q] = 'q',      [SC1_W] = 'w',        [SC1_E] = 'e',
    [SC1_R] = 'r',      [SC1_T] = 't',        [SC1_Y] = 'y',
    [SC1_U] = 'u',      [SC1_I] = 'i',        [SC1_O] = 'o',
    [SC1_P] = 'p',      [SC1_LBRACKET] = '[', [SC1_RBRACKET] = ']',
    [SC1_ENTER] = '\n', [SC1_A] = 'a',        [SC1_S] = 's',
    [SC1_D] = 'd',      [SC1_F] = 'f',        [SC1_G] = 'g',
    [SC1_H] = 'h',      [SC1_J] = 'j',        [SC1_K] = 'k',
    [SC1_L] = 'l',      [SC1_SEMI] = ';',     [SC1_QUOTE] = '\'',
    [SC1_GRAVE] = '`',  [SC1_BSLASH] = '\\',  [SC1_Z] = 'z',
    [SC1_X] = 'x',      [SC1_C] = 'c',        [SC1_V] = 'v',
    [SC1_B] = 'b',      [SC1_N] = 'n',        [SC1_M] = 'm',
    [SC1_COMMA] = ',',  [SC1_DOT] = '.',      [SC1_SLASH] = '/',
    [SC1_SPACE] = ' ',
};

static char sc1_to_ascii(uint8_t make)
{
    if (make & SC1_BREAK)
        return 0;
    if (make >= sizeof(sc1_ascii_map))
        return 0;

    return sc1_ascii_map[make];
}

static irqreturn_t kbd_handler(unsigned int irq, void *dev)
{
    uint8_t sc;
    char c;

    UNUSED(irq);
    UNUSED(dev);

    sc = inb(PS2_REG_DATA);
    if (sc == SC1_EXT || sc == 0)
        return IRQ_HANDLED;

    c = sc1_to_ascii(sc);
    if (c)
        tty_input(c);

    return IRQ_HANDLED;
}

void kbd_init(void)
{
    int err;

    if ((err = irq_register(IRQ_KBD, kbd_handler, 0, "PS/2 Keyboard", NULL)))
        panic_error(err, "failed to register PS/2 keyboard IRQ");
}
