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

#ifndef _KBD_SCANCODE_H_
#define _KBD_SCANCODE_H_

/*
 * Scan code set 1. Release is make | SC1_BREAK.
 * 0xE0 / 0xE1 prefixes start a multi-byte sequence (second IRQ).
 */

#define SC1_BREAK 0x80 /* Release: make | this */
#define SC1_EXT 0xE0   /* Extended prefix */
#define SC1_PAUSE 0xE1 /* Pause prefix */

#define SC1_ESC 0x01 /* Escape */
#define SC1_1 0x02
#define SC1_2 0x03
#define SC1_3 0x04
#define SC1_4 0x05
#define SC1_5 0x06
#define SC1_6 0x07
#define SC1_7 0x08
#define SC1_8 0x09
#define SC1_9 0x0A
#define SC1_0 0x0B
#define SC1_MINUS 0x0C /* - */
#define SC1_EQUAL 0x0D /* = */
#define SC1_BS 0x0E    /* Backspace */
#define SC1_TAB 0x0F

#define SC1_Q 0x10
#define SC1_W 0x11
#define SC1_E 0x12
#define SC1_R 0x13
#define SC1_T 0x14
#define SC1_Y 0x15
#define SC1_U 0x16
#define SC1_I 0x17
#define SC1_O 0x18
#define SC1_P 0x19
#define SC1_LBRACKET 0x1A /* [ */
#define SC1_RBRACKET 0x1B /* ] */
#define SC1_ENTER 0x1C
#define SC1_LCTRL 0x1D
#define SC1_A 0x1E
#define SC1_S 0x1F

#define SC1_D 0x20
#define SC1_F 0x21
#define SC1_G 0x22
#define SC1_H 0x23
#define SC1_J 0x24
#define SC1_K 0x25
#define SC1_L 0x26
#define SC1_SEMI 0x27  /* ; */
#define SC1_QUOTE 0x28 /* ' */
#define SC1_GRAVE 0x29 /* ` */
#define SC1_LSHIFT 0x2A
#define SC1_BSLASH 0x2B /* \ */
#define SC1_Z 0x2C
#define SC1_X 0x2D
#define SC1_C 0x2E
#define SC1_V 0x2F

#define SC1_B 0x30
#define SC1_N 0x31
#define SC1_M 0x32
#define SC1_COMMA 0x33
#define SC1_DOT 0x34
#define SC1_SLASH 0x35
#define SC1_RSHIFT 0x36
#define SC1_KP_MUL 0x37 /* Keypad * */
#define SC1_LALT 0x38
#define SC1_SPACE 0x39
#define SC1_CAPS 0x3A
#define SC1_F1 0x3B
#define SC1_F2 0x3C
#define SC1_F3 0x3D
#define SC1_F4 0x3E
#define SC1_F5 0x3F

#define SC1_F6 0x40
#define SC1_F7 0x41
#define SC1_F8 0x42
#define SC1_F9 0x43
#define SC1_F10 0x44
#define SC1_NUMLOCK 0x45
#define SC1_SCROLL 0x46
#define SC1_KP_7 0x47
#define SC1_KP_8 0x48
#define SC1_KP_9 0x49
#define SC1_KP_MINUS 0x4A
#define SC1_KP_4 0x4B
#define SC1_KP_5 0x4C
#define SC1_KP_6 0x4D
#define SC1_KP_PLUS 0x4E
#define SC1_KP_1 0x4F

#define SC1_KP_2 0x50
#define SC1_KP_3 0x51
#define SC1_KP_0 0x52
#define SC1_KP_DOT 0x53
#define SC1_F11 0x57
#define SC1_F12 0x58

/* Extended (byte after SC1_EXT). Release is make | SC1_BREAK. */

#define SC1_E0_PREVTRACK 0x10 /* Multimedia */
#define SC1_E0_NEXTTRACK 0x19
#define SC1_E0_KP_ENTER 0x1C
#define SC1_E0_RCTRL 0x1D
#define SC1_E0_MUTE 0x20
#define SC1_E0_CALC 0x21
#define SC1_E0_PLAY 0x22
#define SC1_E0_STOP 0x24
#define SC1_E0_VOLDN 0x2E
#define SC1_E0_VOLUP 0x30
#define SC1_E0_WWW_HOME 0x32
#define SC1_E0_KP_SLASH 0x35
#define SC1_E0_RALT 0x38
#define SC1_E0_HOME 0x47
#define SC1_E0_UP 0x48
#define SC1_E0_PGUP 0x49
#define SC1_E0_LEFT 0x4B
#define SC1_E0_RIGHT 0x4D
#define SC1_E0_END 0x4F
#define SC1_E0_DOWN 0x50
#define SC1_E0_PGDN 0x51
#define SC1_E0_INS 0x52
#define SC1_E0_DEL 0x53
#define SC1_E0_LGUI 0x5B
#define SC1_E0_RGUI 0x5C
#define SC1_E0_APPS 0x5D
#define SC1_E0_POWER 0x5E
#define SC1_E0_SLEEP 0x5F
#define SC1_E0_WAKE 0x63
#define SC1_E0_WWW_SEARCH 0x65
#define SC1_E0_WWW_FAV 0x66
#define SC1_E0_WWW_REFRESH 0x67
#define SC1_E0_WWW_STOP 0x68
#define SC1_E0_WWW_FWD 0x69
#define SC1_E0_WWW_BACK 0x6A
#define SC1_E0_MYCOMP 0x6B
#define SC1_E0_EMAIL 0x6C
#define SC1_E0_MEDIA 0x6D

#endif /* _KBD_SCANCODE_H_ */
