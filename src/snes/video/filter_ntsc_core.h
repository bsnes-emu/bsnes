
/* SNES NTSC composite video to RGB emulator/blitter */

/* snes_ntsc 0.2.1 */

#ifndef SNES_NTSC_H
#define SNES_NTSC_H

/* Image parameters, ranging from -1.0 to 1.0 */
typedef struct snes_ntsc_setup_t
{
	/* Basic parameters */
	double hue;        /* -1 = -180 degrees, +1 = +180 degrees */
	double saturation; /* -1 = grayscale, +1 = oversaturated colors */
	double contrast;
	double brightness;
	double sharpness;  /* edge contrast enhancement/blurring */
	
	/* Advanced parameters */
	double gamma;
	double resolution; /* image resolution */
	double artifacts;  /* artifacts caused by color changes */
	double fringing;   /* color artifacts caused by brightness changes */
	double bleed;      /* color bleed (color resolution reduction) */
	double hue_warping;/* -1 = expand purple & green, +1 = expand orange & cyan */
	int merge_fields;  /* if 1, merges even and odd fields together to reduce flicker */
	float const* decoder_matrix; /* optional RGB decoder matrix, 6 elements */
	
	unsigned long const* bsnes_colortbl; /* temporary feature for bsnes only; set to 0 */
} snes_ntsc_setup_t;

/* Video format presets */
extern snes_ntsc_setup_t const snes_ntsc_composite; /* color bleeding + artifacts */
extern snes_ntsc_setup_t const snes_ntsc_svideo;    /* color bleeding only */
extern snes_ntsc_setup_t const snes_ntsc_rgb;       /* crisp image */
extern snes_ntsc_setup_t const snes_ntsc_monochrome;/* desaturated + artifacts */

/* Initialize and adjust parameters. Can be called multiple times on the same
snes_ntsc_t object. Caller must allocate memory for snes_ntsc_t. Can pass 0
for either parameter. */
typedef struct snes_ntsc_t snes_ntsc_t;
void snes_ntsc_init( snes_ntsc_t*, snes_ntsc_setup_t const* setup );

/* Blit one or more rows of pixels. Input pixel format is set by SNES_NTSC_IN_FORMAT
and output RGB depth is set by NES_NTSC_OUT_DEPTH. Both default to 16-bit RGB.
In_row_width is the number of pixels to get to the next input row. Out_pitch
is the number of *bytes* to get to the next output row. */
void snes_ntsc_blit( snes_ntsc_t const*, unsigned short const* snes_in,
		long in_row_width, int burst_phase, int in_width, int in_height,
		void* rgb_out, long out_pitch );

void snes_ntsc_blit_hires( snes_ntsc_t const*, unsigned short const* snes_in,
		long in_row_width, int burst_phase, int in_width, int in_height,
		void* rgb_out, long out_pitch );

/* Number of output pixels written by low-res blitter for given input width. Width
might be rounded down slightly; use SNES_NTSC_IN_WIDTH() on result to find rounded
value. Guaranteed not to round 256 down at all. */
#define SNES_NTSC_OUT_WIDTH( in_width ) \
	(((in_width) - 1) / snes_ntsc_in_chunk * snes_ntsc_out_chunk + snes_ntsc_out_chunk)

/* Number of low-res input pixels that will fit within given output width. Might be
rounded down slightly; use SNES_NTSC_OUT_WIDTH() on result to find rounded
value. */
#define SNES_NTSC_IN_WIDTH( out_width ) \
	((out_width) / snes_ntsc_out_chunk * snes_ntsc_in_chunk - snes_ntsc_in_chunk + 1)


/* Interface for user-defined custom blitters */

enum { snes_ntsc_in_chunk    = 3  }; /* number of snes pixels read per chunk */
enum { snes_ntsc_out_chunk   = 7  }; /* number of output pixels generated per chunk */
enum { snes_ntsc_black       = 0  }; /* palette index for black */
enum { snes_ntsc_burst_count = 3  }; /* burst phase cycles through 0, 1, and 2 */

/* Begin outputting row and start three pixels. First pixel will be cut off a bit.
Use snes_ntsc_black for unused pixels. Declares variables, so must be before first
statement in a block (unless you're using C++). */
#define SNES_NTSC_LORES_ROW( ntsc, burst, pixel0, pixel1, pixel2 ) \
	char const* ktable = (char*) (ntsc)->table + burst * (snes_ntsc_burst_size * sizeof (ntsc_rgb_t));\
	int const snes_pixel0_ = (pixel0);\
	ntsc_rgb_t const* kernel0  = SNES_NTSC_IN_FORMAT( snes_pixel0_ );\
	int const snes_pixel1_ = (pixel1);\
	ntsc_rgb_t const* kernel1  = SNES_NTSC_IN_FORMAT( snes_pixel1_ );\
	int const snes_pixel2_ = (pixel2);\
	ntsc_rgb_t const* kernel2  = SNES_NTSC_IN_FORMAT( snes_pixel2_ );\
	ntsc_rgb_t const* kernelx0;\
	ntsc_rgb_t const* kernelx1 = kernel0;\
	ntsc_rgb_t const* kernelx2 = kernel0

/* Begin input pixel */
#define SNES_NTSC_PIXEL_IN( in_index, color_in ) {\
	unsigned n;\
	kernelx##in_index = kernel##in_index;\
	kernel##in_index = (n = (color_in), SNES_NTSC_IN_FORMAT( n ));\
}

/* Generate output pixel. Bits can be 24, 16, 15, or 32 (treated as 24):
24: RRRRRRRR GGGGGGGG BBBBBBBB
16:          RRRRRGGG GGGBBBBB
15:           RRRRRGG GGGBBBBB
 0: xxxRRRRR RRRxxGGG GGGGGxxB BBBBBBBx (raw format; x = junk bits) */
#define SNES_NTSC_LORES_OUT( x, rgb_out, bits ) {\
	ntsc_rgb_t raw =\
		kernel0  [x       ] + kernel1  [(x+12)%7+14] + kernel2  [(x+10)%7+28] +\
		kernelx0 [(x+7)%14] + kernelx1 [(x+ 5)%7+21] + kernelx2 [(x+ 3)%7+35];\
	SNES_NTSC_CLAMP_( raw, 1 );\
	SNES_NTSC_OUT_( rgb_out, (bits), raw, 1 );\
}

/* Hires equivalents */
#define SNES_NTSC_HIRES_ROW( ntsc, burst, pixel1, pixel2, pixel3, pixel4, pixel5 ) \
	char const* ktable = (char*) (ntsc)->table + burst * (snes_ntsc_burst_size * sizeof (ntsc_rgb_t));\
	int const snes_pixel1_ = (pixel1);\
	ntsc_rgb_t const* kernel1  = SNES_NTSC_IN_FORMAT( snes_pixel1_ );\
	int const snes_pixel2_ = (pixel2);\
	ntsc_rgb_t const* kernel2  = SNES_NTSC_IN_FORMAT( snes_pixel2_ );\
	int const snes_pixel3_ = (pixel3);\
	ntsc_rgb_t const* kernel3  = SNES_NTSC_IN_FORMAT( snes_pixel3_ );\
	int const snes_pixel4_ = (pixel4);\
	ntsc_rgb_t const* kernel4  = SNES_NTSC_IN_FORMAT( snes_pixel4_ );\
	int const snes_pixel5_ = (pixel5);\
	ntsc_rgb_t const* kernel5  = SNES_NTSC_IN_FORMAT( snes_pixel5_ );\
	ntsc_rgb_t const* kernel0 = kernel1;\
	ntsc_rgb_t const* kernelx0;\
	ntsc_rgb_t const* kernelx1 = kernel1;\
	ntsc_rgb_t const* kernelx2 = kernel1;\
	ntsc_rgb_t const* kernelx3 = kernel1;\
	ntsc_rgb_t const* kernelx4 = kernel1;\
	ntsc_rgb_t const* kernelx5 = kernel1

#define SNES_NTSC_HIRES_OUT( x, rgb_out, bits ) {\
	ntsc_rgb_t raw =\
		kernel0  [ x       ] + kernel2  [(x+5)%7+14] + kernel4  [(x+3)%7+28] +\
		kernelx0 [(x+7)%7+7] + kernelx2 [(x+5)%7+21] + kernelx4 [(x+3)%7+35] +\
		kernel1  [(x+6)%7  ] + kernel3  [(x+4)%7+14] + kernel5  [(x+2)%7+28] +\
		kernelx1 [(x+6)%7+7] + kernelx3 [(x+4)%7+21] + kernelx5 [(x+2)%7+35];\
	SNES_NTSC_CLAMP_( raw, 0 );\
	SNES_NTSC_OUT_( rgb_out, (bits), raw, 0 );\
}


/* private */

enum { snes_ntsc_entry_size = 128 };
enum { snes_ntsc_color_count = 0x2000 };
typedef unsigned long ntsc_rgb_t;
struct snes_ntsc_t
{
	ntsc_rgb_t table [snes_ntsc_color_count] [snes_ntsc_entry_size];
};
enum { snes_ntsc_burst_size = snes_ntsc_entry_size / snes_ntsc_burst_count };

enum { ntsc_rgb_builder = (1L << 21) | (1 << 11) | (1 << 1) };
enum { snes_ntsc_clamp_mask = ntsc_rgb_builder * 3 / 2 };
enum { snes_ntsc_clamp_add  = ntsc_rgb_builder * 0x101 };

#define SNES_NTSC_RGB16( n ) \
	(ntsc_rgb_t*) (ktable + ((n & 0x001E) | (n >> 1 & 0x03E0) | (n >> 2 & 0x3C00)) * \
			(snes_ntsc_entry_size / 2 * sizeof (ntsc_rgb_t)))

#define SNES_NTSC_BGR15( n ) \
	(ntsc_rgb_t*) (ktable + ((n << 9 & 0x3C00) | (n & 0x03E0) | (n >> 10 & 0x001E)) * \
			(snes_ntsc_entry_size / 2 * sizeof (ntsc_rgb_t)))

#define SNES_NTSC_CLAMP_( io, shift ) {\
	ntsc_rgb_t sub = io >> (9-shift) & snes_ntsc_clamp_mask;\
	ntsc_rgb_t clamp = snes_ntsc_clamp_add - sub;\
	io |= clamp;\
	clamp -= sub;\
	io &= clamp;\
}

#define SNES_NTSC_OUT_( rgb_out, bits, raw, x ) {\
	if ( bits == 16 ) {\
		rgb_out = (raw>>(13-x)& 0xF800)|(raw>>(8-x)&0x07E0)|(raw>>(4-x)&0x001F);\
		rgb_out = ((rgb_out&0xf800)>>11)|((rgb_out&0x07c0)>>1)|((rgb_out&0x001f)<<10);\
		rgb_out = snes->color_lookup_table[rgb_out];\
	} else if ( bits == 24 || bits == 32 ) {\
		rgb_out = (raw>>(5-x)&0xFF0000)|(raw>>(3-x)&0xFF00)|(raw>>(1-x)&0xFF);\
		rgb_out = ((rgb_out&0xf80000)>>19)|((rgb_out&0x00f800)>>6)|((rgb_out&0x0000f8)<<7);\
		rgb_out = snes->color_lookup_table[rgb_out];\
	} else if ( bits == 15 ) {\
		rgb_out = (raw>>(14-x)& 0x7C00)|(raw>>(9-x)&0x03E0)|(raw>>(4-x)&0x001F);\
		rgb_out = ((rgb_out&0x7c00)>>10)|((rgb_out&0x03e0))|((rgb_out&0x001f)<<10);\
		rgb_out = snes->color_lookup_table[rgb_out];\
	} else {\
		rgb_out = raw;\
	}\
}

#endif

