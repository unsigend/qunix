/* qlibc - A light-weight and portable C standard library
 * Copyright (C) 2025 Qiu Yixiang
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

#ifndef _CTYPE_H_
#define _CTYPE_H_ 1

#include <feature.h>

__BEGIN_DECLS

extern int isalnum(int c);
extern int isalpha(int c);
extern int isdigit(int c);
extern int isxdigit(int c);
extern int isblank(int c);
extern int iscntrl(int c);
extern int isspace(int c);
extern int ispunct(int c);
extern int isgraph(int c);
extern int isprint(int c);
extern int islower(int c);
extern int isupper(int c);
extern int tolower(int c);
extern int toupper(int c);

__END_DECLS

#endif