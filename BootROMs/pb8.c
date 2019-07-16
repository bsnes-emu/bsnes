/*

PB8 compressor and decompressor

Copyright 2019 Damian Yerrick

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// For setting stdin/stdout to binary mode
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <unistd.h>
#define fd_isatty isatty
#elif defined (_WIN32)
#include <io.h>
#include <fcntl.h>
#define fd_isatty _isatty
#endif

/*

; The logo is compressed using PB8, a form of RLE with unary-coded
; run lengths.  Each block representing 8 bytes consists of a control
; byte, where each bit (MSB to LSB) is 0 for literal or 1 for repeat
; previous, followed by the literals in that block.

SameBoyLogo_dst = $8080
SameBoyLogo_length = (128 * 24) / 64

LoadTileset:
    ld hl, SameBoyLogo
    ld de, SameBoyLogo_dst
    ld c, SameBoyLogo_length
.pb8BlockLoop:
    ; Register map for PB8 decompression
    ; HL: source address in boot ROM
    ; DE: destination address in VRAM
    ; A: Current literal value
    ; B: Repeat bits, terminated by 1000...
    ; C: Number of 8-byte blocks left in this block
    ; Source address in HL lets the repeat bits go straight to B,
    ; bypassing A and avoiding spilling registers to the stack.
    ld b, [hl]
    inc hl

    ; Shift a 1 into lower bit of shift value.  Once this bit
    ; reaches the carry, B becomes 0 and the byte is over
    scf
    rl b

.pb8BitLoop:
    ; If not a repeat, load a literal byte
    jr c,.pb8Repeat
    ld a, [hli]
.pb8Repeat:
    ; Decompressed data uses colors 0 and 1, so write once, inc twice
    ld [de], a
    inc de
    inc de
    sla b
    jr nz, .pb8BitLoop

    dec c
    jr nz, .pb8BlockLoop
    ret

*/

/* Compressor and decompressor *************************************/

/**
 * Compresses an input stream to PB8 data on an output stream.
 * @param infp input stream
 * @param outfp output stream
 * @param blocklength size of an independent input block in bytes
 * @return 0 for reaching infp end of file, or EOF for error
 */
int pb8(FILE *infp, FILE *outfp, size_t blocklength) {
  blocklength >>= 3;  // convert bytes to blocks
  assert(blocklength > 0);
  while (1) {
    int last_byte = EOF;  // value that never occurs in a file
    for (size_t blkleft = blocklength; blkleft > 0; --blkleft) {
      unsigned int control_byte = 0x0001;
      unsigned char literals[8];
      size_t nliterals = 0;
      while (control_byte < 0x100) {
        int c = fgetc(infp);
        if (c == EOF) break;

        control_byte <<= 1;
        if (c == last_byte) {
          control_byte |= 0x01;
        } else {
          literals[nliterals++] = last_byte = c;
        }
      }
      if (control_byte > 1) {
        // Fill partial block with repeats
        while (control_byte < 0x100) {
          control_byte = (control_byte << 1) | 1;
        }

        // Write control byte and check for write failure
        int ok = fputc(control_byte & 0xFF, outfp);
        if (ok == EOF) return EOF;
        size_t ok2 = fwrite(literals, 1, nliterals, outfp);
        if (ok2 < nliterals) return EOF;
      }

      // If finished, return success or failure
      if (ferror(infp) || ferror(outfp)) return EOF;
      if (feof(infp)) return 0;
    }  // End 8-byte block
  }  // End packet, resetting last_byte
}

/**
 * Decompresses PB8 data on an input stream to an output stream.
 * @param infp input stream
 * @param outfp output stream
 * @return 0 for reaching infp end of file, or EOF for error
 */
int unpb8(FILE *infp, FILE *outfp) {
  int last_byte = 0;
  while (1) {
    int control_byte = fgetc(infp);
    if (control_byte == EOF) {
      return feof(infp) ? 0 : EOF;
    }
    control_byte &= 0xFF;
    for (size_t bytesleft = 8; bytesleft > 0; --bytesleft) {
      if (!(control_byte & 0x80)) {
        last_byte = fgetc(infp);
        if (last_byte == EOF) return EOF;  // read error
      }
      control_byte <<= 1;
      int ok = fputc(last_byte, outfp);
      if (ok == EOF) return EOF;
    }
  }
}

/* CLI frontend ****************************************************/

static inline void set_fd_binary(unsigned int fd) {
#ifdef _WIN32
  _setmode(fd, _O_BINARY);
#else
  (void) fd;
#endif
}

static const char *usage_msg =
"usage: pb8 [-d] [-l blocklength] [infile [outfile]]\n"
"Compresses a file using RLE with unary run and literal lengths.\n"
"\n"
"options:\n"
"  -d                decompress\n"
"  -l blocklength    allow RLE packets to span up to blocklength\n"
"                      input bytes (multiple of 8; default 8)\n"
"  -h, -?, --help    show this usage page\n"
"  --version         show copyright info\n"
"\n"
"If infile is - or missing, it is standard input.\n"
"If outfile is - or missing, it is standard output.\n"
"You cannot compress to or decompress from a terminal.\n"
;
static const char *version_msg =
"PB8 compressor (C version) v0.01\n"
"Copyright 2019 Damian Yerrick <https://pineight.com/contact/>\n"
"This software is provided 'as-is', without any express or implied\n"
"warranty.\n"
;
static const char *toomanyfilenames_msg =
"pb8: too many filenames; try pb8 --help\n";

int main(int argc, char **argv) {
  const char *infilename = NULL;
  const char *outfilename = NULL;
  bool decompress = false;
  size_t blocklength = 8;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1] != 0) {
      if (!strcmp(argv[i], "--help")) {
        fputs(usage_msg, stdout);
        return 0;
      }
      if (!strcmp(argv[i], "--version")) {
        fputs(version_msg, stdout);
        return 0;
      }

      // -t1 or -t 1
      int argtype = argv[i][1];
      switch (argtype) {
        case 'h':
        case '?':
          fputs(usage_msg, stdout);
          return 0;

        case 'd':
          decompress = true;
          break;

        case 'l': {
          const char *argvalue = argv[i][2] ? argv[i] + 2 : argv[++i];
          const char *endptr = NULL;

          unsigned long tvalue = strtoul(argvalue, (char **)&endptr, 10);
          if (endptr == argvalue || tvalue == 0 || tvalue > SIZE_MAX) {
            fprintf(stderr, "pb8: block length %s not a positive integer\n",
                    argvalue);
            return EXIT_FAILURE;
          }
          if (tvalue % 8 != 0) {
            fprintf(stderr, "pb8: block length %s not a multiple of 8\n",
                    argvalue);
            return EXIT_FAILURE;
          }
          blocklength = tvalue;
        } break;

        default:
          fprintf(stderr, "pb8: unknown option -%c\n", argtype);
          return EXIT_FAILURE;
      }
    } else if (!infilename) {
      infilename = argv[i];
    } else if (!outfilename) {
      outfilename = argv[i];
    } else {
      fputs(toomanyfilenames_msg, stderr);
      return EXIT_FAILURE;
    }
  }
  if (infilename && !strcmp(infilename, "-")) {
    infilename = NULL;
  }
  if (!infilename && decompress && fd_isatty(0)) {
    fputs("pb8: cannot decompress from terminal; try redirecting stdin\n",
          stderr);
    return EXIT_FAILURE;
  }
  if (outfilename && !strcmp(outfilename, "-")) {
    outfilename = NULL;
  }
  if (!outfilename && !decompress && fd_isatty(1)) {
    fputs("pb8: cannot compress to terminal; try redirecting stdout or pb8 --help\n",
          stderr);
    return EXIT_FAILURE;
  }

  FILE *infp = NULL;
  if (infilename) {
    infp = fopen(infilename, "rb");
    if (!infp) {
      fprintf(stderr, "pb8: error opening %s ", infilename);
      perror("for reading");
      return EXIT_FAILURE;
    }
  } else {
    infp = stdin;
    set_fd_binary(0);
  }

  FILE *outfp = NULL;
  if (outfilename) {
    outfp = fopen(outfilename, "wb");
    if (!outfp) {
      fprintf(stderr, "pb8: error opening %s ", outfilename);
      perror("for writing");
      fclose(infp);
      return EXIT_FAILURE;
    }
  } else {
    outfp = stdout;
    set_fd_binary(1);
  }

  int compfailed = 0;
  int has_ferror = 0;
  if (decompress) {
    compfailed = unpb8(infp, outfp);
  } else {
    compfailed = pb8(infp, outfp, blocklength);
  }
  fflush(outfp);
  if (ferror(infp)) {
    fprintf(stderr, "pb8: error reading %s\n",
            infilename ? infilename : "<stdin>");
    has_ferror = EOF;
  }
  fclose(infp);
  if (ferror(outfp)) {
    fprintf(stderr, "pb8: error writing %s\n",
            outfilename ? outfilename : "<stdout>");
    has_ferror = EOF;
  }
  fclose(outfp);

  if (compfailed && !has_ferror) {
    fputs("pb8: unknown compression failure\n", stderr);
  }

  return (compfailed || has_ferror) ? EXIT_FAILURE : EXIT_SUCCESS;
}
