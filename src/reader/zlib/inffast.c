/* inffast.c -- fast decoding
 * Copyright (C) 1995-2004 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zutil.h"
#include "inftrees.h"
#include "inflate.h"
#include "inffast.h"

#ifdef __i386__

void inflate_fast(strm, start)
z_streamp strm;
unsigned start;         /* inflate()'s starting value for strm->avail_out */
{
    struct inflate_state FAR *state;
    struct inffast_ar {
      void *esp;                  /* esp save */
      unsigned char FAR *in;      /* local strm->next_in */
      unsigned char FAR *last;    /* while in < last, enough input available */
      unsigned char FAR *out;     /* local strm->next_out */
      unsigned char FAR *beg;     /* inflate()'s initial strm->next_out */
      unsigned char FAR *end;     /* while out < end, enough space available */
      unsigned wsize;             /* window size or zero if not using window */
      unsigned write;             /* window write index */
      unsigned char FAR *window;  /* allocated sliding window, if wsize != 0 */
      unsigned long hold;         /* local strm->hold */
      unsigned bits;              /* local strm->bits */
      code const FAR *lcode;      /* local strm->lencode */
      code const FAR *dcode;      /* local strm->distcode */
      unsigned lmask;             /* mask for first level of length codes */
      unsigned dmask;             /* mask for first level of distance codes */
      unsigned len;               /* match length, unused bytes */
      unsigned dist;              /* match distance */
      unsigned status;            /* this is set when state changes */
    } ar;

    /* copy state to local variables */
    state = (struct inflate_state FAR *)strm->state;
    ar.in = strm->next_in;
    ar.last = ar.in + (strm->avail_in - 5);
    ar.out = strm->next_out;
    ar.beg = ar.out - (start - strm->avail_out);
    ar.end = ar.out + (strm->avail_out - 257);
    ar.wsize = state->wsize;
    ar.write = state->write;
    ar.window = state->window;
    ar.hold = state->hold;
    ar.bits = state->bits;
    ar.lcode = state->lencode;
    ar.dcode = state->distcode;
    ar.lmask = (1U << state->lenbits) - 1;
    ar.dmask = (1U << state->distbits) - 1;

    /* decode literals and length/distances until end-of-block or not enough
       input data or output space */

    /* align in on 2 byte boundary */
    if (((unsigned long)(void *)ar.in & 0x1) != 0) {
        ar.hold += (unsigned long)*ar.in++ << ar.bits;
        ar.bits += 8;
    }

        __asm__ __volatile__ (
"        leal    %0, %%eax\n"
"        pushf\n"
"        pushl   %%ebp\n"
"        movl    %%esp, (%%eax)\n"
"        movl    %%eax, %%esp\n"
"        movl    4(%%esp), %%esi\n"       /* esi = in */
"        movl    12(%%esp), %%edi\n"      /* edi = out */
"        movl    36(%%esp), %%edx\n"      /* edx = hold */
"        movl    40(%%esp), %%ebx\n"      /* ebx = bits */
"        movl    44(%%esp), %%ebp\n"      /* ebp = lcode */

"        cld\n"
"        jmp     .L_do_loop\n"

".L_while_test:\n"
"        cmpl    %%edi, 20(%%esp)\n"
"        jbe     .L_break_loop\n"
"        cmpl    %%esi, 8(%%esp)\n"
"        jbe     .L_break_loop\n"

".L_do_loop:\n"
"        cmpb    $15, %%bl\n"
"        ja      .L_get_length_code\n"    /* if (15 < bits) */

"        xorl    %%eax, %%eax\n"
"        lodsw\n"                         /* al = *(ushort *)in++ */
"        movb    %%bl, %%cl\n"            /* cl = bits, needs it for shifting */
"        addb    $16, %%bl\n"             /* bits += 16 */
"        shll    %%cl, %%eax\n"
"        orl     %%eax, %%edx\n"        /* hold |= *((ushort *)in)++ << bits */

".L_get_length_code:\n"
"        movl    52(%%esp), %%eax\n"      /* eax = lmask */
"        andl    %%edx, %%eax\n"          /* eax &= hold */
"        movl    (%%ebp,%%eax,4), %%eax\n" /* eax = lcode[hold & lmask] */

".L_dolen:\n"
"        movb    %%ah, %%cl\n"            /* cl = this.bits */
"        subb    %%ah, %%bl\n"            /* bits -= this.bits */
"        shrl    %%cl, %%edx\n"           /* hold >>= this.bits */

"        testb   %%al, %%al\n"
"        jnz     .L_test_for_length_base\n" /* if (op != 0) 45.7% */

"        shrl    $16, %%eax\n"            /* output this.val char */
"        stosb\n"
"        jmp     .L_while_test\n"

".L_test_for_length_base:\n"
"        movl    %%eax, %%ecx\n"          /* len = this */
"        shrl    $16, %%ecx\n"            /* len = this.val */
"        movl    %%ecx, 60(%%esp)\n"      /* len = this */
"        movb    %%al, %%cl\n"

"        testb   $16, %%al\n"
"        jz      .L_test_for_second_level_length\n" /* if ((op & 16) == 0) 8% */
"        andb    $15, %%cl\n"             /* op &= 15 */
"        jz      .L_decode_distance\n"    /* if (!op) */
"        cmpb    %%cl, %%bl\n"
"        jae     .L_add_bits_to_len\n"    /* if (op <= bits) */

"        movb    %%cl, %%ch\n"            /* stash op in ch, freeing cl */
"        xorl    %%eax, %%eax\n"
"        lodsw\n"                         /* al = *(ushort *)in++ */
"        movb    %%bl, %%cl\n"            /* cl = bits, needs it for shifting */
"        addb    $16, %%bl\n"             /* bits += 16 */
"        shll    %%cl, %%eax\n"
"        orl     %%eax, %%edx\n"         /* hold |= *((ushort *)in)++ << bits */
"        movb    %%ch, %%cl\n"            /* move op back to ecx */

".L_add_bits_to_len:\n"
"        movl    $1, %%eax\n"
"        shll    %%cl, %%eax\n"
"        decl    %%eax\n"
"        subb    %%cl, %%bl\n"
"        andl    %%edx, %%eax\n"          /* eax &= hold */
"        shrl    %%cl, %%edx\n"
"        addl    %%eax, 60(%%esp)\n"      /* len += hold & mask[op] */

".L_decode_distance:\n"
"        cmpb    $15, %%bl\n"
"        ja      .L_get_distance_code\n"  /* if (15 < bits) */

"        xorl    %%eax, %%eax\n"
"        lodsw\n"                         /* al = *(ushort *)in++ */
"        movb    %%bl, %%cl\n"            /* cl = bits, needs it for shifting */
"        addb    $16, %%bl\n"             /* bits += 16 */
"        shll    %%cl, %%eax\n"
"        orl     %%eax, %%edx\n"         /* hold |= *((ushort *)in)++ << bits */

".L_get_distance_code:\n"
"        movl    56(%%esp), %%eax\n"      /* eax = dmask */
"        movl    48(%%esp), %%ecx\n"      /* ecx = dcode */
"        andl    %%edx, %%eax\n"          /* eax &= hold */
"        movl    (%%ecx,%%eax,4), %%eax\n"/* eax = dcode[hold & dmask] */

".L_dodist:\n"
"        movl    %%eax, %%ebp\n"          /* dist = this */
"        shrl    $16, %%ebp\n"            /* dist = this.val */
"        movb    %%ah, %%cl\n"
"        subb    %%ah, %%bl\n"            /* bits -= this.bits */
"        shrl    %%cl, %%edx\n"           /* hold >>= this.bits */
"        movb    %%al, %%cl\n"            /* cl = this.op */

"        testb   $16, %%al\n"             /* if ((op & 16) == 0) */
"        jz      .L_test_for_second_level_dist\n"
"        andb    $15, %%cl\n"             /* op &= 15 */
"        jz      .L_check_dist_one\n"
"        cmpb    %%cl, %%bl\n"
"        jae     .L_add_bits_to_dist\n"   /* if (op <= bits) 97.6% */

"        movb    %%cl, %%ch\n"            /* stash op in ch, freeing cl */
"        xorl    %%eax, %%eax\n"
"        lodsw\n"                         /* al = *(ushort *)in++ */
"        movb    %%bl, %%cl\n"            /* cl = bits, needs it for shifting */
"        addb    $16, %%bl\n"             /* bits += 16 */
"        shll    %%cl, %%eax\n"
"        orl     %%eax, %%edx\n"        /* hold |= *((ushort *)in)++ << bits */
"        movb    %%ch, %%cl\n"            /* move op back to ecx */

".L_add_bits_to_dist:\n"
"        movl    $1, %%eax\n"
"        shll    %%cl, %%eax\n"
"        decl    %%eax\n"                 /* (1 << op) - 1 */
"        subb    %%cl, %%bl\n"
"        andl    %%edx, %%eax\n"          /* eax &= hold */
"        shrl    %%cl, %%edx\n"
"        addl    %%eax, %%ebp\n"          /* dist += hold & ((1 << op) - 1) */

".L_check_window:\n"
"        movl    %%esi, 4(%%esp)\n"       /* save in so from can use it's reg */
"        movl    %%edi, %%eax\n"
"        subl    16(%%esp), %%eax\n"      /* nbytes = out - beg */

"        cmpl    %%ebp, %%eax\n"
"        jb      .L_clip_window\n"        /* if (dist > nbytes) 4.2% */

"        movl    60(%%esp), %%ecx\n"
"        movl    %%edi, %%esi\n"
"        subl    %%ebp, %%esi\n"          /* from = out - dist */

"        subl    $3, %%ecx\n"             /* copy from to out */
"        movb    (%%esi), %%al\n"
"        movb    %%al, (%%edi)\n"
"        movb    1(%%esi), %%al\n"
"        movb    2(%%esi), %%ah\n"
"        addl    $3, %%esi\n"
"        movb    %%al, 1(%%edi)\n"
"        movb    %%ah, 2(%%edi)\n"
"        addl    $3, %%edi\n"
"        rep     movsb\n"

"        movl    4(%%esp), %%esi\n"      /* move in back to %esi, toss from */
"        movl    44(%%esp), %%ebp\n"     /* ebp = lcode */
"        jmp     .L_while_test\n"

".L_check_dist_one:\n"
"        cmpl    $1, %%ebp\n"            /* if dist 1, is a memset */
"        jne     .L_check_window\n"
"        cmpl    %%edi, 16(%%esp)\n"
"        je      .L_check_window\n"

"        decl    %%edi\n"
"        movl    60(%%esp), %%ecx\n"
"        movb    (%%edi), %%al\n"
"        subl    $3, %%ecx\n"

"        movb    %%al, 1(%%edi)\n"       /* memset out with from[-1] */
"        movb    %%al, 2(%%edi)\n"
"        movb    %%al, 3(%%edi)\n"
"        addl    $4, %%edi\n"
"        rep     stosb\n"
"        movl    44(%%esp), %%ebp\n"      /* ebp = lcode */
"        jmp     .L_while_test\n"

".L_test_for_second_level_length:\n"
"        testb   $64, %%al\n"
"        jnz     .L_test_for_end_of_block\n" /* if ((op & 64) != 0) */

"        movl    $1, %%eax\n"
"        shll    %%cl, %%eax\n"
"        decl    %%eax\n"
"        andl    %%edx, %%eax\n"         /* eax &= hold */
"        addl    60(%%esp), %%eax\n"     /* eax += this.val */
"        movl    (%%ebp,%%eax,4), %%eax\n" /* eax = lcode[val+(hold&mask[op])]*/
"        jmp     .L_dolen\n"

".L_test_for_second_level_dist:\n"
"        testb   $64, %%al\n"
"        jnz     .L_invalid_distance_code\n" /* if ((op & 64) != 0) */

"        movl    $1, %%eax\n"
"        shll    %%cl, %%eax\n"
"        decl    %%eax\n"
"        andl    %%edx, %%eax\n"         /* eax &= hold */
"        addl    %%ebp, %%eax\n"         /* eax += this.val */
"        movl    48(%%esp), %%ecx\n"     /* ecx = dcode */
"        movl    (%%ecx,%%eax,4), %%eax\n" /* eax = dcode[val+(hold&mask[op])]*/
"        jmp     .L_dodist\n"

".L_clip_window:\n"
"        movl    %%eax, %%ecx\n"
"        movl    24(%%esp), %%eax\n"     /* prepare for dist compare */
"        negl    %%ecx\n"                /* nbytes = -nbytes */
"        movl    32(%%esp), %%esi\n"     /* from = window */

"        cmpl    %%ebp, %%eax\n"
"        jb      .L_invalid_distance_too_far\n" /* if (dist > wsize) */

"        addl    %%ebp, %%ecx\n"         /* nbytes = dist - nbytes */
"        cmpl    $0, 28(%%esp)\n"
"        jne     .L_wrap_around_window\n" /* if (write != 0) */

"        subl    %%ecx, %%eax\n"
"        addl    %%eax, %%esi\n"         /* from += wsize - nbytes */

"        movl    60(%%esp), %%eax\n"
"        cmpl    %%ecx, %%eax\n"
"        jbe     .L_do_copy1\n"          /* if (nbytes >= len) */

"        subl    %%ecx, %%eax\n"         /* len -= nbytes */
"        rep     movsb\n"
"        movl    %%edi, %%esi\n"
"        subl    %%ebp, %%esi\n"         /* from = out - dist */
"        jmp     .L_do_copy1\n"

"        cmpl    %%ecx, %%eax\n"
"        jbe     .L_do_copy1\n"          /* if (nbytes >= len) */

"        subl    %%ecx, %%eax\n"         /* len -= nbytes */
"        rep     movsb\n"
"        movl    %%edi, %%esi\n"
"        subl    %%ebp, %%esi\n"         /* from = out - dist */
"        jmp     .L_do_copy1\n"

".L_wrap_around_window:\n"
"        movl    28(%%esp), %%eax\n"
"        cmpl    %%eax, %%ecx\n"
"        jbe     .L_contiguous_in_window\n" /* if (write >= nbytes) */

"        addl    24(%%esp), %%esi\n"
"        addl    %%eax, %%esi\n"
"        subl    %%ecx, %%esi\n"         /* from += wsize + write - nbytes */
"        subl    %%eax, %%ecx\n"         /* nbytes -= write */

"        movl    60(%%esp), %%eax\n"
"        cmpl    %%ecx, %%eax\n"
"        jbe     .L_do_copy1\n"          /* if (nbytes >= len) */

"        subl    %%ecx, %%eax\n"         /* len -= nbytes */
"        rep     movsb\n"
"        movl    32(%%esp), %%esi\n"     /* from = window */
"        movl    28(%%esp), %%ecx\n"     /* nbytes = write */
"        cmpl    %%ecx, %%eax\n"
"        jbe     .L_do_copy1\n"          /* if (nbytes >= len) */

"        subl    %%ecx, %%eax\n"         /* len -= nbytes */
"        rep     movsb\n"
"        movl    %%edi, %%esi\n"
"        subl    %%ebp, %%esi\n"         /* from = out - dist */
"        jmp     .L_do_copy1\n"

".L_contiguous_in_window:\n"
"        addl    %%eax, %%esi\n"
"        subl    %%ecx, %%esi\n"         /* from += write - nbytes */

"        movl    60(%%esp), %%eax\n"
"        cmpl    %%ecx, %%eax\n"
"        jbe     .L_do_copy1\n"          /* if (nbytes >= len) */

"        subl    %%ecx, %%eax\n"         /* len -= nbytes */
"        rep     movsb\n"
"        movl    %%edi, %%esi\n"
"        subl    %%ebp, %%esi\n"         /* from = out - dist */

".L_do_copy1:\n"
"        movl    %%eax, %%ecx\n"
"        rep     movsb\n"

"        movl    4(%%esp), %%esi\n"      /* move in back to %esi, toss from */
"        movl    44(%%esp), %%ebp\n"     /* ebp = lcode */
"        jmp     .L_while_test\n"

".L_test_for_end_of_block:\n"
"        testb   $32, %%al\n"
"        jz      .L_invalid_literal_length_code\n"
"        movl    $1, 68(%%esp)\n"
"        jmp     .L_break_loop_with_status\n"

".L_invalid_literal_length_code:\n"
"        movl    $2, 68(%%esp)\n"
"        jmp     .L_break_loop_with_status\n"

".L_invalid_distance_code:\n"
"        movl    $3, 68(%%esp)\n"
"        jmp     .L_break_loop_with_status\n"

".L_invalid_distance_too_far:\n"
"        movl    4(%%esp), %%esi\n"
"        movl    $4, 68(%%esp)\n"
"        jmp     .L_break_loop_with_status\n"

".L_break_loop:\n"
"        movl    $0, 68(%%esp)\n"

".L_break_loop_with_status:\n"
/* put in, out, bits, and hold back into ar and pop esp */
"        movl    %%esi, 4(%%esp)\n"
"        movl    %%edi, 12(%%esp)\n"
"        movl    %%ebx, 40(%%esp)\n"
"        movl    %%edx, 36(%%esp)\n"
"        movl    (%%esp), %%esp\n"
"        popl    %%ebp\n"
"        popf\n"
          :
          : "m" (ar)
          : "memory", "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"
    );


    if (ar.status > 1) {
        if (ar.status == 2)
            strm->msg = "invalid literal/length code";
        else if (ar.status == 3)
            strm->msg = "invalid distance code";
        else
            strm->msg = "invalid distance too far back";
        state->mode = BAD;
    }
    else if ( ar.status == 1 ) {
        state->mode = TYPE;
    }

    /* return unused bytes (on entry, bits < 8, so in won't go too far back) */
    ar.len = ar.bits >> 3;
    ar.in -= ar.len;
    ar.bits -= ar.len << 3;
    ar.hold &= (1U << ar.bits) - 1;

    /* update state and return */
    strm->next_in = ar.in;
    strm->next_out = ar.out;
    strm->avail_in = (unsigned)(ar.in < ar.last ? 5 + (ar.last - ar.in) :
                                                  5 - (ar.in - ar.last));
    strm->avail_out = (unsigned)(ar.out < ar.end ? 257 + (ar.end - ar.out) :
                                                   257 - (ar.out - ar.end));
    state->hold = ar.hold;
    state->bits = ar.bits;
    return;
}

#else

/* Allow machine dependent optimization for post-increment or pre-increment.
   Based on testing to date,
   Pre-increment preferred for:
   - PowerPC G3 (Adler)
   - MIPS R5000 (Randers-Pehrson)
   Post-increment preferred for:
   - none
   No measurable difference:
   - Pentium III (Anderson)
   - M68060 (Nikl)
 */
#ifdef POSTINC
#  define OFF 0
#  define PUP(a) *(a)++
#else
#  define OFF 1
#  define PUP(a) *++(a)
#endif

/*
   Decode literal, length, and distance codes and write out the resulting
   literal and match bytes until either not enough input or output is
   available, an end-of-block is encountered, or a data error is encountered.
   When large enough input and output buffers are supplied to inflate(), for
   example, a 16K input buffer and a 64K output buffer, more than 95% of the
   inflate execution time is spent in this routine.

   Entry assumptions:

        state->mode == LEN
        strm->avail_in >= 6
        strm->avail_out >= 258
        start >= strm->avail_out
        state->bits < 8

   On return, state->mode is one of:

        LEN -- ran out of enough output space or enough available input
        TYPE -- reached end of block code, inflate() to interpret next block
        BAD -- error in block data

   Notes:

    - The maximum input bits used by a length/distance pair is 15 bits for the
      length code, 5 bits for the length extra, 15 bits for the distance code,
      and 13 bits for the distance extra.  This totals 48 bits, or six bytes.
      Therefore if strm->avail_in >= 6, then there is enough input to avoid
      checking for available input while decoding.

    - The maximum bytes that a single length/distance pair can output is 258
      bytes, which is the maximum length that can be coded.  inflate_fast()
      requires strm->avail_out >= 258 for each loop to avoid checking for
      output space.
 */
void inflate_fast(strm, start)
z_streamp strm;
unsigned start;         /* inflate()'s starting value for strm->avail_out */
{
    struct inflate_state FAR *state;
    unsigned char FAR *in;      /* local strm->next_in */
    unsigned char FAR *last;    /* while in < last, enough input available */
    unsigned char FAR *out;     /* local strm->next_out */
    unsigned char FAR *beg;     /* inflate()'s initial strm->next_out */
    unsigned char FAR *end;     /* while out < end, enough space available */
#ifdef INFLATE_STRICT
    unsigned dmax;              /* maximum distance from zlib header */
#endif
    unsigned wsize;             /* window size or zero if not using window */
    unsigned whave;             /* valid bytes in the window */
    unsigned write;             /* window write index */
    unsigned char FAR *window;  /* allocated sliding window, if wsize != 0 */
    unsigned long hold;         /* local strm->hold */
    unsigned bits;              /* local strm->bits */
    code const FAR *lcode;      /* local strm->lencode */
    code const FAR *dcode;      /* local strm->distcode */
    unsigned lmask;             /* mask for first level of length codes */
    unsigned dmask;             /* mask for first level of distance codes */
    code this;                  /* retrieved table entry */
    unsigned op;                /* code bits, operation, extra bits, or */
                                /*  window position, window bytes to copy */
    unsigned len;               /* match length, unused bytes */
    unsigned dist;              /* match distance */
    unsigned char FAR *from;    /* where to copy match from */

    /* copy state to local variables */
    state = (struct inflate_state FAR *)strm->state;
    in = strm->next_in - OFF;
    last = in + (strm->avail_in - 5);
    out = strm->next_out - OFF;
    beg = out - (start - strm->avail_out);
    end = out + (strm->avail_out - 257);
#ifdef INFLATE_STRICT
    dmax = state->dmax;
#endif
    wsize = state->wsize;
    whave = state->whave;
    write = state->write;
    window = state->window;
    hold = state->hold;
    bits = state->bits;
    lcode = state->lencode;
    dcode = state->distcode;
    lmask = (1U << state->lenbits) - 1;
    dmask = (1U << state->distbits) - 1;

    /* decode literals and length/distances until end-of-block or not enough
       input data or output space */
    do {
        if (bits < 15) {
            hold += (unsigned long)(PUP(in)) << bits;
            bits += 8;
            hold += (unsigned long)(PUP(in)) << bits;
            bits += 8;
        }
        this = lcode[hold & lmask];
      dolen:
        op = (unsigned)(this.bits);
        hold >>= op;
        bits -= op;
        op = (unsigned)(this.op);
        if (op == 0) {                          /* literal */
            Tracevv((stderr, this.val >= 0x20 && this.val < 0x7f ?
                    "inflate:         literal '%c'\n" :
                    "inflate:         literal 0x%02x\n", this.val));
            PUP(out) = (unsigned char)(this.val);
        }
        else if (op & 16) {                     /* length base */
            len = (unsigned)(this.val);
            op &= 15;                           /* number of extra bits */
            if (op) {
                if (bits < op) {
                    hold += (unsigned long)(PUP(in)) << bits;
                    bits += 8;
                }
                len += (unsigned)hold & ((1U << op) - 1);
                hold >>= op;
                bits -= op;
            }
            Tracevv((stderr, "inflate:         length %u\n", len));
            if (bits < 15) {
                hold += (unsigned long)(PUP(in)) << bits;
                bits += 8;
                hold += (unsigned long)(PUP(in)) << bits;
                bits += 8;
            }
            this = dcode[hold & dmask];
          dodist:
            op = (unsigned)(this.bits);
            hold >>= op;
            bits -= op;
            op = (unsigned)(this.op);
            if (op & 16) {                      /* distance base */
                dist = (unsigned)(this.val);
                op &= 15;                       /* number of extra bits */
                if (bits < op) {
                    hold += (unsigned long)(PUP(in)) << bits;
                    bits += 8;
                    if (bits < op) {
                        hold += (unsigned long)(PUP(in)) << bits;
                        bits += 8;
                    }
                }
                dist += (unsigned)hold & ((1U << op) - 1);
#ifdef INFLATE_STRICT
                if (dist > dmax) {
                    strm->msg = (char *)"invalid distance too far back";
                    state->mode = BAD;
                    break;
                }
#endif
                hold >>= op;
                bits -= op;
                Tracevv((stderr, "inflate:         distance %u\n", dist));
                op = (unsigned)(out - beg);     /* max distance in output */
                if (dist > op) {                /* see if copy from window */
                    op = dist - op;             /* distance back in window */
                    if (op > whave) {
                        strm->msg = (char *)"invalid distance too far back";
                        state->mode = BAD;
                        break;
                    }
                    from = window - OFF;
                    if (write == 0) {           /* very common case */
                        from += wsize - op;
                        if (op < len) {         /* some from window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = out - dist;  /* rest from output */
                        }
                    }
                    else if (write < op) {      /* wrap around window */
                        from += wsize + write - op;
                        op -= write;
                        if (op < len) {         /* some from end of window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = window - OFF;
                            if (write < len) {  /* some from start of window */
                                op = write;
                                len -= op;
                                do {
                                    PUP(out) = PUP(from);
                                } while (--op);
                                from = out - dist;      /* rest from output */
                            }
                        }
                    }
                    else {                      /* contiguous in window */
                        from += write - op;
                        if (op < len) {         /* some from window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = out - dist;  /* rest from output */
                        }
                    }
                    while (len > 2) {
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        len -= 3;
                    }
                    if (len) {
                        PUP(out) = PUP(from);
                        if (len > 1)
                            PUP(out) = PUP(from);
                    }
                }
                else {
                    from = out - dist;          /* copy direct from output */
                    do {                        /* minimum length is three */
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        len -= 3;
                    } while (len > 2);
                    if (len) {
                        PUP(out) = PUP(from);
                        if (len > 1)
                            PUP(out) = PUP(from);
                    }
                }
            }
            else if ((op & 64) == 0) {          /* 2nd level distance code */
                this = dcode[this.val + (hold & ((1U << op) - 1))];
                goto dodist;
            }
            else {
                strm->msg = (char *)"invalid distance code";
                state->mode = BAD;
                break;
            }
        }
        else if ((op & 64) == 0) {              /* 2nd level length code */
            this = lcode[this.val + (hold & ((1U << op) - 1))];
            goto dolen;
        }
        else if (op & 32) {                     /* end-of-block */
            Tracevv((stderr, "inflate:         end of block\n"));
            state->mode = TYPE;
            break;
        }
        else {
            strm->msg = (char *)"invalid literal/length code";
            state->mode = BAD;
            break;
        }
    } while (in < last && out < end);

    /* return unused bytes (on entry, bits < 8, so in won't go too far back) */
    len = bits >> 3;
    in -= len;
    bits -= len << 3;
    hold &= (1U << bits) - 1;

    /* update state and return */
    strm->next_in = in + OFF;
    strm->next_out = out + OFF;
    strm->avail_in = (unsigned)(in < last ? 5 + (last - in) : 5 - (in - last));
    strm->avail_out = (unsigned)(out < end ?
                                 257 + (end - out) : 257 - (out - end));
    state->hold = hold;
    state->bits = bits;
    return;
}

/*
   inflate_fast() speedups that turned out slower (on a PowerPC G3 750CXe):
   - Using bit fields for code structure
   - Different op definition to avoid & for extra bits (do & for table bits)
   - Three separate decoding do-loops for direct, window, and write == 0
   - Special case for distance > 1 copies to do overlapped load and store copy
   - Explicit branch predictions (based on measured branch probabilities)
   - Deferring match copy and interspersed it with decoding subsequent codes
   - Swapping literal/length else
   - Swapping window/direct else
   - Larger unrolled copy loops (three is about right)
   - Moving len -= 3 statement into middle of loop
 */

#endif /* !i386 */
