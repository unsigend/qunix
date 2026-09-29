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

/* Internal implementation of printf family of functions. This source file
   should be a freestanding implementation of the core printf, which is
   independent of the standard library and no buffer related manipulation. */

#include "fmt.h"

#include <kernel/ctype.h>
#include <kernel/macros.h>
#include <kernel/stdarg.h>
#include <kernel/stdbool.h>
#include <kernel/stddef.h>
#include <kernel/stdint.h>
#include <kernel/string.h>
#include <kernel/types.h>

/* Features supported:
   Specifiers: %c, %s, %d, %i, %o, %x, %X, %u, %p
   Flags:
      - left alignment: -
      - zero padding:   0
      - 0x prefix:      # (%x, %X, %o)
   Width:      fixed %Nd, dynamic %*d
   Precision:  fixed %.Nd, dynamic %.*d
   Length:     l (long), ll (long long), z (size_t), h (short), hh (char)
   Format sequence is ordered: %[flags][width][precision][length][specifier]
*/

#define INT_BUFFSZ                                                             \
    32 /* buffer size for integer conversion, enough for 64-bit integer */

/* Output context */
struct ctx {
    unsigned char *buff; /* destination buffer */
    size_t bufsz;        /* destination buffer size */
    size_t pos;          /* current write position in buff */
    int total;           /* total bytes written */
};

#define BUFF_IS_FULL(ctx) (((ctx)->pos >= (ctx)->bufsz)) /* buffer is full */
#define BUFF_LEFT(ctx) (((ctx)->bufsz - (ctx)->pos))     /* buffer left size */

/* Argument pack used for pop arguments from stack */
union arg {
    uintmax_t i; /* integer */
    void *p;     /* pointer */
};

/* Finite state machine */
struct fsm_t {
    int state;        /* current state */
    int flags;        /* flags */
    int length;       /* length */
    size_t width;     /* width */
    size_t precision; /* precision */
};

/* Write an unsigned integer to the buffer, without sign handling and NULL
   terminator. Return the number of characters written. If the buff is NULL,
   no write will be performed just return the number of characters that would
   be written. */

static size_t itoa(unsigned char *buff, uintmax_t val, int base, bool upper);

/* Raw output functions, no padding, formatting or sign handling, just write to
   the context buffer and update the context */
static void rawoutc(struct ctx *ctx, unsigned char ch);
static void rawouts(struct ctx *ctx, const unsigned char *str, size_t len);
static inline void rawpad(struct ctx *ctx, int len, unsigned char pad);

/* Formatting functions, apply padding, formatting, prefix, width, precision
   etc. This function is used as a middle abstraction layer. */
static void emit(struct ctx *ctx, struct fsm_t *fsm,
                 const unsigned char *prefix, size_t prefixlen,
                 const unsigned char *body, size_t bodylen, size_t fillinglen);

/* Write a number or string to the context based on the finite state machine */
static void outnum(struct ctx *ctx, struct fsm_t *fsm, uintmax_t val, int base,
                   bool upper, const unsigned char *prefix, size_t prefixlen);
static void outs(struct ctx *ctx, struct fsm_t *fsm, const unsigned char *str,
                 size_t len);

#define PAD_ZERO 0x01   /* zero padding flag */
#define LEFT_ALIGN 0x02 /* left align flag */
#define PREFIX 0x04     /* prefix flag */
#define PRECISION 0x08  /* precision flag */

#define IS_PAD_ZERO(flag)                                                      \
    ((flag) & PAD_ZERO) /* check if padding zero flag is set */
#define IS_LEFT_ALIGN(flag)                                                    \
    ((flag) & LEFT_ALIGN) /* check if left align flag is set */
#define HAS_PREFIX(flag)                                                       \
    ((flag) & PREFIX) /* check if prefix flag is set                           \
                       */
#define HAS_PRECISION(flag)                                                    \
    ((flag) & PRECISION) /* check if precision flag is set                     \
                          */
/* Reset the finite state machine */
#define FSM_RESET(fsm)                                                         \
    do {                                                                       \
        memset(fsm, 0, sizeof(struct fsm_t));                                  \
        (fsm)->state = STATE_NORMAL;                                           \
    } while (0)
#define FSM_SWITCH(fsm, newstate)                                              \
    ((fsm)->state = (newstate)) /* switch to new state */
#define FSM_SET_FLAG(fsm, flag) ((fsm)->flags |= (flag))    /* set flag */
#define FSM_CLEAR_FLAG(fsm, flag) ((fsm)->flags &= ~(flag)) /* clear flag */

static size_t itoa(unsigned char *buff, uintmax_t val, int base, bool upper)
{
    size_t len = 0;
    do {
        int d = val % base;
        unsigned char cw = d < 10  ? '0' + d
                           : upper ? 'A' + d - 10
                                   : 'a' + d - 10;
        if (buff)
            buff[len] = cw;
        len++;
        val /= base;
    } while (val);

    if (buff) {
        /* reverse the buffer in place */
        for (size_t i = 0, j = len - 1; i < j; i++, j--) {
            unsigned char tmp = buff[i];
            buff[i] = buff[j];
            buff[j] = tmp;
        }
    }

    return len;
}

/* Write a single character to the buffer and update the context */
static void rawoutc(struct ctx *ctx, unsigned char ch)
{
    ctx->total++;
    if (!BUFF_IS_FULL(ctx))
        ctx->buff[ctx->pos++] = ch;
}

/* Write a string to the buffer and update the context, truncate if the string
   is longer than the buffer */
static void rawouts(struct ctx *ctx, const unsigned char *str, size_t len)
{
    size_t nw = MIN(len, BUFF_LEFT(ctx));
    ctx->total += len;
    if (nw) {
        memcpy(ctx->buff + ctx->pos, str, nw);
        ctx->pos += nw;
    }
}

/* Write padding to the buffer and update the context */
static inline void rawpad(struct ctx *ctx, int len, unsigned char pad)
{
    while (len-- > 0)
        rawoutc(ctx, pad);
}

/* Emit the prefix, body, fillings (zeros) and padding (spaces or zeros) to the
 * context */
static void emit(struct ctx *ctx, struct fsm_t *fsm,
                 const unsigned char *prefix, size_t prefixlen,
                 const unsigned char *body, size_t bodylen, size_t fillinglen)
{
    size_t total = prefixlen + fillinglen + bodylen;
    size_t padlen = fsm->width > total ? fsm->width - total : 0;

    /* left align */
    if (IS_LEFT_ALIGN(fsm->flags)) {
        /* Format: [prefix][fillings][body][padding(spaces)] */
        rawouts(ctx, prefix, prefixlen);
        rawpad(ctx, fillinglen, '0');
        rawouts(ctx, body, bodylen);
        rawpad(ctx, padlen, ' ');
    }
    /* right align */
    else {
        if (HAS_PRECISION(fsm->flags)) {
            /* Format: [padding(spaces)][prefix][fillings][body] */
            rawpad(ctx, padlen, ' ');
            rawouts(ctx, prefix, prefixlen);
            rawpad(ctx, fillinglen, '0');
            rawouts(ctx, body, bodylen);
        } else {
            if (IS_PAD_ZERO(fsm->flags)) {
                /* Format: [prefix][padding(zeros)][body] */
                rawouts(ctx, prefix, prefixlen);
                rawpad(ctx, padlen, '0');
                rawouts(ctx, body, bodylen);
            } else {
                /* Format: [padding(spaces)][prefix][body] */
                rawpad(ctx, padlen, ' ');
                rawouts(ctx, prefix, prefixlen);
                rawouts(ctx, body, bodylen);
            }
        }
    }
}

/* Write a number to the context, both the width and precision are minimum
   bounds, the actual length might be exceeded. */
static void outnum(struct ctx *ctx, struct fsm_t *fsm, uintmax_t val, int base,
                   bool upper, const unsigned char *prefix, size_t prefixlen)
{
    unsigned char buff[INT_BUFFSZ];
    size_t len;
    if (HAS_PRECISION(fsm->flags) && !fsm->precision && !val)
        len = 0;
    else
        len = itoa(buff, val, base, upper);
    size_t fillinglen = HAS_PRECISION(fsm->flags)
                            ? fsm->precision > len ? fsm->precision - len : 0
                            : 0;
    emit(ctx, fsm, prefix, prefixlen, buff, len, fillinglen);
}

/* Write a string to the context, width is the minimum, precision is the
   maximum length. */
static void outs(struct ctx *ctx, struct fsm_t *fsm, const unsigned char *str,
                 size_t len)
{
    if (HAS_PRECISION(fsm->flags)) {
        /* Truncate the string if necessary */
        size_t minlen = MIN(fsm->precision, len);
        FSM_CLEAR_FLAG(fsm, PAD_ZERO);
        emit(ctx, fsm, NULL, 0, str, minlen, 0);
    } else
        emit(ctx, fsm, NULL, 0, str, len, 0);
}

/* Get the signed argument type based on the length modifier */
static int signed_arg_type(int len)
{
    switch (len) {
    case LEN_HH: return ARG_CHAR;
    case LEN_H: return ARG_SHORT;
    case LEN_L: return ARG_LONG;
    case LEN_LL: return ARG_LLONG;
    case LEN_Z: return ARG_SSIZE_T;
    default: return ARG_INT;
    }
}

/* Get the unsigned argument type based on the length modifier */
static int unsigned_arg_type(int len)
{
    switch (len) {
    case LEN_HH: return ARG_UCHAR;
    case LEN_H: return ARG_USHORT;
    case LEN_L: return ARG_ULONG;
    case LEN_LL: return ARG_ULLONG;
    case LEN_Z: return ARG_SIZE_T;
    default: return ARG_UINT;
    }
}

/* Pop arguments from the argument list based on promotion rules, the
   implementation is based on the reference implementation of the musl C
   library 1.2.5 */
static void pop_arg(union arg *arg, int type, va_list *ap)
{
    switch (type) {
    case ARG_PTR: arg->p = va_arg(*ap, void *); break;
    case ARG_SHORT: arg->i = va_arg(*ap, int); break;
    case ARG_USHORT: arg->i = va_arg(*ap, unsigned int); break;
    case ARG_CHAR: arg->i = va_arg(*ap, int); break;
    case ARG_UCHAR: arg->i = va_arg(*ap, unsigned int); break;
    case ARG_INT: arg->i = va_arg(*ap, int); break;
    case ARG_UINT: arg->i = va_arg(*ap, unsigned int); break;
    case ARG_LONG: arg->i = va_arg(*ap, long); break;
    case ARG_ULONG: arg->i = va_arg(*ap, unsigned long); break;
    case ARG_LLONG: arg->i = va_arg(*ap, long long); break;
    case ARG_ULLONG: arg->i = va_arg(*ap, unsigned long long); break;
    case ARG_IMAX: arg->i = va_arg(*ap, intmax_t); break;
    case ARG_UMAX: arg->i = va_arg(*ap, uintmax_t); break;
    case ARG_SIZE_T: arg->i = va_arg(*ap, size_t); break;
    case ARG_SSIZE_T: arg->i = va_arg(*ap, ssize_t); break;
    }
}

int printf_core(char *restrict buf, size_t bufsz, const char *restrict fmt,
                va_list vlist)
{
    struct ctx ctx = {
        .buff = (unsigned char *)buf,
        .pos = 0,
        .total = 0,
    };
    struct fsm_t fsm;
    union arg arg;
    va_list ap;

    /* the copy allows passing va_list* even if va_list is an array */
    va_copy(ap, vlist);

    FSM_RESET(&fsm);

    if (!ctx.buff || !bufsz)
        ctx.bufsz = 0;
    else
        ctx.bufsz = bufsz - 1;

    while (fmt && *fmt) {
        switch (fsm.state) {
        case STATE_NORMAL:
            if (*fmt == '%') {
                FSM_RESET(&fsm);
                FSM_SWITCH(&fsm, STATE_FORMAT);
            } else
                rawoutc(&ctx, *fmt);
            break;
        case STATE_FORMAT:
            switch (*fmt) {
            case '%':
                rawoutc(&ctx, '%');
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            /* specifiers */
            case SPEC_C:
                pop_arg(&arg, ARG_CHAR, &ap);
                outs(&ctx, &fsm, (unsigned char *)&arg.i, 1);
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_S:
                pop_arg(&arg, ARG_PTR, &ap);
                if (!arg.p)
                    outs(&ctx, &fsm, (unsigned char *)"(null)", 6);
                else
                    outs(&ctx, &fsm, (unsigned char *)arg.p,
                         strlen((char *)arg.p));
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_I:
            case SPEC_D:
                pop_arg(&arg, signed_arg_type(fsm.length), &ap);
                if ((intmax_t)arg.i < 0)
                    outnum(&ctx, &fsm, -(uintmax_t)(intmax_t)arg.i, 10, false,
                           (unsigned char *)"-", 1);
                else
                    outnum(&ctx, &fsm, arg.i, 10, false, NULL, 0);

                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_O:
                pop_arg(&arg, unsigned_arg_type(fsm.length), &ap);
                if (HAS_PREFIX(fsm.flags)) {
                    if (arg.i != 0) {
                        size_t len = itoa(NULL, arg.i, 8, false);
                        if (HAS_PRECISION(fsm.flags) &&
                            fsm.precision >= len + 1)
                            outnum(&ctx, &fsm, arg.i, 8, false, NULL, 0);
                        else
                            outnum(&ctx, &fsm, arg.i, 8, false,
                                   (unsigned char *)"0", 1);
                    } else
                        emit(&ctx, &fsm, NULL, 0, (unsigned char *)"0", 1, 0);
                } else
                    outnum(&ctx, &fsm, arg.i, 8, false, NULL, 0);
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_X:
                pop_arg(&arg, unsigned_arg_type(fsm.length), &ap);
                if (HAS_PREFIX(fsm.flags) && arg.i != 0)
                    outnum(&ctx, &fsm, arg.i, 16, false, (unsigned char *)"0x",
                           2);
                else
                    outnum(&ctx, &fsm, arg.i, 16, false, NULL, 0);
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_XX:
                pop_arg(&arg, unsigned_arg_type(fsm.length), &ap);
                if (HAS_PREFIX(fsm.flags) && arg.i != 0)
                    outnum(&ctx, &fsm, arg.i, 16, true, (unsigned char *)"0X",
                           2);
                else
                    outnum(&ctx, &fsm, arg.i, 16, true, NULL, 0);
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_U:
                pop_arg(&arg, unsigned_arg_type(fsm.length), &ap);
                outnum(&ctx, &fsm, arg.i, 10, false, NULL, 0);
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            case SPEC_P:
                pop_arg(&arg, ARG_PTR, &ap);
                if (arg.p) /* for %p specifier the prefix is always 0x */
                    outnum(&ctx, &fsm, (uintmax_t)(uintptr_t)arg.p, 16, false,
                           (unsigned char *)"0x", 2);
                else
                    outs(&ctx, &fsm, (unsigned char *)"(nil)", 5);
                FSM_SWITCH(&fsm, STATE_NORMAL);
                break;
            /* flags */
            case '-': FSM_SET_FLAG(&fsm, LEFT_ALIGN); break;
            case '0': FSM_SET_FLAG(&fsm, PAD_ZERO); break;
            case '#': FSM_SET_FLAG(&fsm, PREFIX); break;
            /* length modifiers */
            case 'h':
                if (*(fmt + 1) == 'h') {
                    fsm.length = LEN_HH;
                    fmt++;
                } else
                    fsm.length = LEN_H;
                break;
            case 'l':
                if (*(fmt + 1) == 'l') {
                    fsm.length = LEN_LL;
                    fmt++;
                } else
                    fsm.length = LEN_L;
                break;
            case 'z': fsm.length = LEN_Z; break;
            /* width or precision */
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                while (isdigit(*fmt)) {
                    if (HAS_PRECISION(fsm.flags))
                        fsm.precision = fsm.precision * 10 + (*fmt++ - '0');
                    else
                        fsm.width = fsm.width * 10 + (*fmt++ - '0');
                }
                fmt--;
                break;
            case '.':
                FSM_SET_FLAG(&fsm, PRECISION);
                fsm.precision = 0;
                break;
            case '*':
                pop_arg(&arg, ARG_INT, &ap);
                int wp = (int)(arg.i);
                if (HAS_PRECISION(fsm.flags)) {
                    /* If the * yields a negative value in precision, ignore
                       it */
                    if (wp >= 0)
                        fsm.precision = wp;
                } else {
                    if (wp < 0) {
                        /* If (*) yields a negative value in width, treats it
                           as (-) flag with a width of the absolute value */
                        FSM_SET_FLAG(&fsm, LEFT_ALIGN);
                        fsm.width = -wp;
                    } else
                        fsm.width = wp;
                }
                break;
            }
        }

        fmt++;
    }

    if (buf && bufsz)
        ctx.buff[MIN(ctx.pos, ctx.bufsz)] = '\0';

    va_end(ap);
    return ctx.total;
}