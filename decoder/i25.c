/*------------------------------------------------------------------------
 *  Copyright 2008 (c) Jeff Brown <spadix@users.sourceforge.net>
 *
 *  This file is part of the Zebra Barcode Library.
 *
 *  The Zebra Barcode Library is free software; you can redistribute it
 *  and/or modify it under the terms of the GNU Lesser Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  The Zebra Barcode Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser Public License
 *  along with the Zebra Barcode Library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 *  Boston, MA  02110-1301  USA
 *
 *  http://sourceforge.net/projects/zebra
 *------------------------------------------------------------------------*/

#include <config.h>
#include <string.h>     /* memmove */

#include <zebra.h>
#include "decoder.h"

#ifdef DEBUG_I25
# define DEBUG_LEVEL (DEBUG_I25)
#endif
#include "debug.h"

static inline unsigned char i25_decode1 (unsigned char enc,
                                         unsigned e,
                                         unsigned s)
{
    unsigned char E = decode_e(e, s, 45);
    if(E > 7)
        return(0xff);
    enc <<= 1;
    if(E > 2)
        enc |= 1;
    return(enc);
}

static inline unsigned char i25_decode10 (zebra_decoder_t *dcode,
                                          unsigned char offset)
{
    i25_decoder_t *dcode25 = &dcode->i25;
    dprintf(2, " s=%d", dcode25->s10);
    if(dcode25->s10 < 10)
        return(0xff);

    /* threshold bar width ratios */
    unsigned char enc = 0, par = 0;
    signed char i;
    for(i = 8; i >= 0; i -= 2) {
        unsigned char j = offset + ((dcode25->direction) ? i : 8 - i);
        enc = i25_decode1(enc, get_width(dcode, j), dcode25->s10);
        if(enc == 0xff)
            return(0xff);
        if(enc & 1)
            par++;
    }

    dprintf(2, " enc=%02x par=%x", enc, par);

    /* parity check */
    if(par != 2) {
        dprintf(2, " [bad parity]");
        return(0xff);
    }

    /* decode binary weights */
    enc &= 0xf;
    if(enc & 8) {
        if(enc == 12)
            enc = 0;
        else if(--enc > 9) {
            dprintf(2, " [invalid encoding]");
            return(0xff);
        }
    }

    dprintf(2, " => %x", enc);
    return(enc);
}

static inline signed char i25_decode_start (zebra_decoder_t *dcode)
{
    i25_decoder_t *dcode25 = &dcode->i25;
    if(dcode25->s10 < 10)
        return(ZEBRA_NONE);

    unsigned char enc = 0;
    unsigned char i = 10;
    enc = i25_decode1(enc, get_width(dcode, i++), dcode25->s10);
    enc = i25_decode1(enc, get_width(dcode, i++), dcode25->s10);
    enc = i25_decode1(enc, get_width(dcode, i++), dcode25->s10);

    if((get_color(dcode) == ZEBRA_BAR)
       ? enc != 4
       : i25_decode1(enc, get_width(dcode, i++), dcode25->s10))
        return(ZEBRA_NONE);

    /* check leading quiet zone - spec is 10x(?), we require at least 7x */
    unsigned quiet = get_width(dcode, i++);
    if(quiet && get_width(dcode, i++) &&
       quiet < dcode25->s10 / 4)
        return(ZEBRA_NONE);

    dcode25->direction = get_color(dcode);
    dcode25->element = 1;
    dcode25->character = 0;
    return(ZEBRA_PARTIAL);
}

static inline signed char i25_decode_end (zebra_decoder_t *dcode)
{
    i25_decoder_t *dcode25 = &dcode->i25;

    /* check trailing quiet zone */
    if(get_width(dcode, 0) < dcode25->width / 4 ||
       decode_e(get_width(dcode, 1), dcode25->width, 45) > 2 ||
       decode_e(get_width(dcode, 2), dcode25->width, 45) > 2)
        return(ZEBRA_NONE);

    /* check exit condition */
    unsigned char E = decode_e(get_width(dcode, 3), dcode25->width, 45);
    if((!dcode25->direction)
       ? E - 3 > 4
       : (E > 2 ||
          decode_e(get_width(dcode, 4), dcode25->width, 45) > 2))
        return(ZEBRA_NONE);

    if(dcode25->direction) {
        /* reverse buffer */
        dprintf(2, " (rev)");
        int i;
        for(i = 0; i < dcode25->character / 2; i++) {
            unsigned j = dcode25->character - 1 - i;
            char c = dcode->buf[i];
            dcode->buf[i] = dcode->buf[j];
            dcode->buf[j] = c;
        }
    }

    dcode->buf[dcode25->character] = '\0';
    dprintf(2, " [valid end]\n");
    dcode25->character = -1;
    return(ZEBRA_I25);
}

zebra_symbol_type_t _zebra_decode_i25 (zebra_decoder_t *dcode)
{
    i25_decoder_t *dcode25 = &dcode->i25;

    /* update latest character width */
    dcode25->s10 -= get_width(dcode, 10);
    dcode25->s10 += get_width(dcode, 0);

    if(dcode25->character < 0 &&
       !i25_decode_start(dcode))
        return(ZEBRA_NONE);

    if(--dcode25->element == 6 - dcode25->direction)
        return(i25_decode_end(dcode));
    else if(dcode25->element)
        return(ZEBRA_NONE);

    /* FIXME check current character width against previous */
    dcode25->width = dcode25->s10;

    dprintf(2, "      i25[%c%02d+%x]",
            (dcode25->direction) ? '<' : '>',
            dcode25->character, dcode25->element);

    /* lock shared resources */
    if(!dcode25->character && get_lock(dcode)) {
        dcode25->character = -1;
        dprintf(2, " [locked]\n");
        return(ZEBRA_PARTIAL);
    }

    unsigned char c = i25_decode10(dcode, 1);
    dprintf(2, " c=%x", c);

    if(c > 9 ||
       ((dcode25->character >= BUFFER_MIN) &&
        size_buf(dcode, dcode25->character + 2))) {
        dprintf(2, (c > 9) ? " [aborted]\n" : " [overflow]\n");
        dcode->lock = 0;
        dcode25->character = -1;
        return(ZEBRA_NONE);
    }
    dcode->buf[dcode25->character++] = c + '0';

    c = i25_decode10(dcode, 0);
    dprintf(2, " c=%x", c);
    if(c > 9) {
        dprintf(2, " [aborted]\n");
        dcode->lock = 0;
        dcode25->character = -1;
        return(ZEBRA_NONE);
    }
    else
        dprintf(2, "\n");

    dcode->buf[dcode25->character++] = c + '0';
    dcode25->element = 10;
    return((dcode25->character == 2) ? ZEBRA_PARTIAL : ZEBRA_NONE);
}
