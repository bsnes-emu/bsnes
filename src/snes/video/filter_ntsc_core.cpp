
/* snes_ntsc 0.2.1. http://www.slack.net/~ant/ */

/* compilable in C or C++; just change the file extension */

//#include "snes_ntsc.h"

#include <assert.h>
#include <math.h>

/* Based on algorithm by NewRisingSun */
/* Copyright (C) 2006 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details. You should have received a copy of the GNU Lesser General
Public License along with this module; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */

enum { disable_correction = 0 }; /* for debugging */

/* macro constants are used instead of enum in some places to work around compiler bug */

/* half normal range to allow for doubled hires pixels */
#define rgb_unit 0x80

/* begin mostly common NES/SNES/SMS code */

snes_ntsc_setup_t const snes_ntsc_monochrome = { 0,-1, 0, 0,.2,  0,.2,-.2,-.2,-1, 0, 1, 0, 0 };
snes_ntsc_setup_t const snes_ntsc_composite  = { 0, 0, 0, 0, 0,  0, 0,  0,  0, 0, 0, 1, 0, 0 };
snes_ntsc_setup_t const snes_ntsc_svideo     = { 0, 0, 0, 0,.2,  0,.2, -1, -1, 0, 0, 1, 0, 0 };
snes_ntsc_setup_t const snes_ntsc_rgb        = { 0, 0, 0, 0,.2,  0,.7, -1, -1,-1, 0, 1, 0, 0 };

enum { alignment_count = 3 }; /* different pixel alignments with respect to yiq quads */

enum { kernel_half = 16 };
enum { kernel_size = kernel_half * 2 + 1 };
#define rescale_in 8
enum { rescale_out = 7 };

struct ntsc_impl_t
{
	float to_rgb [snes_ntsc_burst_count] [6];
	float brightness;
	float contrast;
	float artifacts;
	float fringing;
	float hue_warping;
	float kernel [rescale_out * kernel_size * 2];
};

#define PI 3.14159265358979323846f

static void init_ntsc_impl( struct ntsc_impl_t* impl, snes_ntsc_setup_t const* setup )
{
	float kernels [kernel_size * 2];
	
	impl->brightness  = (float) setup->brightness * (0.4f * rgb_unit);
	impl->contrast    = (float) setup->contrast * 0.4f + 1.0f;
	impl->hue_warping = (float) setup->hue_warping;
	
	impl->artifacts   = (float) setup->artifacts;
	if ( impl->artifacts > 0 )
		impl->artifacts *= 0.5f;
	impl->artifacts += 1.0f;
	
	impl->fringing   = (float) setup->fringing;
	if ( impl->fringing > 0 )
		impl->fringing *= 0.5f;
	impl->fringing += 1.0f;
	
	/* generate luma (y) filter using sinc kernel */
	{
		/* sinc with rolloff (dsf) */
		/* double precision avoids instability */
		double const rolloff = 1 + setup->sharpness * 0.004;
		double const maxh = 256;
		double const pow_a_n = pow( rolloff, maxh );
		float sum;
		int i;
		/* quadratic mapping to reduce negative (blurring) range */
		double to_angle = setup->resolution + 1;
		to_angle = PI / maxh * 0.20 * (to_angle * to_angle + 1);
		
		kernels [kernel_size * 3 / 2] = (float) maxh;
		for ( i = 0; i < kernel_half * 2 + 1; i++ )
		{
			int x = i - kernel_half;
			double angle = x * to_angle;
			/* instability occurs at center point with rolloff very close to 1.0 */
			if ( x || pow_a_n > 1.01 || pow_a_n < 0.99 )
			{
				double rolloff_cos_a = rolloff * cos( angle );
				double num = 1 - rolloff_cos_a -
						pow_a_n * cos( maxh * angle ) +
						pow_a_n * rolloff * cos( (maxh - 1) * angle );
				double den = 1 - rolloff_cos_a - rolloff_cos_a + rolloff * rolloff;
				double dsf = num / den;
				kernels [kernel_size * 3 / 2 - kernel_half + i] = (float) dsf;
			}
		}
		
		/* apply blackman window and find sum */
		sum = 0;
		for ( i = 0; i < kernel_half * 2 + 1; i++ )
		{
			float x = PI * 2 / (kernel_half * 2) * i;
			float blackman = 0.42f - 0.5f * (float) cos( x ) + 0.08f * (float) cos( x * 2 );
			sum += (kernels [kernel_size * 3 / 2 - kernel_half + i] *= blackman);
		}
		
		/* normalize kernel */
		sum = 1.0f / sum;
		for ( i = 0; i < kernel_half * 2 + 1; i++ )
		{
			int x = kernel_size * 3 / 2 - kernel_half + i;
			kernels [x] *= sum;
			assert( kernels [x] == kernels [x] ); /* catch numerical instability */
		}
	}
	
	/* generate chroma (iq) filter using gaussian kernel */
	{
		float const cutoff_factor = -0.03125f;
		float cutoff = (float) setup->bleed;
		int i;
		
		if ( cutoff < 0 )
		{
			/* keep extreme value accessible only near upper end of scale (1.0) */
			cutoff *= cutoff;
			cutoff *= cutoff;
			cutoff *= cutoff;
			cutoff *= -30.0f / 0.65f;
		}
		cutoff = cutoff_factor - 0.65f * cutoff_factor * cutoff;
		
		for ( i = -kernel_half; i <= kernel_half; i++ )
			kernels [kernel_size / 2 + i] = (float) exp( i * i * cutoff );
		
		/* normalize even and odd phases separately */
		for ( i = 0; i < 2; i++ )
		{
			float sum = 0;
			int x;
			for ( x = i; x < kernel_size; x += 2 )
				sum += kernels [x];
			
			sum = 1.0f / sum;
			for ( x = i; x < kernel_size; x += 2 )
			{
				kernels [x] *= sum;
				assert( kernels [x] == kernels [x] ); /* catch numerical instability */
			}
		}
	}
	
	/* generate linear rescale kernels */
	{
		int i;
		for ( i = 0; i < rescale_out; i++ )
		{
			float* out = &impl->kernel [i * kernel_size * 2];
			float second = 1.0f / rescale_in * (i + 1);
			float first  = 1.0f - second;
			int x;
			*out++ = kernels [0] * first;
			for ( x = 1; x < kernel_size * 2; x++ )
				*out++ = kernels [x] * first + kernels [x - 1] * second;
		}
	}
	
	/* setup decoder matricies */
	{
		static float const default_decoder [6] =
			{ 0.956f, 0.621f, -0.272f, -0.647f, -1.105f, 1.702f };
		float hue = (float) setup->hue * PI;
		float sat = (float) setup->saturation + 1.0f;
		float const* decoder = setup->decoder_matrix;
		int i;
		if ( !decoder )
			decoder = default_decoder;
		else
			hue += PI / 180 * 15;
		
		for ( i = 0; i < snes_ntsc_burst_count; i++ )
		{
			float s = (float) sin( hue ) * sat;
			float c = (float) cos( hue ) * sat;
			float const* in = decoder;
			float* out = impl->to_rgb [i];
			int n;
			for ( n = 3; n; --n )
			{
				float i = *in++;
				float q = *in++;
				*out++ = i * c - q * s;
				*out++ = i * s + q * c;
			}
			hue -= PI / 180 * 120;
		}
	}
}

/* kernel generation */

enum { rgb_kernel_size = snes_ntsc_burst_size / alignment_count };

static float const rgb_offset = rgb_unit * 2 + 0.5f;
static ntsc_rgb_t const ntsc_rgb_bias = rgb_unit * 2 * ntsc_rgb_builder;

#define TO_RGB( y, i, q, to_rgb ) ( \
	((int) (y + to_rgb [0] * i + to_rgb [1] * q) << 21) +\
	((int) (y + to_rgb [2] * i + to_rgb [3] * q) << 11) +\
	((int) (y + to_rgb [4] * i + to_rgb [5] * q) <<  1)\
)

typedef struct pixel_info_t
{
	int offset;
	float negate;
	float kernel [4];
} pixel_info_t;

#define PIXEL_OFFSET_( ntsc, scaled ) \
	(kernel_size / 2 + ntsc + (scaled != 0) + (rescale_out - scaled) % rescale_out + \
			(kernel_size * 2 * scaled))

#define PIXEL_OFFSET( ntsc, scaled ) \
	PIXEL_OFFSET_( ((ntsc) - (scaled) / rescale_out * rescale_in),\
			(((scaled) + rescale_out * 10) % rescale_out) ),\
	(1.0f - (((ntsc) + 100) & 2))

/* Generate pixel at all burst phases and column alignments */
static void gen_kernel( struct ntsc_impl_t* impl, float y, float i, float q, ntsc_rgb_t* out )
{
	/* generate for each scanline burst phase */
	float const* to_rgb = impl->to_rgb [0];
	do
	{
		static pixel_info_t const pixels [alignment_count] = {
			{ PIXEL_OFFSET( -4, -9 ), { 1.0000f, 1.0000f,  .6667f,  .0000f } },
			{ PIXEL_OFFSET( -2, -7 ), {  .3333f, 1.0000f, 1.0000f,  .3333f } },
			{ PIXEL_OFFSET(  0, -5 ), {  .0000f,  .6667f, 1.0000f, 1.0000f } },
		};
		
		/* Encode yiq into *two* composite signals (to allow control over artifacting).
		Convolve these with kernels which: filter respective components, apply
		sharpening, and rescale horizontally. Convert resulting yiq to rgb and pack
		into integer. */
		pixel_info_t const* pixel = pixels;
		do
		{
			/* negate is -1 when composite starts at odd multiple of 2 */
			float const yy = y * impl->fringing * pixel->negate;
			float const ic0 = (i + yy) * pixel->kernel [0];
			float const qc1 = (q + yy) * pixel->kernel [1];
			float const ic2 = (i - yy) * pixel->kernel [2];
			float const qc3 = (q - yy) * pixel->kernel [3];
			
			float const factor = impl->artifacts * pixel->negate;
			float const ii = i * factor;
			float const yc0 = (y + ii) * pixel->kernel [0];
			float const yc2 = (y - ii) * pixel->kernel [2];
			
			float const qq = q * factor;
			float const yc1 = (y + qq) * pixel->kernel [1];
			float const yc3 = (y - qq) * pixel->kernel [3];
			
			float const* k = &impl->kernel [pixel->offset];
			int n;
			for ( n = rgb_kernel_size; n; --n )
			{
				float i = k[0]*ic0 + k[2]*ic2;
				float q = k[1]*qc1 + k[3]*qc3;
				float y = k[kernel_size+0]*yc0 + k[kernel_size+1]*yc1 +
				          k[kernel_size+2]*yc2 + k[kernel_size+3]*yc3 + rgb_offset;
				if ( k >= &impl->kernel [kernel_size * 2 * (rescale_out - 1)] )
					k -= kernel_size * 2 * (rescale_out - 1) + 2;
				else
					k += kernel_size * 2 - 1;
				*out++ = TO_RGB( y, i, q, to_rgb ) - ntsc_rgb_bias;
			}
		}
		while ( pixel++ < &pixels [alignment_count - 1] );
		
		to_rgb += 6;
		
		/* rotate -120 degrees */
		{
			float const sin_b = -0.866025f;
			float const cos_b = -0.5f;
			float t;
			t = i * cos_b - q * sin_b;
			q = i * sin_b + q * cos_b;
			i = t;
		}
	}
	while ( to_rgb < impl->to_rgb [snes_ntsc_burst_count] );
}

static void merge_fields( ntsc_rgb_t* io )
{
	int n;
	for ( n = snes_ntsc_burst_size; n; --n )
	{
		ntsc_rgb_t p0 = io [snes_ntsc_burst_size * 0] + ntsc_rgb_bias;
		ntsc_rgb_t p1 = io [snes_ntsc_burst_size * 1] + ntsc_rgb_bias;
		ntsc_rgb_t p2 = io [snes_ntsc_burst_size * 2] + ntsc_rgb_bias;
		/* merge fields without losing precision */
		io [snes_ntsc_burst_size * 0] =
				((p0 + p1 - ((p0 ^ p1) & ntsc_rgb_builder)) >> 1) - ntsc_rgb_bias;
		io [snes_ntsc_burst_size * 1] =
				((p1 + p2 - ((p1 ^ p2) & ntsc_rgb_builder)) >> 1) - ntsc_rgb_bias;
		io [snes_ntsc_burst_size * 2] =
				((p2 + p0 - ((p2 ^ p0) & ntsc_rgb_builder)) >> 1) - ntsc_rgb_bias;
		++io;
	}
}

static void correct_errors( ntsc_rgb_t color, ntsc_rgb_t* out )
{
	int burst;
	for ( burst = 0; burst < snes_ntsc_burst_count; burst++ )
	{
		int i;
		for ( i = 0; i < rgb_kernel_size / 2; i++ )
		{
			ntsc_rgb_t error = color -
					out [i           ] -
					out [i + 3    +28] -
					out [i + 5    +14] -
					out [i + 7       ] -
					out [(i+10)%14+28] -
					out [(i+12)%14+14];
			
			/* distribute error among four kernels */
			ntsc_rgb_t fourth = (error + 2 * ntsc_rgb_builder) >> 2;
			fourth &= (ntsc_rgb_bias >> 1) - ntsc_rgb_builder;
			fourth -= ntsc_rgb_bias >> 2;
			if ( disable_correction ) { out [i] += ntsc_rgb_bias; continue; }
			out [i + 3 +28] += fourth;
			out [i + 5 +14] += fourth;
			out [i + 7    ] += fourth;
			out [i        ] += error - (fourth * 3);
		}
		out += alignment_count * rgb_kernel_size;
	}
}
/* end common code */

void snes_ntsc_init( snes_ntsc_t* ntsc, snes_ntsc_setup_t const* setup )
{
	float to_float [32];
	int entry;
	struct ntsc_impl_t impl;
	if ( !setup )
		setup = &snes_ntsc_composite;
	init_ntsc_impl( &impl, setup );
	
	{
		double gamma = 1 - setup->gamma * (setup->gamma > 0 ? 0.5f : 1.5f);
		int i;
		for ( i = 0; i < 32; i++ )
			to_float [i] = (float) pow( (1 / 31.0) * i, gamma ) * rgb_unit;
	}
	
	for ( entry = 0; entry < snes_ntsc_color_count; entry++ )
	{
		/* Reduce number of significant bits of source color. Clearing the
		low bits of R and B were least notictable. Modifying green was too
		noticeable. */
		int ir = entry >> 8 & 0x1E;
		int ig = entry >> 4 & 0x1F;
		int ib = entry << 1 & 0x1E;
		
		if ( setup->bsnes_colortbl )
		{
			int bgr15 = (ib << 10) | (ig << 5) | ir;
			unsigned long rgb16 = setup->bsnes_colortbl [bgr15];
			ir = rgb16 >> 11 & 0x1E;
			ig = rgb16 >>  6 & 0x1F;
			ib = rgb16       & 0x1E;
		}
		
		{
			float r = to_float [ir];
			float g = to_float [ig];
			float b = to_float [ib];
			
			float y = r * 0.299f + g * 0.587f + b * 0.114f;
			float i = r * 0.596f - g * 0.275f - b * 0.321f;
			float q = r * 0.212f - g * 0.523f + b * 0.311f;
			
			float iq = i * q;
			if ( impl.hue_warping && q && iq <= 0 )
			{
				float factor = (iq * impl.hue_warping) / (i * i + q * q);
				i -= i * factor;
				q += q * factor;
			}
			
			y = y * impl.contrast + impl.brightness;
			
			{
				float yy = y + rgb_offset;
				ntsc_rgb_t rgb = TO_RGB( yy, i, q, impl.to_rgb [0] );
				ntsc_rgb_t* out = ntsc->table [entry];
				
				gen_kernel( &impl, y, i, q, out );
				
				if ( setup->merge_fields )
					merge_fields( out );
				
				correct_errors( rgb, out );
			}
		}
	}
}

/* Disable 'restrict' keyword by default. If your compiler supports it, put

	#define restrict restrict

somewhere in a config header, or the equivalent in the command-line:

	-Drestrict=restrict

If your compiler supports a non-standard version, like __restrict, do this:

	#define restrict __restrict

Enabling this if your compiler supports it will allow better optimization. */
#ifndef restrict
	#define restrict
#endif

/* Default to 16-bit RGB input and output */
#ifndef SNES_NTSC_IN_FORMAT
	#define SNES_NTSC_IN_FORMAT SNES_NTSC_RGB16
#endif

#ifndef SNES_NTSC_OUT_DEPTH
	#define SNES_NTSC_OUT_DEPTH 16
#endif

#include <limits.h>

#if SNES_NTSC_OUT_DEPTH > 16
	#if UINT_MAX == 0xFFFFFFFF
		typedef unsigned int  snes_ntsc_out_t;
	#elif ULONG_MAX == 0xFFFFFFFF
		typedef unsigned long snes_ntsc_out_t;
	#else
		#error "Need 32-bit int type"
	#endif
#else
	#if USHRT_MAX == 0xFFFF
		typedef unsigned short snes_ntsc_out_t;
	#else
		#error "Need 16-bit int type"
	#endif
#endif

/* useful if you have a linker which doesn't remove unused code from executable */
#ifndef SNES_NTSC_NO_BLITTERS

/* Use this as a starting point for writing your own blitter. To allow easy upgrades
to new versions of this library, put your blitter in a separate source file rather
than modifying this one directly. */

void snes_ntsc_blit( snes_ntsc_t const* ntsc, unsigned short const* snes_in,
		long in_row_width, int burst_phase, int in_width, int in_height,
		void* rgb_out, long out_pitch )
{
	int chunk_count = (in_width - 1) / snes_ntsc_in_chunk;
	for ( ; in_height; --in_height )
	{
		/* begin row and read first input pixel */
		unsigned short const* line_in = snes_in;
		SNES_NTSC_LORES_ROW( ntsc, burst_phase,
				snes_ntsc_black, snes_ntsc_black, *line_in++ );
		snes_ntsc_out_t* restrict line_out = (snes_ntsc_out_t*) rgb_out;
		int n;
		
		/* blit main chunks, each using 3 input pixels to generate 7 output pixels */
		for ( n = chunk_count; n; --n )
		{
			/* order of input and output pixels must not be altered */
			SNES_NTSC_PIXEL_IN( 0, line_in [0] );
			SNES_NTSC_LORES_OUT( 0, line_out [0], SNES_NTSC_OUT_DEPTH );
			SNES_NTSC_LORES_OUT( 1, line_out [1], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 1, line_in [1] );
			SNES_NTSC_LORES_OUT( 2, line_out [2], SNES_NTSC_OUT_DEPTH );
			SNES_NTSC_LORES_OUT( 3, line_out [3], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 2, line_in [2] );
			SNES_NTSC_LORES_OUT( 4, line_out [4], SNES_NTSC_OUT_DEPTH );
			SNES_NTSC_LORES_OUT( 5, line_out [5], SNES_NTSC_OUT_DEPTH );
			SNES_NTSC_LORES_OUT( 6, line_out [6], SNES_NTSC_OUT_DEPTH );
			
			line_in  += 3;
			line_out += 7;
		}
		
		/* you can eliminate the need for the final chunk below by padding
		input with three extra black pixels at the end of each row */
		
		/* finish final pixels without starting any new ones */
		SNES_NTSC_PIXEL_IN( 0, snes_ntsc_black );
		SNES_NTSC_LORES_OUT( 0, line_out [0], SNES_NTSC_OUT_DEPTH );
		SNES_NTSC_LORES_OUT( 1, line_out [1], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 1, snes_ntsc_black );
		SNES_NTSC_LORES_OUT( 2, line_out [2], SNES_NTSC_OUT_DEPTH );
		SNES_NTSC_LORES_OUT( 3, line_out [3], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 2, snes_ntsc_black );
		SNES_NTSC_LORES_OUT( 4, line_out [4], SNES_NTSC_OUT_DEPTH );
		SNES_NTSC_LORES_OUT( 5, line_out [5], SNES_NTSC_OUT_DEPTH );
		SNES_NTSC_LORES_OUT( 6, line_out [6], SNES_NTSC_OUT_DEPTH );
		
		/* advance burst phase and line pointers */
		burst_phase = (burst_phase + 1) % snes_ntsc_burst_count;
		snes_in += in_row_width;
		rgb_out = (char*) rgb_out + out_pitch;
	}
}

void snes_ntsc_blit_hires( snes_ntsc_t const* ntsc, unsigned short const* snes_in,
		long in_row_width, int burst_phase, int in_width, int in_height,
		void* rgb_out, long out_pitch )
{
	int chunk_count = (in_width - 2) / (snes_ntsc_in_chunk * 2);
	for ( ; in_height; --in_height )
	{
		unsigned short const* line_in = snes_in;
		SNES_NTSC_HIRES_ROW( ntsc, burst_phase,
				snes_ntsc_black, snes_ntsc_black, snes_ntsc_black, line_in [0], line_in [1] );
		snes_ntsc_out_t* restrict line_out = (snes_ntsc_out_t*) rgb_out;
		int n;
		line_in += 2;
		
		for ( n = chunk_count; n; --n )
		{
			SNES_NTSC_PIXEL_IN( 0, line_in [0] );
			SNES_NTSC_HIRES_OUT( 0, line_out [0], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 1, line_in [1] );
			SNES_NTSC_HIRES_OUT( 1, line_out [1], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 2, line_in [2] );
			SNES_NTSC_HIRES_OUT( 2, line_out [2], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 3, line_in [3] );
			SNES_NTSC_HIRES_OUT( 3, line_out [3], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 4, line_in [4] );
			SNES_NTSC_HIRES_OUT( 4, line_out [4], SNES_NTSC_OUT_DEPTH );
			
			SNES_NTSC_PIXEL_IN( 5, line_in [5] );
			SNES_NTSC_HIRES_OUT( 5, line_out [5], SNES_NTSC_OUT_DEPTH );
			SNES_NTSC_HIRES_OUT( 6, line_out [6], SNES_NTSC_OUT_DEPTH );
			
			line_in  += 6;
			line_out += 7;
		}
		
		SNES_NTSC_PIXEL_IN( 0, snes_ntsc_black );
		SNES_NTSC_HIRES_OUT( 0, line_out [0], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 1, snes_ntsc_black );
		SNES_NTSC_HIRES_OUT( 1, line_out [1], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 2, snes_ntsc_black );
		SNES_NTSC_HIRES_OUT( 2, line_out [2], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 3, snes_ntsc_black );
		SNES_NTSC_HIRES_OUT( 3, line_out [3], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 4, snes_ntsc_black );
		SNES_NTSC_HIRES_OUT( 4, line_out [4], SNES_NTSC_OUT_DEPTH );
		
		SNES_NTSC_PIXEL_IN( 5, snes_ntsc_black );
		SNES_NTSC_HIRES_OUT( 5, line_out [5], SNES_NTSC_OUT_DEPTH );
		SNES_NTSC_HIRES_OUT( 6, line_out [6], SNES_NTSC_OUT_DEPTH );
		
		burst_phase = (burst_phase + 1) % snes_ntsc_burst_count;
		snes_in += in_row_width;
		rgb_out = (char*) rgb_out + out_pitch;
	}
}

#endif

