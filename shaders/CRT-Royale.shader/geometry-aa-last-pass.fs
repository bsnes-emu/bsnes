#version 150

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////

//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

in Vertex {
   vec2 vTexCoord;
   vec2 tex_uv;
   vec4 video_and_texture_size_inv;
   vec2 output_size_inv;
   vec3 eye_pos_local;
   vec4 geom_aspect_and_overscan;
   vec3 global_to_local_row0;
   vec3 global_to_local_row1;
   vec3 global_to_local_row2;
};

out vec4 FragColor;

// USER SETTINGS BLOCK //

#define crt_gamma 2.500000
#define lcd_gamma 2.200000
#define levels_contrast 1.0
#define halation_weight 0.0
#define diffusion_weight 0.075
#define bloom_underestimate_levels 0.8
#define bloom_excess 0.000000
#define beam_min_sigma 0.020000
#define beam_max_sigma 0.300000
#define beam_spot_power 0.330000
#define beam_min_shape 2.000000
#define beam_max_shape 4.000000
#define beam_shape_power 0.250000
#define beam_horiz_filter 0.000000
#define beam_horiz_sigma 0.35
#define beam_horiz_linear_rgb_weight 1.000000
#define convergence_offset_x_r -0.000000
#define convergence_offset_x_g 0.000000
#define convergence_offset_x_b 0.000000
#define convergence_offset_y_r 0.000000
#define convergence_offset_y_g -0.000000
#define convergence_offset_y_b 0.000000
#define mask_type 1.000000
#define mask_sample_mode_desired 0.000000
#define mask_specify_num_triads 0.000000
#define mask_triad_size_desired 3.000000
#define mask_num_triads_desired 480.000000
#define aa_subpixel_r_offset_x_runtime -0.0
#define aa_subpixel_r_offset_y_runtime 0.000000
#define aa_cubic_c 0.500000
#define aa_gauss_sigma 0.500000
#define geom_mode_runtime 1.000000
#define geom_radius 2.000000
#define geom_view_dist 2.000000
#define geom_tilt_angle_x 0.000000
#define geom_tilt_angle_y 0.000000
#define geom_aspect_ratio_x 432.000000
#define geom_aspect_ratio_y 329.000000
#define geom_overscan_x 1.000000
#define geom_overscan_y 1.000000
#define border_size 0.015
#define border_darkness 2.0
#define border_compress 2.500000
#define interlace_bff 0.000000
#define interlace_1080i 0.000000

// END USER SETTINGS BLOCK //

// compatibility macros for transparently converting HLSLisms into GLSLisms
#define mul(a,b) (b*a)
#define lerp(a,b,c) mix(a,b,c)
#define saturate(c) clamp(c, 0.0, 1.0)
#define frac(x) (fract(x))
#define float2 vec2
#define float3 vec3
#define float4 vec4
#define bool2 bvec2
#define bool3 bvec3
#define bool4 bvec4
#define float2x2 mat2x2
#define float3x3 mat3x3
#define float4x4 mat4x4
#define float4x3 mat4x3
#define float2x4 mat2x4
#define IN params
#define texture_size sourceSize[0].xy
#define video_size sourceSize[0].xy
#define output_size targetSize.xy
#define frame_count phase
#define static  
#define inline  
#define const  
#define fmod(x,y) mod(x,y)
#define ddx(c) dFdx(c)
#define ddy(c) dFdy(c)
#define atan2(x,y) atan(x,y)
#define rsqrt(c) inversesqrt(c)

#define input_texture source[0]

#if defined(GL_ES)
	#define COMPAT_PRECISION mediump
#else
	#define COMPAT_PRECISION
#endif

#if __VERSION__ >= 130
	#define COMPAT_TEXTURE texture
#else
	#define COMPAT_TEXTURE texture2D
#endif

/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////

#define LAST_PASS
#define SIMULATE_CRT_ON_LCD

//#include "../user-settings.h"

/////////////////////////////  BEGIN USER-SETTINGS  ////////////////////////////

#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////

//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.

//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
    //#define DRIVERS_ALLOW_DERIVATIVES

//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
    #ifdef DRIVERS_ALLOW_DERIVATIVES
        #define DRIVERS_ALLOW_FINE_DERIVATIVES
    #endif

//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
    //#define DRIVERS_ALLOW_DYNAMIC_BRANCHES

//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
    //#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS

//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
    //#define DRIVERS_ALLOW_TEX2DLOD

//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
    //#define DRIVERS_ALLOW_TEX2DBIAS

//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
    //#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE


////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////

//  To disable a #define option, turn its line into a comment with "//."

//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
#define RUNTIME_SHADER_PARAMS_ENABLE
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
#define RUNTIME_PHOSPHOR_BLOOM_SIGMA
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
#define RUNTIME_ANTIALIAS_WEIGHTS
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
#define RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
//  Specify the tilt at runtime?  This makes things about 3% slower.
#define RUNTIME_GEOMETRY_TILT
//  Specify the geometry mode at runtime?
#define RUNTIME_GEOMETRY_MODE
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
#define FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT

//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
    #define PHOSPHOR_MASK_MANUALLY_RESIZE
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
    #define PHOSPHOR_MASK_RESIZE_LANCZOS_WINDOW
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
    #define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_3_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
    //  Here's a helpful chart:
    //  MaxTriadSize    BlurSize    MinTriadCountsByResolution
    //  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect


///////////////////////////////  USER PARAMETERS  //////////////////////////////

//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.

//  GAMMA:
    static const float crt_gamma_static = 2.5;                  //  range [1, 5]
    static const float lcd_gamma_static = 2.2;                  //  range [1, 5]

//  LEVELS MANAGEMENT:
    //  Control the final multiplicative image contrast:
    static const float levels_contrast_static = 1.0;            //  range [0, 4)
    //  We auto-dim to avoid clipping between passes and restore brightness
    //  later.  Control the dim factor here: Lower values clip less but crush
    //  blacks more (static only for now).
    static const float levels_autodim_temp = 0.5;               //  range (0, 1] default is 0.5 but that was unnecessarily dark for me, so I set it to 1.0

//  HALATION/DIFFUSION/BLOOM:
    //  Halation weight: How much energy should be lost to electrons bounding
    //  around under the CRT glass and exciting random phosphors?
    static const float halation_weight_static = 0.0;            //  range [0, 1]
    //  Refractive diffusion weight: How much light should spread/diffuse from
    //  refracting through the CRT glass?
    static const float diffusion_weight_static = 0.075;         //  range [0, 1]
    //  Underestimate brightness: Bright areas bloom more, but we can base the
    //  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
    //  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
    static const float bloom_underestimate_levels_static = 0.8; //  range [0, 5]
    //  Blur all colors more than necessary for a softer phosphor bloom?
    static const float bloom_excess_static = 0.0;               //  range [0, 1]
    //  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
    //  blurred resize of the input (convergence offsets are applied as well).
    //  There are three filter options (static option only for now):
    //  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
    //      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
    //      and beam_max_sigma is low.
    //  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
    //      always uses a static sigma regardless of beam_max_sigma or
    //      mask_num_triads_desired.
    //  2.) True 4x4 Gaussian resize: Slowest, technically correct.
    //  These options are more pronounced for the fast, unbloomed shader version.
#ifndef RADEON_FIX
    static const float bloom_approx_filter_static = 2.0;
#else
    static const float bloom_approx_filter_static = 1.0;
#endif

//  ELECTRON BEAM SCANLINE DISTRIBUTION:
    //  How many scanlines should contribute light to each pixel?  Using more
    //  scanlines is slower (especially for a generalized Gaussian) but less
    //  distorted with larger beam sigmas (especially for a pure Gaussian).  The
    //  max_beam_sigma at which the closest unused weight is guaranteed <
    //  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
    //      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
    //      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
    //      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
    //      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
    //      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
    static const float beam_num_scanlines = 3.0;                //  range [2, 6]
    //  A generalized Gaussian beam varies shape with color too, now just width.
    //  It's slower but more flexible (static option only for now).
    static const bool beam_generalized_gaussian = true;
    //  What kind of scanline antialiasing do you want?
    //  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
    //  Integrals are slow (especially for generalized Gaussians) and rarely any
    //  better than 3x antialiasing (static option only for now).
    static const float beam_antialias_level = 1.0;              //  range [0, 2]
    //  Min/max standard deviations for scanline beams: Higher values widen and
    //  soften scanlines.  Depending on other options, low min sigmas can alias.
    static const float beam_min_sigma_static = 0.02;            //  range (0, 1]
    static const float beam_max_sigma_static = 0.3;             //  range (0, 1]
    //  Beam width varies as a function of color: A power function (0) is more
    //  configurable, but a spherical function (1) gives the widest beam
    //  variability without aliasing (static option only for now).
    static const float beam_spot_shape_function = 0.0;
    //  Spot shape power: Powers <= 1 give smoother spot shapes but lower
    //  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
    static const float beam_spot_power_static = 1.0/3.0;    //  range (0, 16]
    //  Generalized Gaussian max shape parameters: Higher values give flatter
    //  scanline plateaus and steeper dropoffs, simultaneously widening and
    //  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
    //  values > ~40.0 cause artifacts with integrals.
    static const float beam_min_shape_static = 2.0;         //  range [2, 32]
    static const float beam_max_shape_static = 4.0;         //  range [2, 32]
    //  Generalized Gaussian shape power: Affects how quickly the distribution
    //  changes shape from Gaussian to steep/plateaued as color increases from 0
    //  to 1.0.  Higher powers appear softer for most colors, and lower powers
    //  appear sharper for most colors.
    static const float beam_shape_power_static = 1.0/4.0;   //  range (0, 16]
    //  What filter should be used to sample scanlines horizontally?
    //  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
    static const float beam_horiz_filter_static = 0.0;
    //  Standard deviation for horizontal Gaussian resampling:
    static const float beam_horiz_sigma_static = 0.35;      //  range (0, 2/3]
    //  Do horizontal scanline sampling in linear RGB (correct light mixing),
    //  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
    //  limiting circuitry in some CRT's), or a weighted avg.?
    static const float beam_horiz_linear_rgb_weight_static = 1.0;   //  range [0, 1]
    //  Simulate scanline misconvergence?  This needs 3x horizontal texture
    //  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
    //  later passes (static option only for now).
    static const bool beam_misconvergence = true;
    //  Convergence offsets in x/y directions for R/G/B scanline beams in units
    //  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
    static const float2 convergence_offsets_r_static = float2(0.1, 0.2);
    static const float2 convergence_offsets_g_static = float2(0.3, 0.4);
    static const float2 convergence_offsets_b_static = float2(0.5, 0.6);
    //  Detect interlacing (static option only for now)?
    static const bool interlace_detect = true;
    //  Assume 1080-line sources are interlaced?
    static const bool interlace_1080i_static = false;
    //  For interlaced sources, assume TFF (top-field first) or BFF order?
    //  (Whether this matters depends on the nature of the interlaced input.)
    static const bool interlace_bff_static = false;

//  ANTIALIASING:
    //  What AA level do you want for curvature/overscan/subpixels?  Options:
    //  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
    //  (Static option only for now)
    static const float aa_level = 12.0;                     //  range [0, 24]
    //  What antialiasing filter do you want (static option only)?  Options:
    //  0: Box (separable), 1: Box (cylindrical),
    //  2: Tent (separable), 3: Tent (cylindrical),
    //  4: Gaussian (separable), 5: Gaussian (cylindrical),
    //  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
    //  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
    //      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
    static const float aa_filter = 6.0;                     //  range [0, 9]
    //  Flip the sample grid on odd/even frames (static option only for now)?
    static const bool aa_temporal = false;
    //  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
    //  the blue offset is the negative r offset; range [0, 0.5]
    static const float2 aa_subpixel_r_offset_static = float2(-1.0/3.0, 0.0);//float2(0.0);
    //  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
    //  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
    //  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
    //  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
    //  4.) C = 0.0 is a soft spline filter.
    static const float aa_cubic_c_static = 0.5;             //  range [0, 4]
    //  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
    static const float aa_gauss_sigma_static = 0.5;     //  range [0.0625, 1.0]

//  PHOSPHOR MASK:
    //  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
    static const float mask_type_static = 1.0;                  //  range [0, 2]
    //  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
    //  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
    //      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
    //  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
    //      is halfway decent with LUT mipmapping but atrocious without it.
    //  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
    //      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
    //      This mode reuses the same masks, so triads will be enormous unless
    //      you change the mask LUT filenames in your .cgp file.
    static const float mask_sample_mode_static = 0.0;           //  range [0, 2]
    //  Prefer setting the triad size (0.0) or number on the screen (1.0)?
    //  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
    //  will always be used to calculate the full bloom sigma statically.
    static const float mask_specify_num_triads_static = 0.0;    //  range [0, 1]
    //  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
    //  triads) will be rounded to the nearest integer tile size and clamped to
    //  obey minimum size constraints (imposed to reduce downsize taps) and
    //  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
    //  To increase the size limit, double the viewport-relative scales for the
    //  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    static const float mask_triad_size_desired_static = 24.0 / 8.0;
    //  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
    //  final size will be rounded and constrained as above); default 480.0
    static const float mask_num_triads_desired_static = 480.0;
    //  How many lobes should the sinc/Lanczos resizer use?  More lobes require
    //  more samples and avoid moire a bit better, but some is unavoidable
    //  depending on the destination size (static option for now).
    static const float mask_sinc_lobes = 3.0;                   //  range [2, 4]
    //  The mask is resized using a variable number of taps in each dimension,
    //  but some Cg profiles always fetch a constant number of taps no matter
    //  what (no dynamic branching).  We can limit the maximum number of taps if
    //  we statically limit the minimum phosphor triad size.  Larger values are
    //  faster, but the limit IS enforced (static option only, forever);
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    //  TODO: Make this 1.0 and compensate with smarter sampling!
    static const float mask_min_allowed_triad_size = 2.0;

//  GEOMETRY:
    //  Geometry mode:
    //  0: Off (default), 1: Spherical mapping (like cgwg's),
    //  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
    static const float geom_mode_static = 0.0;      //  range [0, 3]
    //  Radius of curvature: Measured in units of your viewport's diagonal size.
    static const float geom_radius_static = 2.0;    //  range [1/(2*pi), 1024]
    //  View dist is the distance from the player to their physical screen, in
    //  units of the viewport's diagonal size.  It controls the field of view.
    static const float geom_view_dist_static = 2.0; //  range [0.5, 1024]
    //  Tilt angle in radians (clockwise around up and right vectors):
    static const float2 geom_tilt_angle_static = float2(0.0, 0.0);  //  range [-pi, pi]
    //  Aspect ratio: When the true viewport size is unknown, this value is used
    //  to help convert between the phosphor triad size and count, along with
    //  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
    //  this equal to Retroarch's display aspect ratio (DAR) for best results;
    //  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
    //  default (256/224)*(54/47) = 1.313069909 (see below)
    static const float geom_aspect_ratio_static = 1.313069909;
    //  Before getting into overscan, here's some general aspect ratio info:
    //  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
    //  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
    //  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
    //  Geometry processing has to "undo" the screen-space 2D DAR to calculate
    //  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
    //  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
    //  a.) Enable Retroarch's "Crop Overscan"
    //  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
    //  Real consoles use horizontal black padding in the signal, but emulators
    //  often crop this without cropping the vertical padding; a 256x224 [S]NES
    //  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
    //  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
    //      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
    //      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
    //  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
    //  without doing a. or b., but horizontal image borders will be tighter
    //  than vertical ones, messing up curvature and overscan.  Fixing the
    //  padding first corrects this.
    //  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
    //  or adjust x/y independently to e.g. readd horizontal padding, as noted
    //  above: Values < 1.0 zoom out; range (0, inf)
    static const float2 geom_overscan_static = float2(1.0, 1.0);// * 1.005 * (1.0, 240/224.0)
    //  Compute a proper pixel-space to texture-space matrix even without ddx()/
    //  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
    //  with strong curvature (static option only for now).
    static const bool geom_force_correct_tangent_matrix = true;

//  BORDERS:
    //  Rounded border size in texture uv coords:
    static const float border_size_static = 0.015;           //  range [0, 0.5]
    //  Border darkness: Moderate values darken the border smoothly, and high
    //  values make the image very dark just inside the border:
    static const float border_darkness_static = 2.0;        //  range [0, inf)
    //  Border compression: High numbers compress border transitions, narrowing
    //  the dark border area.
    static const float border_compress_static = 2.5;        //  range [1, inf)


#endif  //  USER_SETTINGS_H

////////////////////////////  END USER-SETTINGS  //////////////////////////

//#include "derived-settings-and-constants.h"

////////////////////  BEGIN DERIVED-SETTINGS-AND-CONSTANTS  ////////////////////

#ifndef DERIVED_SETTINGS_AND_CONSTANTS_H
#define DERIVED_SETTINGS_AND_CONSTANTS_H

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////

//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA


/////////////////////////////////  DESCRIPTION  ////////////////////////////////

//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.


///////////////////////////////  BEGIN INCLUDES  ///////////////////////////////

//#include "../user-settings.h"

/////////////////////////////  BEGIN USER-SETTINGS  ////////////////////////////

#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////

//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.

//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
    //#define DRIVERS_ALLOW_DERIVATIVES

//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
    #ifdef DRIVERS_ALLOW_DERIVATIVES
        #define DRIVERS_ALLOW_FINE_DERIVATIVES
    #endif

//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
    //#define DRIVERS_ALLOW_DYNAMIC_BRANCHES

//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
    //#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS

//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
    //#define DRIVERS_ALLOW_TEX2DLOD

//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
    //#define DRIVERS_ALLOW_TEX2DBIAS

//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
    //#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE


////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////

//  To disable a #define option, turn its line into a comment with "//."

//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
#define RUNTIME_SHADER_PARAMS_ENABLE
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
#define RUNTIME_PHOSPHOR_BLOOM_SIGMA
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
#define RUNTIME_ANTIALIAS_WEIGHTS
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
#define RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
//  Specify the tilt at runtime?  This makes things about 3% slower.
#define RUNTIME_GEOMETRY_TILT
//  Specify the geometry mode at runtime?
#define RUNTIME_GEOMETRY_MODE
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
#define FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT

//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
    #define PHOSPHOR_MASK_MANUALLY_RESIZE
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
    #define PHOSPHOR_MASK_RESIZE_LANCZOS_WINDOW
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
    #define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_3_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
    //  Here's a helpful chart:
    //  MaxTriadSize    BlurSize    MinTriadCountsByResolution
    //  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect


///////////////////////////////  USER PARAMETERS  //////////////////////////////

//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.

//  GAMMA:
    static const float crt_gamma_static = 2.5;                  //  range [1, 5]
    static const float lcd_gamma_static = 2.2;                  //  range [1, 5]

//  LEVELS MANAGEMENT:
    //  Control the final multiplicative image contrast:
    static const float levels_contrast_static = 1.0;            //  range [0, 4)
    //  We auto-dim to avoid clipping between passes and restore brightness
    //  later.  Control the dim factor here: Lower values clip less but crush
    //  blacks more (static only for now).
    static const float levels_autodim_temp = 0.5;               //  range (0, 1] default is 0.5 but that was unnecessarily dark for me, so I set it to 1.0

//  HALATION/DIFFUSION/BLOOM:
    //  Halation weight: How much energy should be lost to electrons bounding
    //  around under the CRT glass and exciting random phosphors?
    static const float halation_weight_static = 0.0;            //  range [0, 1]
    //  Refractive diffusion weight: How much light should spread/diffuse from
    //  refracting through the CRT glass?
    static const float diffusion_weight_static = 0.075;         //  range [0, 1]
    //  Underestimate brightness: Bright areas bloom more, but we can base the
    //  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
    //  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
    static const float bloom_underestimate_levels_static = 0.8; //  range [0, 5]
    //  Blur all colors more than necessary for a softer phosphor bloom?
    static const float bloom_excess_static = 0.0;               //  range [0, 1]
    //  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
    //  blurred resize of the input (convergence offsets are applied as well).
    //  There are three filter options (static option only for now):
    //  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
    //      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
    //      and beam_max_sigma is low.
    //  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
    //      always uses a static sigma regardless of beam_max_sigma or
    //      mask_num_triads_desired.
    //  2.) True 4x4 Gaussian resize: Slowest, technically correct.
    //  These options are more pronounced for the fast, unbloomed shader version.
#ifndef RADEON_FIX
    static const float bloom_approx_filter_static = 2.0;
#else
    static const float bloom_approx_filter_static = 1.0;
#endif

//  ELECTRON BEAM SCANLINE DISTRIBUTION:
    //  How many scanlines should contribute light to each pixel?  Using more
    //  scanlines is slower (especially for a generalized Gaussian) but less
    //  distorted with larger beam sigmas (especially for a pure Gaussian).  The
    //  max_beam_sigma at which the closest unused weight is guaranteed <
    //  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
    //      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
    //      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
    //      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
    //      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
    //      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
    static const float beam_num_scanlines = 3.0;                //  range [2, 6]
    //  A generalized Gaussian beam varies shape with color too, now just width.
    //  It's slower but more flexible (static option only for now).
    static const bool beam_generalized_gaussian = true;
    //  What kind of scanline antialiasing do you want?
    //  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
    //  Integrals are slow (especially for generalized Gaussians) and rarely any
    //  better than 3x antialiasing (static option only for now).
    static const float beam_antialias_level = 1.0;              //  range [0, 2]
    //  Min/max standard deviations for scanline beams: Higher values widen and
    //  soften scanlines.  Depending on other options, low min sigmas can alias.
    static const float beam_min_sigma_static = 0.02;            //  range (0, 1]
    static const float beam_max_sigma_static = 0.3;             //  range (0, 1]
    //  Beam width varies as a function of color: A power function (0) is more
    //  configurable, but a spherical function (1) gives the widest beam
    //  variability without aliasing (static option only for now).
    static const float beam_spot_shape_function = 0.0;
    //  Spot shape power: Powers <= 1 give smoother spot shapes but lower
    //  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
    static const float beam_spot_power_static = 1.0/3.0;    //  range (0, 16]
    //  Generalized Gaussian max shape parameters: Higher values give flatter
    //  scanline plateaus and steeper dropoffs, simultaneously widening and
    //  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
    //  values > ~40.0 cause artifacts with integrals.
    static const float beam_min_shape_static = 2.0;         //  range [2, 32]
    static const float beam_max_shape_static = 4.0;         //  range [2, 32]
    //  Generalized Gaussian shape power: Affects how quickly the distribution
    //  changes shape from Gaussian to steep/plateaued as color increases from 0
    //  to 1.0.  Higher powers appear softer for most colors, and lower powers
    //  appear sharper for most colors.
    static const float beam_shape_power_static = 1.0/4.0;   //  range (0, 16]
    //  What filter should be used to sample scanlines horizontally?
    //  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
    static const float beam_horiz_filter_static = 0.0;
    //  Standard deviation for horizontal Gaussian resampling:
    static const float beam_horiz_sigma_static = 0.35;      //  range (0, 2/3]
    //  Do horizontal scanline sampling in linear RGB (correct light mixing),
    //  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
    //  limiting circuitry in some CRT's), or a weighted avg.?
    static const float beam_horiz_linear_rgb_weight_static = 1.0;   //  range [0, 1]
    //  Simulate scanline misconvergence?  This needs 3x horizontal texture
    //  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
    //  later passes (static option only for now).
    static const bool beam_misconvergence = true;
    //  Convergence offsets in x/y directions for R/G/B scanline beams in units
    //  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
    static const float2 convergence_offsets_r_static = float2(0.1, 0.2);
    static const float2 convergence_offsets_g_static = float2(0.3, 0.4);
    static const float2 convergence_offsets_b_static = float2(0.5, 0.6);
    //  Detect interlacing (static option only for now)?
    static const bool interlace_detect = true;
    //  Assume 1080-line sources are interlaced?
    static const bool interlace_1080i_static = false;
    //  For interlaced sources, assume TFF (top-field first) or BFF order?
    //  (Whether this matters depends on the nature of the interlaced input.)
    static const bool interlace_bff_static = false;

//  ANTIALIASING:
    //  What AA level do you want for curvature/overscan/subpixels?  Options:
    //  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
    //  (Static option only for now)
    static const float aa_level = 12.0;                     //  range [0, 24]
    //  What antialiasing filter do you want (static option only)?  Options:
    //  0: Box (separable), 1: Box (cylindrical),
    //  2: Tent (separable), 3: Tent (cylindrical),
    //  4: Gaussian (separable), 5: Gaussian (cylindrical),
    //  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
    //  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
    //      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
    static const float aa_filter = 6.0;                     //  range [0, 9]
    //  Flip the sample grid on odd/even frames (static option only for now)?
    static const bool aa_temporal = false;
    //  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
    //  the blue offset is the negative r offset; range [0, 0.5]
    static const float2 aa_subpixel_r_offset_static = float2(-1.0/3.0, 0.0);//float2(0.0);
    //  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
    //  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
    //  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
    //  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
    //  4.) C = 0.0 is a soft spline filter.
    static const float aa_cubic_c_static = 0.5;             //  range [0, 4]
    //  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
    static const float aa_gauss_sigma_static = 0.5;     //  range [0.0625, 1.0]

//  PHOSPHOR MASK:
    //  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
    static const float mask_type_static = 1.0;                  //  range [0, 2]
    //  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
    //  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
    //      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
    //  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
    //      is halfway decent with LUT mipmapping but atrocious without it.
    //  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
    //      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
    //      This mode reuses the same masks, so triads will be enormous unless
    //      you change the mask LUT filenames in your .cgp file.
    static const float mask_sample_mode_static = 0.0;           //  range [0, 2]
    //  Prefer setting the triad size (0.0) or number on the screen (1.0)?
    //  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
    //  will always be used to calculate the full bloom sigma statically.
    static const float mask_specify_num_triads_static = 0.0;    //  range [0, 1]
    //  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
    //  triads) will be rounded to the nearest integer tile size and clamped to
    //  obey minimum size constraints (imposed to reduce downsize taps) and
    //  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
    //  To increase the size limit, double the viewport-relative scales for the
    //  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    static const float mask_triad_size_desired_static = 24.0 / 8.0;
    //  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
    //  final size will be rounded and constrained as above); default 480.0
    static const float mask_num_triads_desired_static = 480.0;
    //  How many lobes should the sinc/Lanczos resizer use?  More lobes require
    //  more samples and avoid moire a bit better, but some is unavoidable
    //  depending on the destination size (static option for now).
    static const float mask_sinc_lobes = 3.0;                   //  range [2, 4]
    //  The mask is resized using a variable number of taps in each dimension,
    //  but some Cg profiles always fetch a constant number of taps no matter
    //  what (no dynamic branching).  We can limit the maximum number of taps if
    //  we statically limit the minimum phosphor triad size.  Larger values are
    //  faster, but the limit IS enforced (static option only, forever);
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    //  TODO: Make this 1.0 and compensate with smarter sampling!
    static const float mask_min_allowed_triad_size = 2.0;

//  GEOMETRY:
    //  Geometry mode:
    //  0: Off (default), 1: Spherical mapping (like cgwg's),
    //  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
    static const float geom_mode_static = 0.0;      //  range [0, 3]
    //  Radius of curvature: Measured in units of your viewport's diagonal size.
    static const float geom_radius_static = 2.0;    //  range [1/(2*pi), 1024]
    //  View dist is the distance from the player to their physical screen, in
    //  units of the viewport's diagonal size.  It controls the field of view.
    static const float geom_view_dist_static = 2.0; //  range [0.5, 1024]
    //  Tilt angle in radians (clockwise around up and right vectors):
    static const float2 geom_tilt_angle_static = float2(0.0, 0.0);  //  range [-pi, pi]
    //  Aspect ratio: When the true viewport size is unknown, this value is used
    //  to help convert between the phosphor triad size and count, along with
    //  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
    //  this equal to Retroarch's display aspect ratio (DAR) for best results;
    //  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
    //  default (256/224)*(54/47) = 1.313069909 (see below)
    static const float geom_aspect_ratio_static = 1.313069909;
    //  Before getting into overscan, here's some general aspect ratio info:
    //  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
    //  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
    //  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
    //  Geometry processing has to "undo" the screen-space 2D DAR to calculate
    //  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
    //  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
    //  a.) Enable Retroarch's "Crop Overscan"
    //  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
    //  Real consoles use horizontal black padding in the signal, but emulators
    //  often crop this without cropping the vertical padding; a 256x224 [S]NES
    //  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
    //  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
    //      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
    //      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
    //  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
    //  without doing a. or b., but horizontal image borders will be tighter
    //  than vertical ones, messing up curvature and overscan.  Fixing the
    //  padding first corrects this.
    //  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
    //  or adjust x/y independently to e.g. readd horizontal padding, as noted
    //  above: Values < 1.0 zoom out; range (0, inf)
    static const float2 geom_overscan_static = float2(1.0, 1.0);// * 1.005 * (1.0, 240/224.0)
    //  Compute a proper pixel-space to texture-space matrix even without ddx()/
    //  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
    //  with strong curvature (static option only for now).
    static const bool geom_force_correct_tangent_matrix = true;

//  BORDERS:
    //  Rounded border size in texture uv coords:
    static const float border_size_static = 0.015;           //  range [0, 0.5]
    //  Border darkness: Moderate values darken the border smoothly, and high
    //  values make the image very dark just inside the border:
    static const float border_darkness_static = 2.0;        //  range [0, inf)
    //  Border compression: High numbers compress border transitions, narrowing
    //  the dark border area.
    static const float border_compress_static = 2.5;        //  range [1, inf)


#endif  //  USER_SETTINGS_H

/////////////////////////////   END USER-SETTINGS   ////////////////////////////

//#include "user-cgp-constants.h"

/////////////////////////   BEGIN USER-CGP-CONSTANTS   /////////////////////////

#ifndef USER_CGP_CONSTANTS_H
#define USER_CGP_CONSTANTS_H

//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.

//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
static const float bloom_approx_size_x = 320.0;
static const float bloom_approx_size_x_for_fake = 400.0;
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
static const float2 mask_resize_viewport_scale = float2(0.0625, 0.0625);
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
static const float geom_max_aspect_ratio = 4.0/3.0;

//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
static const float2 mask_texture_small_size = float2(64.0, 64.0);
static const float2 mask_texture_large_size = float2(512.0, 512.0);
static const float mask_triads_per_tile = 8.0;
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
static const float mask_grille14_avg_color = 50.6666666/255.0;
    //  TileableLinearApertureGrille14Wide7d33Spacing*.png
    //  TileableLinearApertureGrille14Wide10And6Spacing*.png
static const float mask_grille15_avg_color = 53.0/255.0;
    //  TileableLinearApertureGrille15Wide6d33Spacing*.png
    //  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
static const float mask_slot_avg_color = 46.0/255.0;
    //  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
    //  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
static const float mask_shadow_avg_color = 41.0/255.0;
    //  TileableLinearShadowMask*.png
    //  TileableLinearShadowMaskEDP*.png

#ifdef PHOSPHOR_MASK_GRILLE14
    static const float mask_grille_avg_color = mask_grille14_avg_color;
#else
    static const float mask_grille_avg_color = mask_grille15_avg_color;
#endif


#endif  //  USER_CGP_CONSTANTS_H

//////////////////////////   END USER-CGP-CONSTANTS   //////////////////////////

////////////////////////////////  END INCLUDES  ////////////////////////////////

///////////////////////////////  FIXED SETTINGS  ///////////////////////////////

//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
#define FIX_ZERO(c) (max(abs(c), 0.0000152587890625))   //  2^-16

//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
#ifndef SIMULATE_CRT_ON_LCD
    #define SIMULATE_CRT_ON_LCD
#endif

//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
    #define ANISOTROPIC_TILING_COMPAT_TEX2DLOD              //  129.7 FPS, 4x, flat; 101.8 at fullscreen
    #define ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE       //  128.1 FPS, 4x, flat; 101.5 at fullscreen
    #define ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES   //  124.4 FPS, 4x, flat; 97.4 at fullscreen
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
    //#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD


//////////////////////////////  DERIVED SETTINGS  //////////////////////////////

//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
#ifdef INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
    #ifdef PHOSPHOR_MASK_MANUALLY_RESIZE
        #undef PHOSPHOR_MASK_MANUALLY_RESIZE
    #endif
    #ifdef RUNTIME_GEOMETRY_MODE
        #undef RUNTIME_GEOMETRY_MODE
    #endif
    //  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
    //  inferior in most cases, so replace 2.0 with 0.0:
    static const float bloom_approx_filter =
        bloom_approx_filter_static > 1.5 ? 0.0 : bloom_approx_filter_static;
#else
    static const float bloom_approx_filter = bloom_approx_filter_static;
#endif

//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
#ifndef RUNTIME_SHADER_PARAMS_ENABLE
    #ifdef RUNTIME_PHOSPHOR_BLOOM_SIGMA
        #undef RUNTIME_PHOSPHOR_BLOOM_SIGMA
    #endif
    #ifdef RUNTIME_ANTIALIAS_WEIGHTS
        #undef RUNTIME_ANTIALIAS_WEIGHTS
    #endif
    #ifdef RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
        #undef RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
    #endif
    #ifdef RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
        #undef RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
    #endif
    #ifdef RUNTIME_GEOMETRY_TILT
        #undef RUNTIME_GEOMETRY_TILT
    #endif
    #ifdef RUNTIME_GEOMETRY_MODE
        #undef RUNTIME_GEOMETRY_MODE
    #endif
    #ifdef FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
        #undef FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
    #endif
#endif

//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #define ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
#endif
#ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
    #define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
#endif
//  Rule out unavailable anisotropic compatibility strategies:
#ifndef DRIVERS_ALLOW_DERIVATIVES
    #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
        #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
    #endif
#endif
#ifndef DRIVERS_ALLOW_TEX2DLOD
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
        #undef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #endif
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
        #undef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
    #endif
    #ifdef ANTIALIAS_DISABLE_ANISOTROPIC
        #undef ANTIALIAS_DISABLE_ANISOTROPIC
    #endif
#endif
#ifndef DRIVERS_ALLOW_TEX2DBIAS
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
    #endif
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
    #endif
#endif
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
    #endif
    #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
        #undef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
    #endif
    #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
        #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
    #endif
#else
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
        #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
            #undef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
        #endif
        #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
            #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
        #endif
    #else
        //  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
        //  flat texture coords in the same pass, but that's all we use.
        #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
            #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
                #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
            #endif
        #endif
    #endif
#endif
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #define ANISOTROPIC_TILING_COMPAT_TEX2DLOD_FAMILY
#endif
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
    #define ANISOTROPIC_TILING_COMPAT_TEX2DLOD_FAMILY
#endif
//  Prioritize anisotropic resampling compatibility strategies the same way:
#ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
    #endif
#endif


///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////

//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
#ifdef DRIVERS_ALLOW_TEX2DLOD
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
        //  TODO: Take advantage of this!
        #define PHOSPHOR_MASK_RESIZE_MIPMAPPED_LUT
        static const float2 mask_resize_src_lut_size = mask_texture_large_size;
    #else
        static const float2 mask_resize_src_lut_size = mask_texture_small_size;
    #endif
#else
    static const float2 mask_resize_src_lut_size = mask_texture_small_size;
#endif


//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
#ifdef DRIVERS_ALLOW_DYNAMIC_BRANCHES
    #define RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
#else
    #ifdef FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
        #define RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
    #endif
#endif

//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
#ifdef GEOMETRY_EARLY
        static const float max_subpixel_offset = aa_subpixel_r_offset_static.x;
        //  Most antialiasing filters have a base radius of 4.0 pixels:
        static const float max_aa_base_pixel_border = 4.0 +
            max_subpixel_offset;
#else
    static const float max_aa_base_pixel_border = 0.0;
#endif
//  Anisotropic filtering adds about 0.5 to the pixel border:
#ifndef ANISOTROPIC_TILING_COMPAT_TEX2DLOD_FAMILY
    static const float max_aniso_pixel_border = max_aa_base_pixel_border + 0.5;
#else
    static const float max_aniso_pixel_border = max_aa_base_pixel_border;
#endif
//  Fixing discontinuities adds 1.0 more to the pixel border:
#ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
    static const float max_tiled_pixel_border = max_aniso_pixel_border + 1.0;
#else
    static const float max_tiled_pixel_border = max_aniso_pixel_border;
#endif
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
#ifdef GEOMETRY_EARLY
    static const float max_mask_texel_border =
        ceil(max_tiled_pixel_border * 3.0);
#else
    static const float max_mask_texel_border = ceil(max_tiled_pixel_border);
#endif
//  Convert the texel border to a tile border using worst-case assumptions:
static const float max_mask_tile_border = max_mask_texel_border/
    (mask_min_allowed_triad_size * mask_triads_per_tile);

//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
#ifndef GEOMETRY_EARLY
    #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
        //  Special case: Render two tiles without borders.  Anisotropic
        //  filtering doesn't seem to be a problem here.
        static const float mask_resize_num_tiles = 1.0 + 1.0;
        static const float mask_start_texels = 0.0;
    #else
        static const float mask_resize_num_tiles = 1.0 +
            2.0 * max_mask_tile_border;
        static const float mask_start_texels = max_mask_texel_border;
    #endif
#else
    static const float mask_resize_num_tiles = 1.0 + 2.0*max_mask_tile_border;
    static const float mask_start_texels = max_mask_texel_border;
#endif

//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
static const float mask_resize_num_triads =
    mask_resize_num_tiles * mask_triads_per_tile;
static const float2 min_allowed_viewport_triads =
    float2(mask_resize_num_triads) / mask_resize_viewport_scale;


////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////

static const float pi = 3.141592653589;
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
static const float under_half = 0.4995;


#endif  //  DERIVED_SETTINGS_AND_CONSTANTS_H

/////////////////////////////  END DERIVED-SETTINGS-AND-CONSTANTS  ////////////////////////////

//#include "bind-shader-h"

/////////////////////////////  BEGIN BIND-SHADER-PARAMS  ////////////////////////////

#ifndef BIND_SHADER_PARAMS_H
#define BIND_SHADER_PARAMS_H

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////

//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA


/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////

///////////////////////////////  BEGIN INCLUDES  ///////////////////////////////

//#include "../user-settings.h"

/////////////////////////////  BEGIN USER-SETTINGS  ////////////////////////////

#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////

//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.

//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
    //#define DRIVERS_ALLOW_DERIVATIVES

//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
    #ifdef DRIVERS_ALLOW_DERIVATIVES
        #define DRIVERS_ALLOW_FINE_DERIVATIVES
    #endif

//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
    //#define DRIVERS_ALLOW_DYNAMIC_BRANCHES

//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
    //#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS

//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
    //#define DRIVERS_ALLOW_TEX2DLOD

//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
    //#define DRIVERS_ALLOW_TEX2DBIAS

//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
    //#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE


////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////

//  To disable a #define option, turn its line into a comment with "//."

//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
#define RUNTIME_SHADER_PARAMS_ENABLE
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
#define RUNTIME_PHOSPHOR_BLOOM_SIGMA
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
#define RUNTIME_ANTIALIAS_WEIGHTS
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
#define RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
//  Specify the tilt at runtime?  This makes things about 3% slower.
#define RUNTIME_GEOMETRY_TILT
//  Specify the geometry mode at runtime?
#define RUNTIME_GEOMETRY_MODE
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
#define FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT

//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
    #define PHOSPHOR_MASK_MANUALLY_RESIZE
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
    #define PHOSPHOR_MASK_RESIZE_LANCZOS_WINDOW
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
    #define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_3_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
    //  Here's a helpful chart:
    //  MaxTriadSize    BlurSize    MinTriadCountsByResolution
    //  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect


///////////////////////////////  USER PARAMETERS  //////////////////////////////

//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.

//  GAMMA:
    static const float crt_gamma_static = 2.5;                  //  range [1, 5]
    static const float lcd_gamma_static = 2.2;                  //  range [1, 5]

//  LEVELS MANAGEMENT:
    //  Control the final multiplicative image contrast:
    static const float levels_contrast_static = 1.0;            //  range [0, 4)
    //  We auto-dim to avoid clipping between passes and restore brightness
    //  later.  Control the dim factor here: Lower values clip less but crush
    //  blacks more (static only for now).
    static const float levels_autodim_temp = 0.5;               //  range (0, 1] default is 0.5 but that was unnecessarily dark for me, so I set it to 1.0

//  HALATION/DIFFUSION/BLOOM:
    //  Halation weight: How much energy should be lost to electrons bounding
    //  around under the CRT glass and exciting random phosphors?
    static const float halation_weight_static = 0.0;            //  range [0, 1]
    //  Refractive diffusion weight: How much light should spread/diffuse from
    //  refracting through the CRT glass?
    static const float diffusion_weight_static = 0.075;         //  range [0, 1]
    //  Underestimate brightness: Bright areas bloom more, but we can base the
    //  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
    //  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
    static const float bloom_underestimate_levels_static = 0.8; //  range [0, 5]
    //  Blur all colors more than necessary for a softer phosphor bloom?
    static const float bloom_excess_static = 0.0;               //  range [0, 1]
    //  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
    //  blurred resize of the input (convergence offsets are applied as well).
    //  There are three filter options (static option only for now):
    //  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
    //      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
    //      and beam_max_sigma is low.
    //  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
    //      always uses a static sigma regardless of beam_max_sigma or
    //      mask_num_triads_desired.
    //  2.) True 4x4 Gaussian resize: Slowest, technically correct.
    //  These options are more pronounced for the fast, unbloomed shader version.
#ifndef RADEON_FIX
    static const float bloom_approx_filter_static = 2.0;
#else
    static const float bloom_approx_filter_static = 1.0;
#endif

//  ELECTRON BEAM SCANLINE DISTRIBUTION:
    //  How many scanlines should contribute light to each pixel?  Using more
    //  scanlines is slower (especially for a generalized Gaussian) but less
    //  distorted with larger beam sigmas (especially for a pure Gaussian).  The
    //  max_beam_sigma at which the closest unused weight is guaranteed <
    //  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
    //      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
    //      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
    //      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
    //      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
    //      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
    static const float beam_num_scanlines = 3.0;                //  range [2, 6]
    //  A generalized Gaussian beam varies shape with color too, now just width.
    //  It's slower but more flexible (static option only for now).
    static const bool beam_generalized_gaussian = true;
    //  What kind of scanline antialiasing do you want?
    //  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
    //  Integrals are slow (especially for generalized Gaussians) and rarely any
    //  better than 3x antialiasing (static option only for now).
    static const float beam_antialias_level = 1.0;              //  range [0, 2]
    //  Min/max standard deviations for scanline beams: Higher values widen and
    //  soften scanlines.  Depending on other options, low min sigmas can alias.
    static const float beam_min_sigma_static = 0.02;            //  range (0, 1]
    static const float beam_max_sigma_static = 0.3;             //  range (0, 1]
    //  Beam width varies as a function of color: A power function (0) is more
    //  configurable, but a spherical function (1) gives the widest beam
    //  variability without aliasing (static option only for now).
    static const float beam_spot_shape_function = 0.0;
    //  Spot shape power: Powers <= 1 give smoother spot shapes but lower
    //  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
    static const float beam_spot_power_static = 1.0/3.0;    //  range (0, 16]
    //  Generalized Gaussian max shape parameters: Higher values give flatter
    //  scanline plateaus and steeper dropoffs, simultaneously widening and
    //  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
    //  values > ~40.0 cause artifacts with integrals.
    static const float beam_min_shape_static = 2.0;         //  range [2, 32]
    static const float beam_max_shape_static = 4.0;         //  range [2, 32]
    //  Generalized Gaussian shape power: Affects how quickly the distribution
    //  changes shape from Gaussian to steep/plateaued as color increases from 0
    //  to 1.0.  Higher powers appear softer for most colors, and lower powers
    //  appear sharper for most colors.
    static const float beam_shape_power_static = 1.0/4.0;   //  range (0, 16]
    //  What filter should be used to sample scanlines horizontally?
    //  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
    static const float beam_horiz_filter_static = 0.0;
    //  Standard deviation for horizontal Gaussian resampling:
    static const float beam_horiz_sigma_static = 0.35;      //  range (0, 2/3]
    //  Do horizontal scanline sampling in linear RGB (correct light mixing),
    //  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
    //  limiting circuitry in some CRT's), or a weighted avg.?
    static const float beam_horiz_linear_rgb_weight_static = 1.0;   //  range [0, 1]
    //  Simulate scanline misconvergence?  This needs 3x horizontal texture
    //  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
    //  later passes (static option only for now).
    static const bool beam_misconvergence = true;
    //  Convergence offsets in x/y directions for R/G/B scanline beams in units
    //  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
    static const float2 convergence_offsets_r_static = float2(0.1, 0.2);
    static const float2 convergence_offsets_g_static = float2(0.3, 0.4);
    static const float2 convergence_offsets_b_static = float2(0.5, 0.6);
    //  Detect interlacing (static option only for now)?
    static const bool interlace_detect = true;
    //  Assume 1080-line sources are interlaced?
    static const bool interlace_1080i_static = false;
    //  For interlaced sources, assume TFF (top-field first) or BFF order?
    //  (Whether this matters depends on the nature of the interlaced input.)
    static const bool interlace_bff_static = false;

//  ANTIALIASING:
    //  What AA level do you want for curvature/overscan/subpixels?  Options:
    //  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
    //  (Static option only for now)
    static const float aa_level = 12.0;                     //  range [0, 24]
    //  What antialiasing filter do you want (static option only)?  Options:
    //  0: Box (separable), 1: Box (cylindrical),
    //  2: Tent (separable), 3: Tent (cylindrical),
    //  4: Gaussian (separable), 5: Gaussian (cylindrical),
    //  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
    //  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
    //      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
    static const float aa_filter = 6.0;                     //  range [0, 9]
    //  Flip the sample grid on odd/even frames (static option only for now)?
    static const bool aa_temporal = false;
    //  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
    //  the blue offset is the negative r offset; range [0, 0.5]
    static const float2 aa_subpixel_r_offset_static = float2(-1.0/3.0, 0.0);//float2(0.0);
    //  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
    //  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
    //  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
    //  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
    //  4.) C = 0.0 is a soft spline filter.
    static const float aa_cubic_c_static = 0.5;             //  range [0, 4]
    //  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
    static const float aa_gauss_sigma_static = 0.5;     //  range [0.0625, 1.0]

//  PHOSPHOR MASK:
    //  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
    static const float mask_type_static = 1.0;                  //  range [0, 2]
    //  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
    //  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
    //      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
    //  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
    //      is halfway decent with LUT mipmapping but atrocious without it.
    //  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
    //      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
    //      This mode reuses the same masks, so triads will be enormous unless
    //      you change the mask LUT filenames in your .cgp file.
    static const float mask_sample_mode_static = 0.0;           //  range [0, 2]
    //  Prefer setting the triad size (0.0) or number on the screen (1.0)?
    //  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
    //  will always be used to calculate the full bloom sigma statically.
    static const float mask_specify_num_triads_static = 0.0;    //  range [0, 1]
    //  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
    //  triads) will be rounded to the nearest integer tile size and clamped to
    //  obey minimum size constraints (imposed to reduce downsize taps) and
    //  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
    //  To increase the size limit, double the viewport-relative scales for the
    //  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    static const float mask_triad_size_desired_static = 24.0 / 8.0;
    //  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
    //  final size will be rounded and constrained as above); default 480.0
    static const float mask_num_triads_desired_static = 480.0;
    //  How many lobes should the sinc/Lanczos resizer use?  More lobes require
    //  more samples and avoid moire a bit better, but some is unavoidable
    //  depending on the destination size (static option for now).
    static const float mask_sinc_lobes = 3.0;                   //  range [2, 4]
    //  The mask is resized using a variable number of taps in each dimension,
    //  but some Cg profiles always fetch a constant number of taps no matter
    //  what (no dynamic branching).  We can limit the maximum number of taps if
    //  we statically limit the minimum phosphor triad size.  Larger values are
    //  faster, but the limit IS enforced (static option only, forever);
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    //  TODO: Make this 1.0 and compensate with smarter sampling!
    static const float mask_min_allowed_triad_size = 2.0;

//  GEOMETRY:
    //  Geometry mode:
    //  0: Off (default), 1: Spherical mapping (like cgwg's),
    //  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
    static const float geom_mode_static = 0.0;      //  range [0, 3]
    //  Radius of curvature: Measured in units of your viewport's diagonal size.
    static const float geom_radius_static = 2.0;    //  range [1/(2*pi), 1024]
    //  View dist is the distance from the player to their physical screen, in
    //  units of the viewport's diagonal size.  It controls the field of view.
    static const float geom_view_dist_static = 2.0; //  range [0.5, 1024]
    //  Tilt angle in radians (clockwise around up and right vectors):
    static const float2 geom_tilt_angle_static = float2(0.0, 0.0);  //  range [-pi, pi]
    //  Aspect ratio: When the true viewport size is unknown, this value is used
    //  to help convert between the phosphor triad size and count, along with
    //  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
    //  this equal to Retroarch's display aspect ratio (DAR) for best results;
    //  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
    //  default (256/224)*(54/47) = 1.313069909 (see below)
    static const float geom_aspect_ratio_static = 1.313069909;
    //  Before getting into overscan, here's some general aspect ratio info:
    //  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
    //  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
    //  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
    //  Geometry processing has to "undo" the screen-space 2D DAR to calculate
    //  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
    //  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
    //  a.) Enable Retroarch's "Crop Overscan"
    //  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
    //  Real consoles use horizontal black padding in the signal, but emulators
    //  often crop this without cropping the vertical padding; a 256x224 [S]NES
    //  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
    //  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
    //      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
    //      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
    //  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
    //  without doing a. or b., but horizontal image borders will be tighter
    //  than vertical ones, messing up curvature and overscan.  Fixing the
    //  padding first corrects this.
    //  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
    //  or adjust x/y independently to e.g. readd horizontal padding, as noted
    //  above: Values < 1.0 zoom out; range (0, inf)
    static const float2 geom_overscan_static = float2(1.0, 1.0);// * 1.005 * (1.0, 240/224.0)
    //  Compute a proper pixel-space to texture-space matrix even without ddx()/
    //  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
    //  with strong curvature (static option only for now).
    static const bool geom_force_correct_tangent_matrix = true;

//  BORDERS:
    //  Rounded border size in texture uv coords:
    static const float border_size_static = 0.015;           //  range [0, 0.5]
    //  Border darkness: Moderate values darken the border smoothly, and high
    //  values make the image very dark just inside the border:
    static const float border_darkness_static = 2.0;        //  range [0, inf)
    //  Border compression: High numbers compress border transitions, narrowing
    //  the dark border area.
    static const float border_compress_static = 2.5;        //  range [1, inf)


#endif  //  USER_SETTINGS_H

/////////////////////////////   END USER-SETTINGS   ////////////////////////////

//#include "derived-settings-and-constants.h"

/////////////////////  BEGIN DERIVED-SETTINGS-AND-CONSTANTS  ////////////////////

#ifndef DERIVED_SETTINGS_AND_CONSTANTS_H
#define DERIVED_SETTINGS_AND_CONSTANTS_H

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////

//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA


/////////////////////////////////  DESCRIPTION  ////////////////////////////////

//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.


///////////////////////////////  BEGIN INCLUDES  ///////////////////////////////

//#include "../user-settings.h"

/////////////////////////////  BEGIN USER-SETTINGS  ////////////////////////////

#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////

//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.

//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
    //#define DRIVERS_ALLOW_DERIVATIVES

//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
    #ifdef DRIVERS_ALLOW_DERIVATIVES
        #define DRIVERS_ALLOW_FINE_DERIVATIVES
    #endif

//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
    //#define DRIVERS_ALLOW_DYNAMIC_BRANCHES

//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
    //#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS

//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
    //#define DRIVERS_ALLOW_TEX2DLOD

//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
    //#define DRIVERS_ALLOW_TEX2DBIAS

//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
    //#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE


////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////

//  To disable a #define option, turn its line into a comment with "//."

//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
#define RUNTIME_SHADER_PARAMS_ENABLE
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
#define RUNTIME_PHOSPHOR_BLOOM_SIGMA
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
#define RUNTIME_ANTIALIAS_WEIGHTS
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
#define RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
//  Specify the tilt at runtime?  This makes things about 3% slower.
#define RUNTIME_GEOMETRY_TILT
//  Specify the geometry mode at runtime?
#define RUNTIME_GEOMETRY_MODE
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
#define FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT

//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
    #define PHOSPHOR_MASK_MANUALLY_RESIZE
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
    #define PHOSPHOR_MASK_RESIZE_LANCZOS_WINDOW
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
    #define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_3_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
    //#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
    //  Here's a helpful chart:
    //  MaxTriadSize    BlurSize    MinTriadCountsByResolution
    //  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
    //  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect


///////////////////////////////  USER PARAMETERS  //////////////////////////////

//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.

//  GAMMA:
    static const float crt_gamma_static = 2.5;                  //  range [1, 5]
    static const float lcd_gamma_static = 2.2;                  //  range [1, 5]

//  LEVELS MANAGEMENT:
    //  Control the final multiplicative image contrast:
    static const float levels_contrast_static = 1.0;            //  range [0, 4)
    //  We auto-dim to avoid clipping between passes and restore brightness
    //  later.  Control the dim factor here: Lower values clip less but crush
    //  blacks more (static only for now).
    static const float levels_autodim_temp = 0.5;               //  range (0, 1] default is 0.5 but that was unnecessarily dark for me, so I set it to 1.0

//  HALATION/DIFFUSION/BLOOM:
    //  Halation weight: How much energy should be lost to electrons bounding
    //  around under the CRT glass and exciting random phosphors?
    static const float halation_weight_static = 0.0;            //  range [0, 1]
    //  Refractive diffusion weight: How much light should spread/diffuse from
    //  refracting through the CRT glass?
    static const float diffusion_weight_static = 0.075;         //  range [0, 1]
    //  Underestimate brightness: Bright areas bloom more, but we can base the
    //  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
    //  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
    static const float bloom_underestimate_levels_static = 0.8; //  range [0, 5]
    //  Blur all colors more than necessary for a softer phosphor bloom?
    static const float bloom_excess_static = 0.0;               //  range [0, 1]
    //  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
    //  blurred resize of the input (convergence offsets are applied as well).
    //  There are three filter options (static option only for now):
    //  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
    //      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
    //      and beam_max_sigma is low.
    //  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
    //      always uses a static sigma regardless of beam_max_sigma or
    //      mask_num_triads_desired.
    //  2.) True 4x4 Gaussian resize: Slowest, technically correct.
    //  These options are more pronounced for the fast, unbloomed shader version.
#ifndef RADEON_FIX
    static const float bloom_approx_filter_static = 2.0;
#else
    static const float bloom_approx_filter_static = 1.0;
#endif

//  ELECTRON BEAM SCANLINE DISTRIBUTION:
    //  How many scanlines should contribute light to each pixel?  Using more
    //  scanlines is slower (especially for a generalized Gaussian) but less
    //  distorted with larger beam sigmas (especially for a pure Gaussian).  The
    //  max_beam_sigma at which the closest unused weight is guaranteed <
    //  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
    //      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
    //      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
    //      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
    //      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
    //      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
    static const float beam_num_scanlines = 3.0;                //  range [2, 6]
    //  A generalized Gaussian beam varies shape with color too, now just width.
    //  It's slower but more flexible (static option only for now).
    static const bool beam_generalized_gaussian = true;
    //  What kind of scanline antialiasing do you want?
    //  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
    //  Integrals are slow (especially for generalized Gaussians) and rarely any
    //  better than 3x antialiasing (static option only for now).
    static const float beam_antialias_level = 1.0;              //  range [0, 2]
    //  Min/max standard deviations for scanline beams: Higher values widen and
    //  soften scanlines.  Depending on other options, low min sigmas can alias.
    static const float beam_min_sigma_static = 0.02;            //  range (0, 1]
    static const float beam_max_sigma_static = 0.3;             //  range (0, 1]
    //  Beam width varies as a function of color: A power function (0) is more
    //  configurable, but a spherical function (1) gives the widest beam
    //  variability without aliasing (static option only for now).
    static const float beam_spot_shape_function = 0.0;
    //  Spot shape power: Powers <= 1 give smoother spot shapes but lower
    //  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
    static const float beam_spot_power_static = 1.0/3.0;    //  range (0, 16]
    //  Generalized Gaussian max shape parameters: Higher values give flatter
    //  scanline plateaus and steeper dropoffs, simultaneously widening and
    //  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
    //  values > ~40.0 cause artifacts with integrals.
    static const float beam_min_shape_static = 2.0;         //  range [2, 32]
    static const float beam_max_shape_static = 4.0;         //  range [2, 32]
    //  Generalized Gaussian shape power: Affects how quickly the distribution
    //  changes shape from Gaussian to steep/plateaued as color increases from 0
    //  to 1.0.  Higher powers appear softer for most colors, and lower powers
    //  appear sharper for most colors.
    static const float beam_shape_power_static = 1.0/4.0;   //  range (0, 16]
    //  What filter should be used to sample scanlines horizontally?
    //  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
    static const float beam_horiz_filter_static = 0.0;
    //  Standard deviation for horizontal Gaussian resampling:
    static const float beam_horiz_sigma_static = 0.35;      //  range (0, 2/3]
    //  Do horizontal scanline sampling in linear RGB (correct light mixing),
    //  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
    //  limiting circuitry in some CRT's), or a weighted avg.?
    static const float beam_horiz_linear_rgb_weight_static = 1.0;   //  range [0, 1]
    //  Simulate scanline misconvergence?  This needs 3x horizontal texture
    //  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
    //  later passes (static option only for now).
    static const bool beam_misconvergence = true;
    //  Convergence offsets in x/y directions for R/G/B scanline beams in units
    //  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
    static const float2 convergence_offsets_r_static = float2(0.1, 0.2);
    static const float2 convergence_offsets_g_static = float2(0.3, 0.4);
    static const float2 convergence_offsets_b_static = float2(0.5, 0.6);
    //  Detect interlacing (static option only for now)?
    static const bool interlace_detect = true;
    //  Assume 1080-line sources are interlaced?
    static const bool interlace_1080i_static = false;
    //  For interlaced sources, assume TFF (top-field first) or BFF order?
    //  (Whether this matters depends on the nature of the interlaced input.)
    static const bool interlace_bff_static = false;

//  ANTIALIASING:
    //  What AA level do you want for curvature/overscan/subpixels?  Options:
    //  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
    //  (Static option only for now)
    static const float aa_level = 12.0;                     //  range [0, 24]
    //  What antialiasing filter do you want (static option only)?  Options:
    //  0: Box (separable), 1: Box (cylindrical),
    //  2: Tent (separable), 3: Tent (cylindrical),
    //  4: Gaussian (separable), 5: Gaussian (cylindrical),
    //  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
    //  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
    //      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
    static const float aa_filter = 6.0;                     //  range [0, 9]
    //  Flip the sample grid on odd/even frames (static option only for now)?
    static const bool aa_temporal = false;
    //  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
    //  the blue offset is the negative r offset; range [0, 0.5]
    static const float2 aa_subpixel_r_offset_static = float2(-1.0/3.0, 0.0);//float2(0.0);
    //  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
    //  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
    //  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
    //  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
    //  4.) C = 0.0 is a soft spline filter.
    static const float aa_cubic_c_static = 0.5;             //  range [0, 4]
    //  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
    static const float aa_gauss_sigma_static = 0.5;     //  range [0.0625, 1.0]

//  PHOSPHOR MASK:
    //  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
    static const float mask_type_static = 1.0;                  //  range [0, 2]
    //  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
    //  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
    //      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
    //  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
    //      is halfway decent with LUT mipmapping but atrocious without it.
    //  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
    //      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
    //      This mode reuses the same masks, so triads will be enormous unless
    //      you change the mask LUT filenames in your .cgp file.
    static const float mask_sample_mode_static = 0.0;           //  range [0, 2]
    //  Prefer setting the triad size (0.0) or number on the screen (1.0)?
    //  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
    //  will always be used to calculate the full bloom sigma statically.
    static const float mask_specify_num_triads_static = 0.0;    //  range [0, 1]
    //  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
    //  triads) will be rounded to the nearest integer tile size and clamped to
    //  obey minimum size constraints (imposed to reduce downsize taps) and
    //  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
    //  To increase the size limit, double the viewport-relative scales for the
    //  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    static const float mask_triad_size_desired_static = 24.0 / 8.0;
    //  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
    //  final size will be rounded and constrained as above); default 480.0
    static const float mask_num_triads_desired_static = 480.0;
    //  How many lobes should the sinc/Lanczos resizer use?  More lobes require
    //  more samples and avoid moire a bit better, but some is unavoidable
    //  depending on the destination size (static option for now).
    static const float mask_sinc_lobes = 3.0;                   //  range [2, 4]
    //  The mask is resized using a variable number of taps in each dimension,
    //  but some Cg profiles always fetch a constant number of taps no matter
    //  what (no dynamic branching).  We can limit the maximum number of taps if
    //  we statically limit the minimum phosphor triad size.  Larger values are
    //  faster, but the limit IS enforced (static option only, forever);
    //      range [1, mask_texture_small_size/mask_triads_per_tile]
    //  TODO: Make this 1.0 and compensate with smarter sampling!
    static const float mask_min_allowed_triad_size = 2.0;

//  GEOMETRY:
    //  Geometry mode:
    //  0: Off (default), 1: Spherical mapping (like cgwg's),
    //  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
    static const float geom_mode_static = 0.0;      //  range [0, 3]
    //  Radius of curvature: Measured in units of your viewport's diagonal size.
    static const float geom_radius_static = 2.0;    //  range [1/(2*pi), 1024]
    //  View dist is the distance from the player to their physical screen, in
    //  units of the viewport's diagonal size.  It controls the field of view.
    static const float geom_view_dist_static = 2.0; //  range [0.5, 1024]
    //  Tilt angle in radians (clockwise around up and right vectors):
    static const float2 geom_tilt_angle_static = float2(0.0, 0.0);  //  range [-pi, pi]
    //  Aspect ratio: When the true viewport size is unknown, this value is used
    //  to help convert between the phosphor triad size and count, along with
    //  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
    //  this equal to Retroarch's display aspect ratio (DAR) for best results;
    //  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
    //  default (256/224)*(54/47) = 1.313069909 (see below)
    static const float geom_aspect_ratio_static = 1.313069909;
    //  Before getting into overscan, here's some general aspect ratio info:
    //  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
    //  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
    //  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
    //  Geometry processing has to "undo" the screen-space 2D DAR to calculate
    //  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
    //  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
    //  a.) Enable Retroarch's "Crop Overscan"
    //  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
    //  Real consoles use horizontal black padding in the signal, but emulators
    //  often crop this without cropping the vertical padding; a 256x224 [S]NES
    //  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
    //  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
    //      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
    //      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
    //  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
    //  without doing a. or b., but horizontal image borders will be tighter
    //  than vertical ones, messing up curvature and overscan.  Fixing the
    //  padding first corrects this.
    //  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
    //  or adjust x/y independently to e.g. readd horizontal padding, as noted
    //  above: Values < 1.0 zoom out; range (0, inf)
    static const float2 geom_overscan_static = float2(1.0, 1.0);// * 1.005 * (1.0, 240/224.0)
    //  Compute a proper pixel-space to texture-space matrix even without ddx()/
    //  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
    //  with strong curvature (static option only for now).
    static const bool geom_force_correct_tangent_matrix = true;

//  BORDERS:
    //  Rounded border size in texture uv coords:
    static const float border_size_static = 0.015;           //  range [0, 0.5]
    //  Border darkness: Moderate values darken the border smoothly, and high
    //  values make the image very dark just inside the border:
    static const float border_darkness_static = 2.0;        //  range [0, inf)
    //  Border compression: High numbers compress border transitions, narrowing
    //  the dark border area.
    static const float border_compress_static = 2.5;        //  range [1, inf)


#endif  //  USER_SETTINGS_H

/////////////////////////////   END USER-SETTINGS   ////////////////////////////

//#include "user-cgp-constants.h"

/////////////////////////   BEGIN USER-CGP-CONSTANTS   /////////////////////////

#ifndef USER_CGP_CONSTANTS_H
#define USER_CGP_CONSTANTS_H

//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.

//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
static const float bloom_approx_size_x = 320.0;
static const float bloom_approx_size_x_for_fake = 400.0;
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
static const float2 mask_resize_viewport_scale = float2(0.0625, 0.0625);
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
static const float geom_max_aspect_ratio = 4.0/3.0;

//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
static const float2 mask_texture_small_size = float2(64.0, 64.0);
static const float2 mask_texture_large_size = float2(512.0, 512.0);
static const float mask_triads_per_tile = 8.0;
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
static const float mask_grille14_avg_color = 50.6666666/255.0;
    //  TileableLinearApertureGrille14Wide7d33Spacing*.png
    //  TileableLinearApertureGrille14Wide10And6Spacing*.png
static const float mask_grille15_avg_color = 53.0/255.0;
    //  TileableLinearApertureGrille15Wide6d33Spacing*.png
    //  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
static const float mask_slot_avg_color = 46.0/255.0;
    //  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
    //  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
static const float mask_shadow_avg_color = 41.0/255.0;
    //  TileableLinearShadowMask*.png
    //  TileableLinearShadowMaskEDP*.png

#ifdef PHOSPHOR_MASK_GRILLE14
    static const float mask_grille_avg_color = mask_grille14_avg_color;
#else
    static const float mask_grille_avg_color = mask_grille15_avg_color;
#endif


#endif  //  USER_CGP_CONSTANTS_H

//////////////////////////   END USER-CGP-CONSTANTS   //////////////////////////

////////////////////////////////  END INCLUDES  ////////////////////////////////

///////////////////////////////  FIXED SETTINGS  ///////////////////////////////

//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
#define FIX_ZERO(c) (max(abs(c), 0.0000152587890625))   //  2^-16

//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
#ifndef SIMULATE_CRT_ON_LCD
    #define SIMULATE_CRT_ON_LCD
#endif

//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
    #define ANISOTROPIC_TILING_COMPAT_TEX2DLOD              //  129.7 FPS, 4x, flat; 101.8 at fullscreen
    #define ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE       //  128.1 FPS, 4x, flat; 101.5 at fullscreen
    #define ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES   //  124.4 FPS, 4x, flat; 97.4 at fullscreen
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
    //#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD


//////////////////////////////  DERIVED SETTINGS  //////////////////////////////

//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
#ifdef INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
    #ifdef PHOSPHOR_MASK_MANUALLY_RESIZE
        #undef PHOSPHOR_MASK_MANUALLY_RESIZE
    #endif
    #ifdef RUNTIME_GEOMETRY_MODE
        #undef RUNTIME_GEOMETRY_MODE
    #endif
    //  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
    //  inferior in most cases, so replace 2.0 with 0.0:
    static const float bloom_approx_filter =
        bloom_approx_filter_static > 1.5 ? 0.0 : bloom_approx_filter_static;
#else
    static const float bloom_approx_filter = bloom_approx_filter_static;
#endif

//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
#ifndef RUNTIME_SHADER_PARAMS_ENABLE
    #ifdef RUNTIME_PHOSPHOR_BLOOM_SIGMA
        #undef RUNTIME_PHOSPHOR_BLOOM_SIGMA
    #endif
    #ifdef RUNTIME_ANTIALIAS_WEIGHTS
        #undef RUNTIME_ANTIALIAS_WEIGHTS
    #endif
    #ifdef RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
        #undef RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
    #endif
    #ifdef RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
        #undef RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
    #endif
    #ifdef RUNTIME_GEOMETRY_TILT
        #undef RUNTIME_GEOMETRY_TILT
    #endif
    #ifdef RUNTIME_GEOMETRY_MODE
        #undef RUNTIME_GEOMETRY_MODE
    #endif
    #ifdef FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
        #undef FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
    #endif
#endif

//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #define ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
#endif
#ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
    #define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
#endif
//  Rule out unavailable anisotropic compatibility strategies:
#ifndef DRIVERS_ALLOW_DERIVATIVES
    #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
        #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
    #endif
#endif
#ifndef DRIVERS_ALLOW_TEX2DLOD
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
        #undef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #endif
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
        #undef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
    #endif
    #ifdef ANTIALIAS_DISABLE_ANISOTROPIC
        #undef ANTIALIAS_DISABLE_ANISOTROPIC
    #endif
#endif
#ifndef DRIVERS_ALLOW_TEX2DBIAS
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
    #endif
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
    #endif
#endif
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
    #endif
    #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
        #undef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
    #endif
    #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
        #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
    #endif
#else
    #ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
        #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
            #undef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
        #endif
        #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
            #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
        #endif
    #else
        //  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
        //  flat texture coords in the same pass, but that's all we use.
        #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
            #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
                #undef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
            #endif
        #endif
    #endif
#endif
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DLOD
    #define ANISOTROPIC_TILING_COMPAT_TEX2DLOD_FAMILY
#endif
#ifdef ANISOTROPIC_TILING_COMPAT_TEX2DBIAS
    #define ANISOTROPIC_TILING_COMPAT_TEX2DLOD_FAMILY
#endif
//  Prioritize anisotropic resampling compatibility strategies the same way:
#ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
        #undef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
    #endif
#endif


///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////

//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
#ifdef DRIVERS_ALLOW_TEX2DLOD
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
        //  TODO: Take advantage of this!
        #define PHOSPHOR_MASK_RESIZE_MIPMAPPED_LUT
        static const float2 mask_resize_src_lut_size = mask_texture_large_size;
    #else
        static const float2 mask_resize_src_lut_size = mask_texture_small_size;
    #endif
#else
    static const float2 mask_resize_src_lut_size = mask_texture_small_size;
#endif


//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
#ifdef DRIVERS_ALLOW_DYNAMIC_BRANCHES
    #define RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
#else
    #ifdef FORCE_RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
        #define RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
    #endif
#endif

//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
#ifdef GEOMETRY_EARLY
        static const float max_subpixel_offset = aa_subpixel_r_offset_static.x;
        //  Most antialiasing filters have a base radius of 4.0 pixels:
        static const float max_aa_base_pixel_border = 4.0 +
            max_subpixel_offset;
#else
    static const float max_aa_base_pixel_border = 0.0;
#endif
//  Anisotropic filtering adds about 0.5 to the pixel border:
#ifndef ANISOTROPIC_TILING_COMPAT_TEX2DLOD_FAMILY
    static const float max_aniso_pixel_border = max_aa_base_pixel_border + 0.5;
#else
    static const float max_aniso_pixel_border = max_aa_base_pixel_border;
#endif
//  Fixing discontinuities adds 1.0 more to the pixel border:
#ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
    static const float max_tiled_pixel_border = max_aniso_pixel_border + 1.0;
#else
    static const float max_tiled_pixel_border = max_aniso_pixel_border;
#endif
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
#ifdef GEOMETRY_EARLY
    static const float max_mask_texel_border =
        ceil(max_tiled_pixel_border * 3.0);
#else
    static const float max_mask_texel_border = ceil(max_tiled_pixel_border);
#endif
//  Convert the texel border to a tile border using worst-case assumptions:
static const float max_mask_tile_border = max_mask_texel_border/
    (mask_min_allowed_triad_size * mask_triads_per_tile);

//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
#ifndef GEOMETRY_EARLY
    #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
        //  Special case: Render two tiles without borders.  Anisotropic
        //  filtering doesn't seem to be a problem here.
        static const float mask_resize_num_tiles = 1.0 + 1.0;
        static const float mask_start_texels = 0.0;
    #else
        static const float mask_resize_num_tiles = 1.0 +
            2.0 * max_mask_tile_border;
        static const float mask_start_texels = max_mask_texel_border;
    #endif
#else
    static const float mask_resize_num_tiles = 1.0 + 2.0*max_mask_tile_border;
    static const float mask_start_texels = max_mask_texel_border;
#endif

//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
static const float mask_resize_num_triads =
    mask_resize_num_tiles * mask_triads_per_tile;
static const float2 min_allowed_viewport_triads =
    float2(mask_resize_num_triads) / mask_resize_viewport_scale;


////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////

static const float pi = 3.141592653589;
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
static const float under_half = 0.4995;


#endif  //  DERIVED_SETTINGS_AND_CONSTANTS_H

////////////////////   END DERIVED-SETTINGS-AND-CONSTANTS   /////////////////////

////////////////////////////////  END INCLUDES  ////////////////////////////////

//  Override some parameters for gamma-management.h and tex2Dantialias.h:
#define OVERRIDE_DEVICE_GAMMA
static const float gba_gamma = 3.5; //  Irrelevant but necessary to define.
#define ANTIALIAS_OVERRIDE_BASICS
#define ANTIALIAS_OVERRIDE_PARAMETERS

//  Provide accessors for vector constants that pack scalar uniforms:
inline float2 get_aspect_vector(const float geom_aspect_ratio)
{
    //  Get an aspect ratio vector.  Enforce geom_max_aspect_ratio, and prevent
    //  the absolute scale from affecting the uv-mapping for curvature:
    const float geom_clamped_aspect_ratio =
        min(geom_aspect_ratio, geom_max_aspect_ratio);
    const float2 geom_aspect =
        normalize(float2(geom_clamped_aspect_ratio, 1.0));
    return geom_aspect;
}

inline float2 get_geom_overscan_vector()
{
    return float2(geom_overscan_x, geom_overscan_y);
}

inline float2 get_geom_tilt_angle_vector()
{
    return float2(geom_tilt_angle_x, geom_tilt_angle_y);
}

inline float3 get_convergence_offsets_x_vector()
{
    return float3(convergence_offset_x_r, convergence_offset_x_g,
        convergence_offset_x_b);
}

inline float3 get_convergence_offsets_y_vector()
{
    return float3(convergence_offset_y_r, convergence_offset_y_g,
        convergence_offset_y_b);
}

inline float2 get_convergence_offsets_r_vector()
{
    return float2(convergence_offset_x_r, convergence_offset_y_r);
}

inline float2 get_convergence_offsets_g_vector()
{
    return float2(convergence_offset_x_g, convergence_offset_y_g);
}

inline float2 get_convergence_offsets_b_vector()
{
    return float2(convergence_offset_x_b, convergence_offset_y_b);
}

inline float2 get_aa_subpixel_r_offset()
{
    #ifdef RUNTIME_ANTIALIAS_WEIGHTS
        #ifdef RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
            //  WARNING: THIS IS EXTREMELY EXPENSIVE.
            return float2(aa_subpixel_r_offset_x_runtime,
                aa_subpixel_r_offset_y_runtime);
        #else
            return aa_subpixel_r_offset_static;
        #endif
    #else
        return aa_subpixel_r_offset_static;
    #endif
}

//  Provide accessors settings which still need "cooking:"
inline float get_mask_amplify()
{
    static const float mask_grille_amplify = 1.0/mask_grille_avg_color;
    static const float mask_slot_amplify = 1.0/mask_slot_avg_color;
    static const float mask_shadow_amplify = 1.0/mask_shadow_avg_color;
    return mask_type < 0.5 ? mask_grille_amplify :
        mask_type < 1.5 ? mask_slot_amplify :
        mask_shadow_amplify;
}

inline float get_mask_sample_mode()
{
    #ifdef RUNTIME_PHOSPHOR_MASK_MODE_TYPE_SELECT
        #ifdef PHOSPHOR_MASK_MANUALLY_RESIZE
            return mask_sample_mode_desired;
        #else
            return clamp(mask_sample_mode_desired, 1.0, 2.0);
        #endif
    #else
        #ifdef PHOSPHOR_MASK_MANUALLY_RESIZE
            return mask_sample_mode_static;
        #else
            return clamp(mask_sample_mode_static, 1.0, 2.0);
        #endif
    #endif
}

#endif  //  BIND_SHADER_PARAMS_H

////////////////////////////  END BIND-SHADER-PARAMS  ///////////////////////////

#ifndef RUNTIME_GEOMETRY_TILT
    //  Create a local-to-global rotation matrix for the CRT's coordinate frame
    //  and its global-to-local inverse.  See the vertex shader for details.
    //  It's faster to compute these statically if possible.
    static const float2 sin_tilt = sin(geom_tilt_angle_static);
    static const float2 cos_tilt = cos(geom_tilt_angle_static);
    static const float3x3 geom_local_to_global_static = float3x3(
        cos_tilt.x, sin_tilt.y*sin_tilt.x, cos_tilt.y*sin_tilt.x,
        0.0, cos_tilt.y, -sin_tilt.y,
        -sin_tilt.x, sin_tilt.y*cos_tilt.x, cos_tilt.y*cos_tilt.x);
    static const float3x3 geom_global_to_local_static = float3x3(
        cos_tilt.x, 0.0, -sin_tilt.x,
        sin_tilt.y*sin_tilt.x, cos_tilt.y, sin_tilt.y*cos_tilt.x,
        cos_tilt.y*sin_tilt.x, -sin_tilt.y, cos_tilt.y*cos_tilt.x);
#endif

//////////////////////////////////  INCLUDES  //////////////////////////////////

//#include "../../../../include/gamma-management.h"

////////////////////////////  BEGIN GAMMA-MANAGEMENT  //////////////////////////

#ifndef GAMMA_MANAGEMENT_H
#define GAMMA_MANAGEMENT_H

/////////////////////////////////  MIT LICENSE  ////////////////////////////////

//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.

/////////////////////////////////  DESCRIPTION  ////////////////////////////////

//  This file provides gamma-aware tex*D*() and encode_output() functions.
//  Requires:   Before #include-ing this file, the including file must #define
//              the following macros when applicable and follow their rules:
//              1.) #define FIRST_PASS if this is the first pass.
//              2.) #define LAST_PASS if this is the last pass.
//              3.) If sRGB is available, set srgb_framebufferN = "true" for
//                  every pass except the last in your .cgp preset.
//              4.) If sRGB isn't available but you want gamma-correctness with
//                  no banding, #define GAMMA_ENCODE_EVERY_FBO each pass.
//              5.) #define SIMULATE_CRT_ON_LCD if desired (precedence over 5-7)
//              6.) #define SIMULATE_GBA_ON_LCD if desired (precedence over 6-7)
//              7.) #define SIMULATE_LCD_ON_CRT if desired (precedence over 7)
//              8.) #define SIMULATE_GBA_ON_CRT if desired (precedence over -)
//              If an option in [5, 8] is #defined in the first or last pass, it
//              should be #defined for both.  It shouldn't make a difference
//              whether it's #defined for intermediate passes or not.
//  Optional:   The including file (or an earlier included file) may optionally
//              #define a number of macros indicating it will override certain
//              macros and associated constants are as follows:
//              static constants with either static or uniform constants.  The
//              1.) OVERRIDE_STANDARD_GAMMA: The user must first define:
//                  static const float ntsc_gamma
//                  static const float pal_gamma
//                  static const float crt_reference_gamma_high
//                  static const float crt_reference_gamma_low
//                  static const float lcd_reference_gamma
//                  static const float crt_office_gamma
//                  static const float lcd_office_gamma
//              2.) OVERRIDE_DEVICE_GAMMA: The user must first define:
//                  static const float crt_gamma
//                  static const float gba_gamma
//                  static const float lcd_gamma
//              3.) OVERRIDE_FINAL_GAMMA: The user must first define:
//                  static const float input_gamma
//                  static const float intermediate_gamma
//                  static const float output_gamma
//                  (intermediate_gamma is for GAMMA_ENCODE_EVERY_FBO.)
//              4.) OVERRIDE_ALPHA_ASSUMPTIONS: The user must first define:
//                  static const bool assume_opaque_alpha
//              The gamma constant overrides must be used in every pass or none,
//              and OVERRIDE_FINAL_GAMMA bypasses all of the SIMULATE* macros.
//              OVERRIDE_ALPHA_ASSUMPTIONS may be set on a per-pass basis.
//  Usage:      After setting macros appropriately, ignore gamma correction and
//              replace all tex*D*() calls with equivalent gamma-aware
//              tex*D*_linearize calls, except:
//              1.) When you read an LUT, use regular tex*D or a gamma-specified
//                  function, depending on its gamma encoding:
//                      tex*D*_linearize_gamma (takes a runtime gamma parameter)
//              2.) If you must read pass0's original input in a later pass, use
//                  tex2D_linearize_ntsc_gamma.  If you want to read pass0's
//                  input with gamma-corrected bilinear filtering, consider
//                  creating a first linearizing pass and reading from the input
//                  of pass1 later.
//              Then, return encode_output(color) from every fragment shader.
//              Finally, use the global gamma_aware_bilinear boolean if you want
//              to statically branch based on whether bilinear filtering is
//              gamma-correct or not (e.g. for placing Gaussian blur samples).
//
//  Detailed Policy:
//  tex*D*_linearize() functions enforce a consistent gamma-management policy
//  based on the FIRST_PASS and GAMMA_ENCODE_EVERY_FBO settings.  They assume
//  their input texture has the same encoding characteristics as the input for
//  the current pass (which doesn't apply to the exceptions listed above).
//  Similarly, encode_output() enforces a policy based on the LAST_PASS and
//  GAMMA_ENCODE_EVERY_FBO settings.  Together, they result in one of the
//  following two pipelines.
//  Typical pipeline with intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = linear_color;     //  Automatic sRGB encoding
//      linear_color = intermediate_output;     //  Automatic sRGB decoding
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Typical pipeline without intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = pow(linear_color, 1.0/intermediate_gamma);
//      linear_color = pow(intermediate_output, intermediate_gamma);
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Using GAMMA_ENCODE_EVERY_FBO is much slower, but it's provided as a way to
//  easily get gamma-correctness without banding on devices where sRGB isn't
//  supported.
//
//  Use This Header to Maximize Code Reuse:
//  The purpose of this header is to provide a consistent interface for texture
//  reads and output gamma-encoding that localizes and abstracts away all the
//  annoying details.  This greatly reduces the amount of code in each shader
//  pass that depends on the pass number in the .cgp preset or whether sRGB
//  FBO's are being used: You can trivially change the gamma behavior of your
//  whole pass by commenting or uncommenting 1-3 #defines.  To reuse the same
//  code in your first, Nth, and last passes, you can even put it all in another
//  header file and #include it from skeleton .cg files that #define the
//  appropriate pass-specific settings.
//
//  Rationale for Using Three Macros:
//  This file uses GAMMA_ENCODE_EVERY_FBO instead of an opposite macro like
//  SRGB_PIPELINE to ensure sRGB is assumed by default, which hopefully imposes
//  a lower maintenance burden on each pass.  At first glance it seems we could
//  accomplish everything with two macros: GAMMA_CORRECT_IN / GAMMA_CORRECT_OUT.
//  This works for simple use cases where input_gamma == output_gamma, but it
//  breaks down for more complex scenarios like CRT simulation, where the pass
//  number determines the gamma encoding of the input and output.


///////////////////////////////  BASE CONSTANTS  ///////////////////////////////

//  Set standard gamma constants, but allow users to override them:
#ifndef OVERRIDE_STANDARD_GAMMA
    //  Standard encoding gammas:
    static const float ntsc_gamma = 2.2;    //  Best to use NTSC for PAL too?
    static const float pal_gamma = 2.8;     //  Never actually 2.8 in practice
    //  Typical device decoding gammas (only use for emulating devices):
    //  CRT/LCD reference gammas are higher than NTSC and Rec.709 video standard
    //  gammas: The standards purposely undercorrected for an analog CRT's
    //  assumed 2.5 reference display gamma to maintain contrast in assumed
    //  [dark] viewing conditions: http://www.poynton.com/PDFs/GammaFAQ.pdf
    //  These unstated assumptions about display gamma and perceptual rendering
    //  intent caused a lot of confusion, and more modern CRT's seemed to target
    //  NTSC 2.2 gamma with circuitry.  LCD displays seem to have followed suit
    //  (they struggle near black with 2.5 gamma anyway), especially PC/laptop
    //  displays designed to view sRGB in bright environments.  (Standards are
    //  also in flux again with BT.1886, but it's underspecified for displays.)
    static const float crt_reference_gamma_high = 2.5;  //  In (2.35, 2.55)
    static const float crt_reference_gamma_low = 2.35;  //  In (2.35, 2.55)
    static const float lcd_reference_gamma = 2.5;       //  To match CRT
    static const float crt_office_gamma = 2.2;  //  Circuitry-adjusted for NTSC
    static const float lcd_office_gamma = 2.2;  //  Approximates sRGB
#endif  //  OVERRIDE_STANDARD_GAMMA

//  Assuming alpha == 1.0 might make it easier for users to avoid some bugs,
//  but only if they're aware of it.
#ifndef OVERRIDE_ALPHA_ASSUMPTIONS
    static const bool assume_opaque_alpha = false;
#endif


///////////////////////  DERIVED CONSTANTS AS FUNCTIONS  ///////////////////////

//  gamma-management.h should be compatible with overriding gamma values with
//  runtime user parameters, but we can only define other global constants in
//  terms of static constants, not uniform user parameters.  To get around this
//  limitation, we need to define derived constants using functions.

//  Set device gamma constants, but allow users to override them:
#ifdef OVERRIDE_DEVICE_GAMMA
    //  The user promises to globally define the appropriate constants:
    inline float get_crt_gamma()    {   return crt_gamma;   }
    inline float get_gba_gamma()    {   return gba_gamma;   }
    inline float get_lcd_gamma()    {   return lcd_gamma;   }
#else
    inline float get_crt_gamma()    {   return crt_reference_gamma_high;    }
    inline float get_gba_gamma()    {   return 3.5; }   //  Game Boy Advance; in (3.0, 4.0)
    inline float get_lcd_gamma()    {   return lcd_office_gamma;            }
#endif  //  OVERRIDE_DEVICE_GAMMA

//  Set decoding/encoding gammas for the first/lass passes, but allow overrides:
#ifdef OVERRIDE_FINAL_GAMMA
    //  The user promises to globally define the appropriate constants:
    inline float get_intermediate_gamma()   {   return intermediate_gamma;  }
    inline float get_input_gamma()          {   return input_gamma;         }
    inline float get_output_gamma()         {   return output_gamma;        }
#else
    //  If we gamma-correct every pass, always use ntsc_gamma between passes to
    //  ensure middle passes don't need to care if anything is being simulated:
    inline float get_intermediate_gamma()   {   return ntsc_gamma;          }
    #ifdef SIMULATE_CRT_ON_LCD
        inline float get_input_gamma()      {   return get_crt_gamma();     }
        inline float get_output_gamma()     {   return get_lcd_gamma();     }
    #else
    #ifdef SIMULATE_GBA_ON_LCD
        inline float get_input_gamma()      {   return get_gba_gamma();     }
        inline float get_output_gamma()     {   return get_lcd_gamma();     }
    #else
    #ifdef SIMULATE_LCD_ON_CRT
        inline float get_input_gamma()      {   return get_lcd_gamma();     }
        inline float get_output_gamma()     {   return get_crt_gamma();     }
    #else
    #ifdef SIMULATE_GBA_ON_CRT
        inline float get_input_gamma()      {   return get_gba_gamma();     }
        inline float get_output_gamma()     {   return get_crt_gamma();     }
    #else   //  Don't simulate anything:
        inline float get_input_gamma()      {   return ntsc_gamma;          }
        inline float get_output_gamma()     {   return ntsc_gamma;          }
    #endif  //  SIMULATE_GBA_ON_CRT
    #endif  //  SIMULATE_LCD_ON_CRT
    #endif  //  SIMULATE_GBA_ON_LCD
    #endif  //  SIMULATE_CRT_ON_LCD
#endif  //  OVERRIDE_FINAL_GAMMA

//  Set decoding/encoding gammas for the current pass.  Use static constants for
//  linearize_input and gamma_encode_output, because they aren't derived, and
//  they let the compiler do dead-code elimination.
#ifndef GAMMA_ENCODE_EVERY_FBO
    #ifdef FIRST_PASS
        static const bool linearize_input = true;
        inline float get_pass_input_gamma()     {   return get_input_gamma();   }
    #else
        static const bool linearize_input = false;
        inline float get_pass_input_gamma()     {   return 1.0;                 }
    #endif
    #ifdef LAST_PASS
        static const bool gamma_encode_output = true;
        inline float get_pass_output_gamma()    {   return get_output_gamma();  }
    #else
        static const bool gamma_encode_output = false;
        inline float get_pass_output_gamma()    {   return 1.0;                 }
    #endif
#else
    static const bool linearize_input = true;
    static const bool gamma_encode_output = true;
    #ifdef FIRST_PASS
        inline float get_pass_input_gamma()     {   return get_input_gamma();   }
    #else
        inline float get_pass_input_gamma()     {   return get_intermediate_gamma();    }
    #endif
    #ifdef LAST_PASS
        inline float get_pass_output_gamma()    {   return get_output_gamma();  }
    #else
        inline float get_pass_output_gamma()    {   return get_intermediate_gamma();    }
    #endif
#endif

//  Users might want to know if bilinear filtering will be gamma-correct:
static const bool gamma_aware_bilinear = !linearize_input;


//////////////////////  COLOR ENCODING/DECODING FUNCTIONS  /////////////////////

inline float4 encode_output(const float4 color)
{
    if(gamma_encode_output)
    {
        if(assume_opaque_alpha)
        {
            return float4(pow(color.rgb, float3(1.0/get_pass_output_gamma())), 1.0);
        }
        else
        {
            return float4(pow(color.rgb, float3(1.0/get_pass_output_gamma())), color.a);
        }
    }
    else
    {
        return color;
    }
}

inline float4 decode_input(const float4 color)
{
    if(linearize_input)
    {
        if(assume_opaque_alpha)
        {
            return float4(pow(color.rgb, float3(get_pass_input_gamma())), 1.0);
        }
        else
        {
            return float4(pow(color.rgb, float3(get_pass_input_gamma())), color.a);
        }
    }
    else
    {
        return color;
    }
}

inline float4 decode_gamma_input(const float4 color, const float3 gamma)
{
    if(assume_opaque_alpha)
    {
        return float4(pow(color.rgb, gamma), 1.0);
    }
    else
    {
        return float4(pow(color.rgb, gamma), color.a);
    }
}

//TODO/FIXME: I have no idea why replacing the lookup wrappers with this macro fixes the blurs being offset ¯\_(ツ)_/¯
//#define tex2D_linearize(C, D) decode_input(vec4(COMPAT_TEXTURE(C, D)))
// EDIT: it's the 'const' in front of the coords that's doing it

///////////////////////////  TEXTURE LOOKUP WRAPPERS  //////////////////////////

//  "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a wide array of linearizing texture lookup wrapper functions.  The
//  Cg shader spec Retroarch uses only allows for 2D textures, but 1D and 3D
//  lookups are provided for completeness in case that changes someday.  Nobody
//  is likely to use the *fetch and *proj functions, but they're included just
//  in case.  The only tex*D texture sampling functions omitted are:
//      - tex*Dcmpbias
//      - tex*Dcmplod
//      - tex*DARRAY*
//      - tex*DMS*
//      - Variants returning integers
//  Standard line length restrictions are ignored below for vertical brevity.
/*
//  tex1D:
inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

//  tex1Dbias:
inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dbias(tex, tex_coords));   }

inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dbias(tex, tex_coords, texel_off));    }

//  tex1Dfetch:
inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords)
{   return decode_input(tex1Dfetch(tex, tex_coords));  }

inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex1Dfetch(tex, tex_coords, texel_off));   }

//  tex1Dlod:
inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dlod(tex, tex_coords));    }

inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dlod(tex, tex_coords, texel_off));     }

//  tex1Dproj:
inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }
*/
//  tex2D:
inline float4 tex2D_linearize(const sampler2D tex, float2 tex_coords)
{   return decode_input(COMPAT_TEXTURE(tex, tex_coords));   }

inline float4 tex2D_linearize(const sampler2D tex, float3 tex_coords)
{   return decode_input(COMPAT_TEXTURE(tex, tex_coords.xy));   }

inline float4 tex2D_linearize(const sampler2D tex, float2 tex_coords, int texel_off)
{   return decode_input(textureLod(tex, tex_coords, texel_off));    }

inline float4 tex2D_linearize(const sampler2D tex, float3 tex_coords, int texel_off)
{   return decode_input(textureLod(tex, tex_coords.xy, texel_off));    }

//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy));   }

//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy));   }

//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy, texel_off));    }

//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy, texel_off));    }

//  tex2Dbias:
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords)
//{   return decode_input(tex2Dbias(tex, tex_coords));   }

//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dbias(tex, tex_coords, texel_off));    }

//  tex2Dfetch:
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords)
//{   return decode_input(tex2Dfetch(tex, tex_coords));  }

//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dfetch(tex, tex_coords, texel_off));   }

//  tex2Dlod:
inline float4 tex2Dlod_linearize(const sampler2D tex, float4 tex_coords)
{   return decode_input(textureLod(tex, tex_coords.xy, 0.0));    }

inline float4 tex2Dlod_linearize(const sampler2D tex, float4 tex_coords, int texel_off)
{   return decode_input(textureLod(tex, tex_coords.xy, texel_off));     }
/*
//  tex2Dproj:
inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }
*/
/*
//  tex3D:
inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords)
{   return decode_input(tex3D(tex, tex_coords));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, texel_off));    }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy)
{   return decode_input(tex3D(tex, tex_coords, dx, dy));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, dx, dy, texel_off));    }

//  tex3Dbias:
inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dbias(tex, tex_coords));   }

inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dbias(tex, tex_coords, texel_off));    }

//  tex3Dfetch:
inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords)
{   return decode_input(tex3Dfetch(tex, tex_coords));  }

inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex3Dfetch(tex, tex_coords, texel_off));   }

//  tex3Dlod:
inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dlod(tex, tex_coords));    }

inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dlod(tex, tex_coords, texel_off));     }

//  tex3Dproj:
inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dproj(tex, tex_coords));   }

inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dproj(tex, tex_coords, texel_off));    }
/////////*

//  NONSTANDARD "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  This narrow selection of nonstandard tex2D* functions can be useful:

//  tex2Dlod0: Automatically fill in the tex2D LOD parameter for mip level 0.
//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0)));   }

//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords, const int texel_off)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0), texel_off));    }


//  MANUALLY LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a narrower selection of tex2D* wrapper functions that decode an
//  input sample with a specified gamma value.  These are useful for reading
//  LUT's and for reading the input of pass0 in a later pass.

//  tex2D:
inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float3 gamma)
{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords), gamma);   }

inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float3 gamma)
{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords.xy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(COMPAT_TEXTURE(tex, tex_coords, dx, dy, texel_off), gamma);    }
/*
//  tex2Dbias:
inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords), gamma);   }

inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords, texel_off), gamma);    }

//  tex2Dfetch:
inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords), gamma);  }

inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords, texel_off), gamma);   }
*/
//  tex2Dlod:
inline float4 tex2Dlod_linearize_gamma(const sampler2D tex, float4 tex_coords, float3 gamma)
{   return decode_gamma_input(textureLod(tex, tex_coords.xy, 0.0), gamma);    }

inline float4 tex2Dlod_linearize_gamma(const sampler2D tex, float4 tex_coords, int texel_off, float3 gamma)
{   return decode_gamma_input(textureLod(tex, tex_coords.xy, texel_off), gamma);     }


#endif  //  GAMMA_MANAGEMENT_H

////////////////////////////  END GAMMA-MANAGEMENT  //////////////////////////

//#include "tex2Dantialias.h"

/////////////////////////  BEGIN TEX2DANTIALIAS  /////////////////////////

#ifndef TEX2DANTIALIAS_H
#define TEX2DANTIALIAS_H

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////

//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA


/////////////////////////////////  DESCRIPTION  ////////////////////////////////

//  This file provides antialiased and subpixel-aware tex2D lookups.
//  Requires:   All functions share these requirements:
//              1.) All requirements of gamma-management.h must be satisfied!
//              2.) pixel_to_tex_uv must be a 2x2 matrix that transforms pixe-
//                  space offsets to texture uv offsets.  You can get this with:
//                      const float2 duv_dx = ddx(tex_uv);
//                      const float2 duv_dy = ddy(tex_uv);
//                      const float2x2 pixel_to_tex_uv = float2x2(
//                          duv_dx.x, duv_dy.x,
//                          duv_dx.y, duv_dy.y);
//                  This is left to the user in case the current Cg profile
//                  doesn't support ddx()/ddy().  Ideally, the user could find
//                  calculate a distorted tangent-space mapping analytically.
//                  If not, a simple flat mapping can be obtained with:
//                      const float2 xy_to_uv_scale = output_size *
//                          video_size/texture_size;
//                      const float2x2 pixel_to_tex_uv = float2x2(
//                          xy_to_uv_scale.x, 0.0,
//                          0.0, xy_to_uv_scale.y);
//  Optional:   To set basic AA settings, #define ANTIALIAS_OVERRIDE_BASICS and:
//              1.) Set an antialiasing level:
//                      static const float aa_level = {0 (none),
//                          1 (sample subpixels), 4, 5, 6, 7, 8, 12, 16, 20, 24}
//              2.) Set a filter type:
//                      static const float aa_filter = {
//                          0 (Box, Separable), 1 (Box, Cylindrical),
//                          2 (Tent, Separable), 3 (Tent, Cylindrical)
//                          4 (Gaussian, Separable), 5 (Gaussian, Cylindrical)
//                          6 (Cubic, Separable), 7 (Cubic, Cylindrical)
//                          8 (Lanczos Sinc, Separable),
//                          9 (Lanczos Jinc, Cylindrical)}
//                  If the input is unknown, a separable box filter is used.
//                  Note: Lanczos Jinc is terrible for sparse sampling, and
//                  using aa_axis_importance (see below) defeats the purpose.
//              3.) Mirror the sample pattern on odd frames?
//                      static const bool aa_temporal = {true, false]
//                  This helps rotational invariance but can look "fluttery."
//              The user may #define ANTIALIAS_OVERRIDE_PARAMETERS to override
//              (all of) the following default parameters with static or uniform
//              constants (or an accessor function for subpixel offsets):
//              1.) Cubic parameters:
//                      static const float aa_cubic_c = 0.5;
//                  See http://www.imagemagick.org/Usage/filter/#mitchell
//              2.) Gaussian parameters:
//                      static const float aa_gauss_sigma =
//                          0.5/aa_pixel_diameter;
//              3.) Set subpixel offsets.  This requires an accessor function
//                  for compatibility with scalar runtime shader   Return
//                  a float2 pixel offset in [-0.5, 0.5] for the red subpixel:
//                      float2 get_aa_subpixel_r_offset()
//              The user may also #define ANTIALIAS_OVERRIDE_STATIC_CONSTANTS to
//              override (all of) the following default static values.  However,
//              the file's structure requires them to be declared static const:
//              1.) static const float aa_lanczos_lobes = 3.0;
//              2.) static const float aa_gauss_support = 1.0/aa_pixel_diameter;
//                  Note the default tent/Gaussian support radii may appear
//                  arbitrary, but extensive testing found them nearly optimal
//                  for tough cases like strong distortion at low AA levels.
//                  (The Gaussian default is only best for practical gauss_sigma
//                  values; much larger gauss_sigmas ironically prefer slightly
//                  smaller support given sparse sampling, and vice versa.)
//              3.) static const float aa_tent_support = 1.0 / aa_pixel_diameter;
//              4.) static const float2 aa_xy_axis_importance:
//                  The sparse N-queens sampling grid interacts poorly with
//                  negative-lobed 2D filters.  However, if aliasing is much
//                  stronger in one direction (e.g. horizontally with a phosphor
//                  mask), it can be useful to downplay sample offsets along the
//                  other axis.  The support radius in each direction scales with
//                  aa_xy_axis_importance down to a minimum of 0.5 (box support),
//                  after which point only the offsets used for calculating
//                  weights continue to scale downward.  This works as follows:
//                  If aa_xy_axis_importance = float2(1.0, 1.0/support_radius),
//                  the vertical support radius will drop to 1.0, and we'll just
//                  filter vertical offsets with the first filter lobe, while
//                  horizontal offsets go through the full multi-lobe filter.
//                  If aa_xy_axis_importance = float2(1.0, 0.0), the vertical
//                  support radius will drop to box support, and the vertical
//                  offsets will be ignored entirely (essentially giving us a
//                  box filter vertically).  The former is potentially smoother
//                  (but less predictable) and the default behavior of Lanczos
//                  jinc, whereas the latter is sharper and the default behavior
//                  of cubics and Lanczos sinc.
//              5.) static const float aa_pixel_diameter: You can expand the
//                  pixel diameter to e.g. sqrt(2.0), which may be a better
//                  support range for cylindrical filters (they don't
//                  currently discard out-of-circle samples though).
//              Finally, there are two miscellaneous options:
//              1.) If you want to antialias a manually tiled texture, you can
//                  #define ANTIALIAS_DISABLE_ANISOTROPIC to use tex2Dlod() to
//                  fix incompatibilities with anisotropic filtering.  This is
//                  slower, and the Cg profile must support tex2Dlod().
//              2.) If aa_cubic_c is a runtime uniform, you can #define
//                  RUNTIME_ANTIALIAS_WEIGHTS to evaluate cubic weights once per
//                  fragment instead of at the usage site (which is used by
//                  default, because it enables static evaluation).
//  Description:
//  Each antialiased lookup follows these steps:
//  1.) Define a sample pattern of pixel offsets in the range of [-0.5, 0.5]
//      pixels, spanning the diameter of a rectangular box filter.
//  2.) Scale these offsets by the support diameter of the user's chosen filter.
//  3.) Using these pixel offsets from the pixel center, compute the offsets to
//      predefined subpixel locations.
//  4.) Compute filter weights based on subpixel offsets.
//  Much of that can often be done at compile-time.  At runtime:
//  1.) Project pixel-space offsets into uv-space with a matrix multiplication
//      to get the uv offsets for each sample.  Rectangular pixels have a
//      diameter of 1.0.  Circular pixels are not currently supported, but they
//      might be better with a diameter of sqrt(2.0) to ensure there are no gaps
//      between them.
//  2.) Load, weight, and sum samples.
//  We use a sparse bilinear sampling grid, so there are two major implications:
//  1.) We can directly project the pixel-space support box into uv-space even
//      if we're upsizing.  This wouldn't be the case for nearest neighbor,
//      where we'd have to expand the uv-space diameter to at least the support
//      size to ensure sufficient filter support.  In our case, this allows us
//      to treat upsizing the same as downsizing and use static weighting. :)
//  2.) For decent results, negative-lobed filters must be computed based on
//      separable weights, not radial distances, because the sparse sampling
//      makes no guarantees about radial distributions.  Even then, it's much
//      better to set aa_xy_axis_importance to e.g. float2(1.0, 0.0) to use e.g.
//      Lanczos2 horizontally and a box filter vertically.  This is mainly due
//      to the sparse N-queens sampling and a statistically enormous positive or
//      negative covariance between horizontal and vertical weights.
//
//  Design Decision Comments:
//  "aa_temporal" mirrors the sample pattern on odd frames along the axis that
//  keeps subpixel weights constant.  This helps with rotational invariance, but
//  it can cause distracting fluctuations, and horizontal and vertical edges
//  will look the same.  Using a different pattern on a shifted grid would
//  exploit temporal AA better, but it would require a dynamic branch or a lot
//  of conditional moves, so it's prohibitively slow for the minor benefit.


/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////

#ifndef ANTIALIAS_OVERRIDE_BASICS
    //  The following settings must be static constants:
    static const float aa_level = 12.0;
    static const float aa_filter = 0.0;
    static const bool aa_temporal = false;
#endif

#ifndef ANTIALIAS_OVERRIDE_STATIC_CONSTANTS
    //  Users may override these parameters, but the file structure requires
    //  them to be static constants; see the descriptions above.
    static const float aa_pixel_diameter = 1.0;
    static const float aa_lanczos_lobes = 3.0;
    static const float aa_gauss_support = 1.0 / aa_pixel_diameter;
    static const float aa_tent_support = 1.0 / aa_pixel_diameter;
    
    //  If we're using a negative-lobed filter, default to using it horizontally
    //  only, and use only the first lobe vertically or a box filter, over a
    //  correspondingly smaller range.  This compensates for the sparse sampling
    //  grid's typically large positive/negative x/y covariance.
    static const float2 aa_xy_axis_importance =
        aa_filter < 5.5 ? float2(1.0) :         //  Box, tent, Gaussian
        aa_filter < 8.5 ? float2(1.0, 0.0) :    //  Cubic and Lanczos sinc
        aa_filter < 9.5 ? float2(1.0, 1.0/aa_lanczos_lobes) :   //  Lanczos jinc
        float2(1.0);                            //  Default to box
#endif

#ifndef ANTIALIAS_OVERRIDE_PARAMETERS
    //  Users may override these values with their own uniform or static consts.
    //  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
    //  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
    //  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
    //  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
    //  4.) C = 0.0 is a soft spline filter.
    static const float aa_cubic_c = 0.5;
    static const float aa_gauss_sigma = 0.5 / aa_pixel_diameter;
    //  Users may override the subpixel offset accessor function with their own.
    //  A function is used for compatibility with scalar runtime shader 
    inline float2 get_aa_subpixel_r_offset()
    {
        return float2(0.0, 0.0);
    }
#endif


//////////////////////////////////  INCLUDES  //////////////////////////////////

//#include "../../../../include/gamma-management.h"


//////////////////////////////////  CONSTANTS  /////////////////////////////////

static const float aa_box_support = 0.5;
static const float aa_cubic_support = 2.0;


////////////////////////////  GLOBAL NON-CONSTANTS  ////////////////////////////

//  We'll want to define these only once per fragment at most.
#ifdef RUNTIME_ANTIALIAS_WEIGHTS
    float aa_cubic_b;
    float cubic_branch1_x3_coeff;
    float cubic_branch1_x2_coeff;
    float cubic_branch1_x0_coeff;
    float cubic_branch2_x3_coeff;
    float cubic_branch2_x2_coeff;
    float cubic_branch2_x1_coeff;
    float cubic_branch2_x0_coeff;
#endif


///////////////////////////////////  HELPERS  //////////////////////////////////

void assign_aa_cubic_constants()
{
    //  Compute cubic coefficients on demand at runtime, and save them to global
    //  uniforms.  The B parameter is computed from C, because "Keys cubics"
    //  with B = 1 - 2C are considered the highest quality.
    #ifdef RUNTIME_ANTIALIAS_WEIGHTS
        if(aa_filter > 5.5 && aa_filter < 7.5)
        {
            aa_cubic_b = 1.0 - 2.0*aa_cubic_c;
            cubic_branch1_x3_coeff = 12.0 - 9.0*aa_cubic_b - 6.0*aa_cubic_c;
            cubic_branch1_x2_coeff = -18.0 + 12.0*aa_cubic_b + 6.0*aa_cubic_c;
            cubic_branch1_x0_coeff = 6.0 - 2.0 * aa_cubic_b;
            cubic_branch2_x3_coeff = -aa_cubic_b - 6.0 * aa_cubic_c;
            cubic_branch2_x2_coeff = 6.0*aa_cubic_b + 30.0*aa_cubic_c;
            cubic_branch2_x1_coeff = -12.0*aa_cubic_b - 48.0*aa_cubic_c;
            cubic_branch2_x0_coeff = 8.0*aa_cubic_b + 24.0*aa_cubic_c;
        }
    #endif
}

inline float4 get_subpixel_support_diam_and_final_axis_importance()
{
    //  Statically select the base support radius:
    static const float base_support_radius =
        aa_filter < 1.5 ? aa_box_support :
        aa_filter < 3.5 ? aa_tent_support :
        aa_filter < 5.5 ? aa_gauss_support :
        aa_filter < 7.5 ? aa_cubic_support :
        aa_filter < 9.5 ? aa_lanczos_lobes :
        aa_box_support; //  Default to box
    //  Expand the filter support for subpixel filtering.
    const float2 subpixel_support_radius_raw =
        float2(base_support_radius) + abs(get_aa_subpixel_r_offset());
    if(aa_filter < 1.5)
    {
        //  Ignore aa_xy_axis_importance for box filtering.
        const float2 subpixel_support_diam =
            2.0 * subpixel_support_radius_raw;
        const float2 final_axis_importance = float2(1.0);
        return float4(subpixel_support_diam, final_axis_importance);
    }
    else
    {
        //  Scale the support window by aa_xy_axis_importance, but don't narrow
        //  it further than box support.  This allows decent vertical AA without
        //  messing up horizontal weights or using something silly like Lanczos4
        //  horizontally with a huge vertical average over an 8-pixel radius.
        const float2 subpixel_support_radius = max(float2(aa_box_support, aa_box_support),
            subpixel_support_radius_raw * aa_xy_axis_importance);
        //  Adjust aa_xy_axis_importance to compensate for what's already done:
        const float2 final_axis_importance = aa_xy_axis_importance *
            subpixel_support_radius_raw/subpixel_support_radius;
        const float2 subpixel_support_diam = 2.0 * subpixel_support_radius;
        return float4(subpixel_support_diam, final_axis_importance);
    }
}


///////////////////////////  FILTER WEIGHT FUNCTIONS  //////////////////////////

inline float eval_box_filter(const float dist)
{
    return float(abs(dist) <= aa_box_support);
}

inline float eval_separable_box_filter(const float2 offset)
{
    return float(all(bool2((abs(offset.x) <= aa_box_support), (abs(offset.y) <= aa_box_support))));
}

inline float eval_tent_filter(const float dist)
{
    return clamp((aa_tent_support - dist)/
        aa_tent_support, 0.0, 1.0);
}

inline float eval_gaussian_filter(const float dist)
{
    return exp(-(dist*dist) / (2.0*aa_gauss_sigma*aa_gauss_sigma));
}

inline float eval_cubic_filter(const float dist)
{
    //  Compute coefficients like assign_aa_cubic_constants(), but statically.
    #ifndef RUNTIME_ANTIALIAS_WEIGHTS
        //  When runtime weights are used, these values are instead written to
        //  global uniforms at the beginning of each tex2Daa* call.
        const float aa_cubic_b = 1.0 - 2.0*aa_cubic_c;
        const float cubic_branch1_x3_coeff = 12.0 - 9.0*aa_cubic_b - 6.0*aa_cubic_c;
        const float cubic_branch1_x2_coeff = -18.0 + 12.0*aa_cubic_b + 6.0*aa_cubic_c;
        const float cubic_branch1_x0_coeff = 6.0 - 2.0 * aa_cubic_b;
        const float cubic_branch2_x3_coeff = -aa_cubic_b - 6.0 * aa_cubic_c;
        const float cubic_branch2_x2_coeff = 6.0*aa_cubic_b + 30.0*aa_cubic_c;
        const float cubic_branch2_x1_coeff = -12.0*aa_cubic_b - 48.0*aa_cubic_c;
        const float cubic_branch2_x0_coeff = 8.0*aa_cubic_b + 24.0*aa_cubic_c;
    #endif
    const float abs_dist = abs(dist);
    //  Compute the cubic based on the Horner's method formula in:
    //  http://www.cs.utexas.edu/users/fussell/courses/cs384g/lectures/mitchell/Mitchell.pdf
    return (abs_dist < 1.0 ?
        (cubic_branch1_x3_coeff*abs_dist +
            cubic_branch1_x2_coeff)*abs_dist*abs_dist +
            cubic_branch1_x0_coeff :
        abs_dist < 2.0 ?
            ((cubic_branch2_x3_coeff*abs_dist +
                cubic_branch2_x2_coeff)*abs_dist +
                cubic_branch2_x1_coeff)*abs_dist + cubic_branch2_x0_coeff :
            0.0)/6.0;
}

inline float eval_separable_cubic_filter(const float2 offset)
{
    //  This is faster than using a specific float2 version:
    return eval_cubic_filter(offset.x) *
        eval_cubic_filter(offset.y);
}

inline float2 eval_sinc_filter(const float2 offset)
{
    //  It's faster to let the caller handle the zero case, or at least it
    //  was when I used macros and the shader preset took a full minute to load.
    const float2 pi_offset = pi * offset;
    return sin(pi_offset)/pi_offset;
}

inline float eval_separable_lanczos_sinc_filter(const float2 offset_unsafe)
{
    //  Note: For sparse sampling, you really need to pick an axis to use
    //  Lanczos along (e.g. set aa_xy_axis_importance = float2(1.0, 0.0)).
    const float2 offset = FIX_ZERO(offset_unsafe);
    const float2 xy_weights = eval_sinc_filter(offset) *
        eval_sinc_filter(offset/aa_lanczos_lobes);
    return xy_weights.x * xy_weights.y;
}

inline float eval_jinc_filter_unorm(const float x)
{
    //  This is a Jinc approximation for x in [0, 45).  We'll use x in range
    //  [0, 4*pi) or so.  There are faster/closer approximations based on
    //  piecewise cubics from [0, 45) and asymptotic approximations beyond that,
    //  but this has a maximum absolute error < 1/512, and it's simpler/faster
    //  for shaders...not that it's all that useful for sparse sampling anyway.
    const float point3845_x = 0.38448566093564*x;
    const float exp_term = exp(-(point3845_x*point3845_x));
    const float point8154_plus_x = 0.815362332840791 + x;
    const float cos_term = cos(point8154_plus_x);
    return (
        0.0264727330997042*min(x, 6.83134964622778) +
        0.680823557250528*exp_term +
        -0.0597255978950933*min(7.41043194481873, x)*cos_term /
            (point8154_plus_x + 0.0646074538634482*(x*x) +
            cos(x)*max(exp_term, cos(x) + cos_term)) -
        0.180837503591406);
}

inline float eval_jinc_filter(const float dist)
{
    return eval_jinc_filter_unorm(pi * dist);
}

inline float eval_lanczos_jinc_filter(const float dist)
{
    return eval_jinc_filter(dist) * eval_jinc_filter(dist/aa_lanczos_lobes);
}


inline float3 eval_unorm_rgb_weights(const float2 offset,
    const float2 final_axis_importance)
{
    //  Requires:   1.) final_axis_impportance must be computed according to
    //                  get_subpixel_support_diam_and_final_axis_importance().
    //              2.) aa_filter must be a global constant.
    //              3.) offset must be an xy pixel offset in the range:
    //                      ([-subpixel_support_diameter.x/2,
    //                      subpixel_support_diameter.x/2],
    //                      [-subpixel_support_diameter.y/2,
    //                      subpixel_support_diameter.y/2])
    //  Returns:    Sample weights at R/G/B destination subpixels for the
    //              given xy pixel offset.
    const float2 offset_g = offset * final_axis_importance;
    const float2 aa_r_offset = get_aa_subpixel_r_offset();
    const float2 offset_r = offset_g - aa_r_offset * final_axis_importance;
    const float2 offset_b = offset_g + aa_r_offset * final_axis_importance;
    //  Statically select a filter:
    if(aa_filter < 0.5)
    {
        return float3(eval_separable_box_filter(offset_r),
            eval_separable_box_filter(offset_g),
            eval_separable_box_filter(offset_b));
    }
    else if(aa_filter < 1.5)
    {
        return float3(eval_box_filter(length(offset_r)),
            eval_box_filter(length(offset_g)),
            eval_box_filter(length(offset_b)));
    }
    else if(aa_filter < 2.5)
    {
        return float3(
            eval_tent_filter(offset_r.x) * eval_tent_filter(offset_r.y),
            eval_tent_filter(offset_g.x) * eval_tent_filter(offset_g.y),
            eval_tent_filter(offset_b.x) * eval_tent_filter(offset_b.y));
    }
    else if(aa_filter < 3.5)
    {
        return float3(eval_tent_filter(length(offset_r)),
            eval_tent_filter(length(offset_g)),
            eval_tent_filter(length(offset_b)));
    }
    else if(aa_filter < 4.5)
    {
        return float3(
            eval_gaussian_filter(offset_r.x) * eval_gaussian_filter(offset_r.y),
            eval_gaussian_filter(offset_g.x) * eval_gaussian_filter(offset_g.y),
            eval_gaussian_filter(offset_b.x) * eval_gaussian_filter(offset_b.y));
    }
    else if(aa_filter < 5.5)
    {
        return float3(eval_gaussian_filter(length(offset_r)),
            eval_gaussian_filter(length(offset_g)),
            eval_gaussian_filter(length(offset_b)));
    }
    else if(aa_filter < 6.5)
    {
        return float3(
            eval_cubic_filter(offset_r.x) * eval_cubic_filter(offset_r.y),
            eval_cubic_filter(offset_g.x) * eval_cubic_filter(offset_g.y),
            eval_cubic_filter(offset_b.x) * eval_cubic_filter(offset_b.y));
    }
    else if(aa_filter < 7.5)
    {
        return float3(eval_cubic_filter(length(offset_r)),
            eval_cubic_filter(length(offset_g)),
            eval_cubic_filter(length(offset_b)));
    }
    else if(aa_filter < 8.5)
    {
        return float3(eval_separable_lanczos_sinc_filter(offset_r),
            eval_separable_lanczos_sinc_filter(offset_g),
            eval_separable_lanczos_sinc_filter(offset_b));
    }
    else if(aa_filter < 9.5)
    {
        return float3(eval_lanczos_jinc_filter(length(offset_r)),
            eval_lanczos_jinc_filter(length(offset_g)),
            eval_lanczos_jinc_filter(length(offset_b)));
    }
    else
    {
        //  Default to a box, because Lanczos Jinc is so bad. ;)
        return float3(eval_separable_box_filter(offset_r),
            eval_separable_box_filter(offset_g),
            eval_separable_box_filter(offset_b));
    }
}


//////////////////////////////  HELPER FUNCTIONS  //////////////////////////////

inline float4 tex2Daa_tiled_linearize(const sampler2D samp, const float2 s)
{
    //  If we're manually tiling a texture, anisotropic filtering can get
    //  confused.  This is one workaround:
    #ifdef ANTIALIAS_DISABLE_ANISOTROPIC
        //  TODO: Use tex2Dlod_linearize with a calculated mip level.
        return tex2Dlod_linearize(samp, float4(s, 0.0, 0.0));
    #else
        return tex2D_linearize(samp, s);
    #endif
}

inline float2 get_frame_sign(const float frame)
{
    if(aa_temporal)
    {
        //  Mirror the sampling pattern for odd frames in a direction that
        //  lets us keep the same subpixel sample weights:
        const float frame_odd = float(fmod(frame, 2.0) > 0.5);
        const float2 aa_r_offset = get_aa_subpixel_r_offset();
        const float2 mirror = -float2(abs(aa_r_offset.x) < (FIX_ZERO(0.0)), abs(aa_r_offset.y) < (FIX_ZERO(0.0)));
        return mirror;
    }
    else
    {
        return float2(1.0, 1.0);
    }
}


/////////////////////////  ANTIALIASED TEXTURE LOOKUPS  ////////////////////////

float3 tex2Daa_subpixel_weights_only(const sampler2D tex,
    const float2 tex_uv, const float2x2 pixel_to_tex_uv)
{
    //  This function is unlike the others: Just perform a single independent
    //  lookup for each subpixel.  It may be very aliased.
    const float2 aa_r_offset = get_aa_subpixel_r_offset();
    const float2 aa_r_offset_uv_offset = mul(pixel_to_tex_uv, aa_r_offset);
    const float color_g = tex2D_linearize(tex, tex_uv).g;
    const float color_r = tex2D_linearize(tex, tex_uv + aa_r_offset_uv_offset).r;
    const float color_b = tex2D_linearize(tex, tex_uv - aa_r_offset_uv_offset).b;
    return float3(color_r, color_g, color_b);
}

//  The tex2Daa* functions compile very slowly due to all the macros and
//  compile-time math, so only include the ones we'll actually use!
float3 tex2Daa4x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use an RGMS4 pattern (4-queens):
    //  . . Q .  : off =(-1.5, -1.5)/4 + (2.0, 0.0)/4
    //  Q . . .  : off =(-1.5, -1.5)/4 + (0.0, 1.0)/4
    //  . . . Q  : off =(-1.5, -1.5)/4 + (3.0, 2.0)/4
    //  . Q . .  : off =(-1.5, -1.5)/4 + (1.0, 3.0)/4
    //  Static screenspace sample offsets (compute some implicitly):
    static const float grid_size = 4.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0,1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5,0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(2.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(0.0, 1.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = w1.bgr;
    const float3 w3 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0,1.0,1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (w0 * sample0 + w1 * sample1 +
        w2 * sample2 + w3 * sample3);
}

float3 tex2Daa5x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 5-queens pattern:
    //  . Q . . .  : off =(-2.0, -2.0)/5 + (1.0, 0.0)/5
    //  . . . . Q  : off =(-2.0, -2.0)/5 + (4.0, 1.0)/5
    //  . . Q . .  : off =(-2.0, -2.0)/5 + (2.0, 2.0)/5
    //  Q . . . .  : off =(-2.0, -2.0)/5 + (0.0, 3.0)/5
    //  . . . Q .  : off =(-2.0, -2.0)/5 + (3.0, 4.0)/5
    //  Static screenspace sample offsets (compute some implicitly):
    static const float grid_size = 5.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(1.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(4.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(2.0, 2.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = w1.bgr;
    const float3 w4 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 w_sum_inv = float3(1.0)/(w0 + w1 + w2 + w3 + w4);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (w0 * sample0 + w1 * sample1 +
        w2 * sample2 + w3 * sample3 + w4 * sample4);
}

float3 tex2Daa6x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 6-queens pattern with a stronger horizontal
    //  than vertical slant:
    //  . . . . Q .  : off =(-2.5, -2.5)/6 + (4.0, 0.0)/6
    //  . . Q . . .  : off =(-2.5, -2.5)/6 + (2.0, 1.0)/6
    //  Q . . . . .  : off =(-2.5, -2.5)/6 + (0.0, 2.0)/6
    //  . . . . . Q  : off =(-2.5, -2.5)/6 + (5.0, 3.0)/6
    //  . . . Q . .  : off =(-2.5, -2.5)/6 + (3.0, 4.0)/6
    //  . Q . . . .  : off =(-2.5, -2.5)/6 + (1.0, 5.0)/6
    //  Static screenspace sample offsets (compute some implicitly):
    static const float grid_size = 6.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(4.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(2.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(0.0, 2.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = w2.bgr;
    const float3 w4 = w1.bgr;
    const float3 w5 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (w0 * sample0 + w1 * sample1 + w2 * sample2 +
        w3 * sample3 + w4 * sample4 + w5 * sample5);
}

float3 tex2Daa7x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 7-queens pattern with a queen in the center:
    //  . Q . . . . .  : off =(-3.0, -3.0)/7 + (1.0, 0.0)/7
    //  . . . . Q . .  : off =(-3.0, -3.0)/7 + (4.0, 1.0)/7
    //  Q . . . . . .  : off =(-3.0, -3.0)/7 + (0.0, 2.0)/7
    //  . . . Q . . .  : off =(-3.0, -3.0)/7 + (3.0, 3.0)/7
    //  . . . . . . Q  : off =(-3.0, -3.0)/7 + (6.0, 4.0)/7
    //  . . Q . . . .  : off =(-3.0, -3.0)/7 + (2.0, 5.0)/7
    //  . . . . . Q .  : off =(-3.0, -3.0)/7 + (5.0, 6.0)/7
    static const float grid_size = 7.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(1.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(4.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(0.0, 2.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(3.0, 3.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = w2.bgr;
    const float3 w5 = w1.bgr;
    const float3 w6 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2;
    const float3 w_sum = half_sum + half_sum.bgr + w3;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6);
}

float3 tex2Daa8x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 8-queens pattern.
    //  . . Q . . . . .  : off =(-3.5, -3.5)/8 + (2.0, 0.0)/8
    //  . . . . Q . . .  : off =(-3.5, -3.5)/8 + (4.0, 1.0)/8
    //  . Q . . . . . .  : off =(-3.5, -3.5)/8 + (1.0, 2.0)/8
    //  . . . . . . . Q  : off =(-3.5, -3.5)/8 + (7.0, 3.0)/8
    //  Q . . . . . . .  : off =(-3.5, -3.5)/8 + (0.0, 4.0)/8
    //  . . . . . . Q .  : off =(-3.5, -3.5)/8 + (6.0, 5.0)/8
    //  . . . Q . . . .  : off =(-3.5, -3.5)/8 + (3.0, 6.0)/8
    //  . . . . . Q . .  : off =(-3.5, -3.5)/8 + (5.0, 7.0)/8
    static const float grid_size = 8.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(2.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(4.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(1.0, 2.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(7.0, 3.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = w3.bgr;
    const float3 w5 = w2.bgr;
    const float3 w6 = w1.bgr;
    const float3 w7 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2 + w3;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, and mirror on odd frames if directed:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    const float2 uv_offset3 = mul(true_pixel_to_tex_uv, xy_offset3 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset3).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset3).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample7 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6 + w7 * sample7);
}

float3 tex2Daa12x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 12-superqueens pattern where no 3 points are
    //  exactly collinear.
    //  . . . Q . . . . . . . .  : off =(-5.5, -5.5)/12 + (3.0, 0.0)/12
    //  . . . . . . . . . Q . .  : off =(-5.5, -5.5)/12 + (9.0, 1.0)/12
    //  . . . . . . Q . . . . .  : off =(-5.5, -5.5)/12 + (6.0, 2.0)/12
    //  . Q . . . . . . . . . .  : off =(-5.5, -5.5)/12 + (1.0, 3.0)/12
    //  . . . . . . . . . . . Q  : off =(-5.5, -5.5)/12 + (11.0, 4.0)/12
    //  . . . . Q . . . . . . .  : off =(-5.5, -5.5)/12 + (4.0, 5.0)/12
    //  . . . . . . . Q . . . .  : off =(-5.5, -5.5)/12 + (7.0, 6.0)/12
    //  Q . . . . . . . . . . .  : off =(-5.5, -5.5)/12 + (0.0, 7.0)/12
    //  . . . . . . . . . . Q .  : off =(-5.5, -5.5)/12 + (10.0, 8.0)/12
    //  . . . . . Q . . . . . .  : off =(-5.5, -5.5)/12 + (5.0, 9.0)/12
    //  . . Q . . . . . . . . .  : off =(-5.5, -5.5)/12 + (2.0, 10.0)/12
    //  . . . . . . . . Q . . .  : off =(-5.5, -5.5)/12 + (8.0, 11.0)/12
    static const float grid_size = 12.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(3.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(9.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(6.0, 2.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(1.0, 3.0) * xy_step;
    const float2 xy_offset4 = xy_start_offset + float2(11.0, 4.0) * xy_step;
    const float2 xy_offset5 = xy_start_offset + float2(4.0, 5.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = eval_unorm_rgb_weights(xy_offset4, final_axis_importance);
    const float3 w5 = eval_unorm_rgb_weights(xy_offset5, final_axis_importance);
    const float3 w6 = w5.bgr;
    const float3 w7 = w4.bgr;
    const float3 w8 = w3.bgr;
    const float3 w9 = w2.bgr;
    const float3 w10 = w1.bgr;
    const float3 w11 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2 + w3 + w4 + w5;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/w_sum;
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    const float2 uv_offset3 = mul(true_pixel_to_tex_uv, xy_offset3 * frame_sign);
    const float2 uv_offset4 = mul(true_pixel_to_tex_uv, xy_offset4 * frame_sign);
    const float2 uv_offset5 = mul(true_pixel_to_tex_uv, xy_offset5 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset3).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset4).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset5).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset5).rgb;
    const float3 sample7 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset4).rgb;
    const float3 sample8 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset3).rgb;
    const float3 sample9 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample10 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample11 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6 + w7 * sample7 +
        w8 * sample8 + w9 * sample9 + w10 * sample10 + w11 * sample11);
}

float3 tex2Daa16x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 16-superqueens pattern where no 3 points are
    //  exactly collinear.
    //  . . Q . . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (2.0, 0.0)/16
    //  . . . . . . . . . Q . . . . . .  : off =(-7.5, -7.5)/16 + (9.0, 1.0)/16
    //  . . . . . . . . . . . . Q . . .  : off =(-7.5, -7.5)/16 + (12.0, 2.0)/16
    //  . . . . Q . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (4.0, 3.0)/16
    //  . . . . . . . . Q . . . . . . .  : off =(-7.5, -7.5)/16 + (8.0, 4.0)/16
    //  . . . . . . . . . . . . . . Q .  : off =(-7.5, -7.5)/16 + (14.0, 5.0)/16
    //  Q . . . . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (0.0, 6.0)/16
    //  . . . . . . . . . . Q . . . . .  : off =(-7.5, -7.5)/16 + (10.0, 7.0)/16
    //  . . . . . Q . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (5.0, 8.0)/16
    //  . . . . . . . . . . . . . . . Q  : off =(-7.5, -7.5)/16 + (15.0, 9.0)/16
    //  . Q . . . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (1.0, 10.0)/16
    //  . . . . . . . Q . . . . . . . .  : off =(-7.5, -7.5)/16 + (7.0, 11.0)/16
    //  . . . . . . . . . . . Q . . . .  : off =(-7.5, -7.5)/16 + (11.0, 12.0)/16
    //  . . . Q . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (3.0, 13.0)/16
    //  . . . . . . Q . . . . . . . . .  : off =(-7.5, -7.5)/16 + (6.0, 14.0)/16
    //  . . . . . . . . . . . . . Q . .  : off =(-7.5, -7.5)/16 + (13.0, 15.0)/16
    static const float grid_size = 16.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(2.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(9.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(12.0, 2.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(4.0, 3.0) * xy_step;
    const float2 xy_offset4 = xy_start_offset + float2(8.0, 4.0) * xy_step;
    const float2 xy_offset5 = xy_start_offset + float2(14.0, 5.0) * xy_step;
    const float2 xy_offset6 = xy_start_offset + float2(0.0, 6.0) * xy_step;
    const float2 xy_offset7 = xy_start_offset + float2(10.0, 7.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = eval_unorm_rgb_weights(xy_offset4, final_axis_importance);
    const float3 w5 = eval_unorm_rgb_weights(xy_offset5, final_axis_importance);
    const float3 w6 = eval_unorm_rgb_weights(xy_offset6, final_axis_importance);
    const float3 w7 = eval_unorm_rgb_weights(xy_offset7, final_axis_importance);
    const float3 w8 = w7.bgr;
    const float3 w9 = w6.bgr;
    const float3 w10 = w5.bgr;
    const float3 w11 = w4.bgr;
    const float3 w12 = w3.bgr;
    const float3 w13 = w2.bgr;
    const float3 w14 = w1.bgr;
    const float3 w15 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2 + w3 + w4 + w5 + w6 + w7;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    const float2 uv_offset3 = mul(true_pixel_to_tex_uv, xy_offset3 * frame_sign);
    const float2 uv_offset4 = mul(true_pixel_to_tex_uv, xy_offset4 * frame_sign);
    const float2 uv_offset5 = mul(true_pixel_to_tex_uv, xy_offset5 * frame_sign);
    const float2 uv_offset6 = mul(true_pixel_to_tex_uv, xy_offset6 * frame_sign);
    const float2 uv_offset7 = mul(true_pixel_to_tex_uv, xy_offset7 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset3).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset4).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset5).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset6).rgb;
    const float3 sample7 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset7).rgb;
    const float3 sample8 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset7).rgb;
    const float3 sample9 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset6).rgb;
    const float3 sample10 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset5).rgb;
    const float3 sample11 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset4).rgb;
    const float3 sample12 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset3).rgb;
    const float3 sample13 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample14 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample15 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6 + w7 * sample7 +
        w8 * sample8 + w9 * sample9 + w10 * sample10 + w11 * sample11 +
        w12 * sample12 + w13 * sample13 + w14 * sample14 + w15 * sample15);
}

float3 tex2Daa20x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 20-superqueens pattern where no 3 points are
    //  exactly collinear and superqueens have a squared attack radius of 13.
    //  . . . . . . . Q . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (7.0, 0.0)/20
    //  . . . . . . . . . . . . . . . . Q . . .  : off =(-9.5, -9.5)/20 + (16.0, 1.0)/20
    //  . . . . . . . . . . . Q . . . . . . . .  : off =(-9.5, -9.5)/20 + (11.0, 2.0)/20
    //  . Q . . . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (1.0, 3.0)/20
    //  . . . . . Q . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (5.0, 4.0)/20
    //  . . . . . . . . . . . . . . . Q . . . .  : off =(-9.5, -9.5)/20 + (15.0, 5.0)/20
    //  . . . . . . . . . . Q . . . . . . . . .  : off =(-9.5, -9.5)/20 + (10.0, 6.0)/20
    //  . . . . . . . . . . . . . . . . . . . Q  : off =(-9.5, -9.5)/20 + (19.0, 7.0)/20
    //  . . Q . . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (2.0, 8.0)/20
    //  . . . . . . Q . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (6.0, 9.0)/20
    //  . . . . . . . . . . . . . Q . . . . . .  : off =(-9.5, -9.5)/20 + (13.0, 10.0)/20
    //  . . . . . . . . . . . . . . . . . Q . .  : off =(-9.5, -9.5)/20 + (17.0, 11.0)/20
    //  Q . . . . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (0.0, 12.0)/20
    //  . . . . . . . . . Q . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (9.0, 13.0)/20
    //  . . . . Q . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (4.0, 14.0)/20
    //  . . . . . . . . . . . . . . Q . . . . .  : off =(-9.5, -9.5)/20 + (14.0, 15.0)/20
    //  . . . . . . . . . . . . . . . . . . Q .  : off =(-9.5, -9.5)/20 + (18.0, 16.0)/20
    //  . . . . . . . . Q . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (8.0, 17.0)/20
    //  . . . Q . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (3.0, 18.0)/20
    //  . . . . . . . . . . . . Q . . . . . . .  : off =(-9.5, -9.5)/20 + (12.0, 19.0)/20
    static const float grid_size = 20.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(7.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(16.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(11.0, 2.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(1.0, 3.0) * xy_step;
    const float2 xy_offset4 = xy_start_offset + float2(5.0, 4.0) * xy_step;
    const float2 xy_offset5 = xy_start_offset + float2(15.0, 5.0) * xy_step;
    const float2 xy_offset6 = xy_start_offset + float2(10.0, 6.0) * xy_step;
    const float2 xy_offset7 = xy_start_offset + float2(19.0, 7.0) * xy_step;
    const float2 xy_offset8 = xy_start_offset + float2(2.0, 8.0) * xy_step;
    const float2 xy_offset9 = xy_start_offset + float2(6.0, 9.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = eval_unorm_rgb_weights(xy_offset4, final_axis_importance);
    const float3 w5 = eval_unorm_rgb_weights(xy_offset5, final_axis_importance);
    const float3 w6 = eval_unorm_rgb_weights(xy_offset6, final_axis_importance);
    const float3 w7 = eval_unorm_rgb_weights(xy_offset7, final_axis_importance);
    const float3 w8 = eval_unorm_rgb_weights(xy_offset8, final_axis_importance);
    const float3 w9 = eval_unorm_rgb_weights(xy_offset9, final_axis_importance);
    const float3 w10 = w9.bgr;
    const float3 w11 = w8.bgr;
    const float3 w12 = w7.bgr;
    const float3 w13 = w6.bgr;
    const float3 w14 = w5.bgr;
    const float3 w15 = w4.bgr;
    const float3 w16 = w3.bgr;
    const float3 w17 = w2.bgr;
    const float3 w18 = w1.bgr;
    const float3 w19 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    const float2 uv_offset3 = mul(true_pixel_to_tex_uv, xy_offset3 * frame_sign);
    const float2 uv_offset4 = mul(true_pixel_to_tex_uv, xy_offset4 * frame_sign);
    const float2 uv_offset5 = mul(true_pixel_to_tex_uv, xy_offset5 * frame_sign);
    const float2 uv_offset6 = mul(true_pixel_to_tex_uv, xy_offset6 * frame_sign);
    const float2 uv_offset7 = mul(true_pixel_to_tex_uv, xy_offset7 * frame_sign);
    const float2 uv_offset8 = mul(true_pixel_to_tex_uv, xy_offset8 * frame_sign);
    const float2 uv_offset9 = mul(true_pixel_to_tex_uv, xy_offset9 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset3).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset4).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset5).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset6).rgb;
    const float3 sample7 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset7).rgb;
    const float3 sample8 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset8).rgb;
    const float3 sample9 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset9).rgb;
    const float3 sample10 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset9).rgb;
    const float3 sample11 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset8).rgb;
    const float3 sample12 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset7).rgb;
    const float3 sample13 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset6).rgb;
    const float3 sample14 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset5).rgb;
    const float3 sample15 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset4).rgb;
    const float3 sample16 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset3).rgb;
    const float3 sample17 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample18 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample19 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6 + w7 * sample7 +
        w8 * sample8 + w9 * sample9 + w10 * sample10 + w11 * sample11 +
        w12 * sample12 + w13 * sample13 + w14 * sample14 + w15 * sample15 +
        w16 * sample16 + w17 * sample17 + w18 * sample18 + w19 * sample19);
}

float3 tex2Daa24x(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Use a diagonally symmetric 24-superqueens pattern where no 3 points are
    //  exactly collinear and superqueens have a squared attack radius of 13.
    //  . . . . . . Q . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (6.0, 0.0)/24
    //  . . . . . . . . . . . . . . . . Q . . . . . . .  : off =(-11.5, -11.5)/24 + (16.0, 1.0)/24
    //  . . . . . . . . . . Q . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (10.0, 2.0)/24
    //  . . . . . . . . . . . . . . . . . . . . . Q . .  : off =(-11.5, -11.5)/24 + (21.0, 3.0)/24
    //  . . . . . Q . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (5.0, 4.0)/24
    //  . . . . . . . . . . . . . . . Q . . . . . . . .  : off =(-11.5, -11.5)/24 + (15.0, 5.0)/24
    //  . Q . . . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (1.0, 6.0)/24
    //  . . . . . . . . . . . Q . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (11.0, 7.0)/24
    //  . . . . . . . . . . . . . . . . . . . Q . . . .  : off =(-11.5, -11.5)/24 + (19.0, 8.0)/24
    //  . . . . . . . . . . . . . . . . . . . . . . . Q  : off =(-11.5, -11.5)/24 + (23.0, 9.0)/24
    //  . . . Q . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (3.0, 10.0)/24
    //  . . . . . . . . . . . . . . Q . . . . . . . . .  : off =(-11.5, -11.5)/24 + (14.0, 11.0)/24
    //  . . . . . . . . . Q . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (9.0, 12.0)/24
    //  . . . . . . . . . . . . . . . . . . . . Q . . .  : off =(-11.5, -11.5)/24 + (20.0, 13.0)/24
    //  Q . . . . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (0.0, 14.0)/24
    //  . . . . Q . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (4.0, 15.0)/24
    //  . . . . . . . . . . . . Q . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (12.0, 16.0)/24
    //  . . . . . . . . . . . . . . . . . . . . . . Q .  : off =(-11.5, -11.5)/24 + (22.0, 17.0)/24
    //  . . . . . . . . Q . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (8.0, 18.0)/24
    //  . . . . . . . . . . . . . . . . . . Q . . . . .  : off =(-11.5, -11.5)/24 + (18.0, 19.0)/24
    //  . . Q . . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (2.0, 20.0)/24
    //  . . . . . . . . . . . . . Q . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (13.0, 21.0)/24
    //  . . . . . . . Q . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (7.0, 22.0)/24
    //  . . . . . . . . . . . . . . . . . Q . . . . . .  : off =(-11.5, -11.5)/24 + (17.0, 23.0)/24
    static const float grid_size = 24.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample.  Exploit diagonal symmetry:
    const float2 xy_offset0 = xy_start_offset + float2(6.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(16.0, 1.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(10.0, 2.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(21.0, 3.0) * xy_step;
    const float2 xy_offset4 = xy_start_offset + float2(5.0, 4.0) * xy_step;
    const float2 xy_offset5 = xy_start_offset + float2(15.0, 5.0) * xy_step;
    const float2 xy_offset6 = xy_start_offset + float2(1.0, 6.0) * xy_step;
    const float2 xy_offset7 = xy_start_offset + float2(11.0, 7.0) * xy_step;
    const float2 xy_offset8 = xy_start_offset + float2(19.0, 8.0) * xy_step;
    const float2 xy_offset9 = xy_start_offset + float2(23.0, 9.0) * xy_step;
    const float2 xy_offset10 = xy_start_offset + float2(3.0, 10.0) * xy_step;
    const float2 xy_offset11 = xy_start_offset + float2(14.0, 11.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = eval_unorm_rgb_weights(xy_offset4, final_axis_importance);
    const float3 w5 = eval_unorm_rgb_weights(xy_offset5, final_axis_importance);
    const float3 w6 = eval_unorm_rgb_weights(xy_offset6, final_axis_importance);
    const float3 w7 = eval_unorm_rgb_weights(xy_offset7, final_axis_importance);
    const float3 w8 = eval_unorm_rgb_weights(xy_offset8, final_axis_importance);
    const float3 w9 = eval_unorm_rgb_weights(xy_offset9, final_axis_importance);
    const float3 w10 = eval_unorm_rgb_weights(xy_offset10, final_axis_importance);
    const float3 w11 = eval_unorm_rgb_weights(xy_offset11, final_axis_importance);
    const float3 w12 = w11.bgr;
    const float3 w13 = w10.bgr;
    const float3 w14 = w9.bgr;
    const float3 w15 = w8.bgr;
    const float3 w16 = w7.bgr;
    const float3 w17 = w6.bgr;
    const float3 w18 = w5.bgr;
    const float3 w19 = w4.bgr;
    const float3 w20 = w3.bgr;
    const float3 w21 = w2.bgr;
    const float3 w22 = w1.bgr;
    const float3 w23 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2 + w3 + w4 +
        w5 + w6 + w7 + w8 + w9 + w10 + w11;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, mirror on odd frames if directed, and exploit
    //  diagonal symmetry:
    const float2 frame_sign = get_frame_sign(frame);
    const float2 uv_offset0 = mul(true_pixel_to_tex_uv, xy_offset0 * frame_sign);
    const float2 uv_offset1 = mul(true_pixel_to_tex_uv, xy_offset1 * frame_sign);
    const float2 uv_offset2 = mul(true_pixel_to_tex_uv, xy_offset2 * frame_sign);
    const float2 uv_offset3 = mul(true_pixel_to_tex_uv, xy_offset3 * frame_sign);
    const float2 uv_offset4 = mul(true_pixel_to_tex_uv, xy_offset4 * frame_sign);
    const float2 uv_offset5 = mul(true_pixel_to_tex_uv, xy_offset5 * frame_sign);
    const float2 uv_offset6 = mul(true_pixel_to_tex_uv, xy_offset6 * frame_sign);
    const float2 uv_offset7 = mul(true_pixel_to_tex_uv, xy_offset7 * frame_sign);
    const float2 uv_offset8 = mul(true_pixel_to_tex_uv, xy_offset8 * frame_sign);
    const float2 uv_offset9 = mul(true_pixel_to_tex_uv, xy_offset9 * frame_sign);
    const float2 uv_offset10 = mul(true_pixel_to_tex_uv, xy_offset10 * frame_sign);
    const float2 uv_offset11 = mul(true_pixel_to_tex_uv, xy_offset11 * frame_sign);
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset0).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset1).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset2).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset3).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset4).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset5).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset6).rgb;
    const float3 sample7 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset7).rgb;
    const float3 sample8 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset8).rgb;
    const float3 sample9 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset9).rgb;
    const float3 sample10 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset10).rgb;
    const float3 sample11 = tex2Daa_tiled_linearize(tex, tex_uv + uv_offset11).rgb;
    const float3 sample12 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset11).rgb;
    const float3 sample13 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset10).rgb;
    const float3 sample14 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset9).rgb;
    const float3 sample15 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset8).rgb;
    const float3 sample16 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset7).rgb;
    const float3 sample17 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset6).rgb;
    const float3 sample18 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset5).rgb;
    const float3 sample19 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset4).rgb;
    const float3 sample20 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset3).rgb;
    const float3 sample21 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset2).rgb;
    const float3 sample22 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset1).rgb;
    const float3 sample23 = tex2Daa_tiled_linearize(tex, tex_uv - uv_offset0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6 + w7 * sample7 +
        w8 * sample8 + w9 * sample9 + w10 * sample10 + w11 * sample11 +
        w12 * sample12 + w13 * sample13 + w14 * sample14 + w15 * sample15 +
        w16 * sample16 + w17 * sample17 + w18 * sample18 + w19 * sample19 +
        w20 * sample20 + w21 * sample21 + w22 * sample22 + w23 * sample23);
}

float3 tex2Daa_debug_16x_regular(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  Sample on a regular 4x4 grid.  This is mainly for testing.
    static const float grid_size = 4.0;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float2 xy_step = float2(1.0)/grid_size * subpixel_support_diameter;
    const float2 xy_start_offset = float2(0.5 - grid_size*0.5) * xy_step;
    //  Get the xy offset of each sample:
    const float2 xy_offset0 = xy_start_offset + float2(0.0, 0.0) * xy_step;
    const float2 xy_offset1 = xy_start_offset + float2(1.0, 0.0) * xy_step;
    const float2 xy_offset2 = xy_start_offset + float2(2.0, 0.0) * xy_step;
    const float2 xy_offset3 = xy_start_offset + float2(3.0, 0.0) * xy_step;
    const float2 xy_offset4 = xy_start_offset + float2(0.0, 1.0) * xy_step;
    const float2 xy_offset5 = xy_start_offset + float2(1.0, 1.0) * xy_step;
    const float2 xy_offset6 = xy_start_offset + float2(2.0, 1.0) * xy_step;
    const float2 xy_offset7 = xy_start_offset + float2(3.0, 1.0) * xy_step;
    //  Compute subpixel weights, and exploit diagonal symmetry for speed.
    //  (We can't exploit vertical or horizontal symmetry due to uncertain
    //  subpixel offsets.  We could fix that by rotating xy offsets with the
    //  subpixel structure, but...no.)
    const float3 w0 = eval_unorm_rgb_weights(xy_offset0, final_axis_importance);
    const float3 w1 = eval_unorm_rgb_weights(xy_offset1, final_axis_importance);
    const float3 w2 = eval_unorm_rgb_weights(xy_offset2, final_axis_importance);
    const float3 w3 = eval_unorm_rgb_weights(xy_offset3, final_axis_importance);
    const float3 w4 = eval_unorm_rgb_weights(xy_offset4, final_axis_importance);
    const float3 w5 = eval_unorm_rgb_weights(xy_offset5, final_axis_importance);
    const float3 w6 = eval_unorm_rgb_weights(xy_offset6, final_axis_importance);
    const float3 w7 = eval_unorm_rgb_weights(xy_offset7, final_axis_importance);
    const float3 w8 = w7.bgr;
    const float3 w9 = w6.bgr;
    const float3 w10 = w5.bgr;
    const float3 w11 = w4.bgr;
    const float3 w12 = w3.bgr;
    const float3 w13 = w2.bgr;
    const float3 w14 = w1.bgr;
    const float3 w15 = w0.bgr;
    //  Get the weight sum to normalize the total to 1.0 later:
    const float3 half_sum = w0 + w1 + w2 + w3 + w4 + w5 + w6 + w7;
    const float3 w_sum = half_sum + half_sum.bgr;
    const float3 w_sum_inv = float3(1.0)/(w_sum);
    //  Scale the pixel-space to texture offset matrix by the pixel diameter.
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    //  Get uv sample offsets, taking advantage of row alignment:
    const float2 uv_step_x = mul(true_pixel_to_tex_uv, float2(xy_step.x, 0.0));
    const float2 uv_step_y = mul(true_pixel_to_tex_uv, float2(0.0, xy_step.y));
    const float2 uv_offset0 = -1.5 * (uv_step_x + uv_step_y);
    const float2 sample0_uv = tex_uv + uv_offset0;
    const float2 sample4_uv = sample0_uv + uv_step_y;
    const float2 sample8_uv = sample0_uv + uv_step_y * 2.0;
    const float2 sample12_uv = sample0_uv + uv_step_y * 3.0;
    //  Load samples, linearizing if necessary, etc.:
    const float3 sample0 = tex2Daa_tiled_linearize(tex, sample0_uv).rgb;
    const float3 sample1 = tex2Daa_tiled_linearize(tex, sample0_uv + uv_step_x).rgb;
    const float3 sample2 = tex2Daa_tiled_linearize(tex, sample0_uv + uv_step_x * 2.0).rgb;
    const float3 sample3 = tex2Daa_tiled_linearize(tex, sample0_uv + uv_step_x * 3.0).rgb;
    const float3 sample4 = tex2Daa_tiled_linearize(tex, sample4_uv).rgb;
    const float3 sample5 = tex2Daa_tiled_linearize(tex, sample4_uv + uv_step_x).rgb;
    const float3 sample6 = tex2Daa_tiled_linearize(tex, sample4_uv + uv_step_x * 2.0).rgb;
    const float3 sample7 = tex2Daa_tiled_linearize(tex, sample4_uv + uv_step_x * 3.0).rgb;
    const float3 sample8 = tex2Daa_tiled_linearize(tex, sample8_uv).rgb;
    const float3 sample9 = tex2Daa_tiled_linearize(tex, sample8_uv + uv_step_x).rgb;
    const float3 sample10 = tex2Daa_tiled_linearize(tex, sample8_uv + uv_step_x * 2.0).rgb;
    const float3 sample11 = tex2Daa_tiled_linearize(tex, sample8_uv + uv_step_x * 3.0).rgb;
    const float3 sample12 = tex2Daa_tiled_linearize(tex, sample12_uv).rgb;
    const float3 sample13 = tex2Daa_tiled_linearize(tex, sample12_uv + uv_step_x).rgb;
    const float3 sample14 = tex2Daa_tiled_linearize(tex, sample12_uv + uv_step_x * 2.0).rgb;
    const float3 sample15 = tex2Daa_tiled_linearize(tex, sample12_uv + uv_step_x * 3.0).rgb;
    //  Sum weighted samples (weight sum must equal 1.0 for each channel):
    return w_sum_inv * (
        w0 * sample0 + w1 * sample1 + w2 * sample2 + w3 * sample3 +
        w4 * sample4 + w5 * sample5 + w6 * sample6 + w7 * sample7 +
        w8 * sample8 + w9 * sample9 + w10 * sample10 + w11 * sample11 +
        w12 * sample12 + w13 * sample13 + w14 * sample14 + w15 * sample15);
}

float3 tex2Daa_debug_dynamic(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
    //  This function is for testing only: Use an NxN grid with dynamic weights.
    static const int grid_size = 8;
    assign_aa_cubic_constants();
    const float4 ssd_fai = get_subpixel_support_diam_and_final_axis_importance();
    const float2 subpixel_support_diameter = ssd_fai.xy;
    const float2 final_axis_importance = ssd_fai.zw;
    const float grid_radius_in_samples = (float(grid_size) - 1.0)/2.0;
    const float2 filter_space_offset_step =
        subpixel_support_diameter/float2(grid_size);
    const float2 sample0_filter_space_offset =
        -grid_radius_in_samples * filter_space_offset_step;
    //  Compute xy sample offsets and subpixel weights:
    float3 weights[64]; //originally grid_size * grid_size
    float3 weight_sum = float3(0.0, 0.0, 0.0);
    for(int i = 0; i < grid_size; ++i)
    {
        for(int j = 0; j < grid_size; ++j)
        {
            //  Weights based on xy distances:
            const float2 offset = sample0_filter_space_offset +
                float2(j, i) * filter_space_offset_step;
            const float3 weight = eval_unorm_rgb_weights(offset, final_axis_importance);
            weights[i*grid_size + j] = weight;
            weight_sum += weight;
        }
    }
    //  Get uv offset vectors along x and y directions:
    const float2x2 true_pixel_to_tex_uv =
        float2x2(pixel_to_tex_uv * aa_pixel_diameter);
    const float2 uv_offset_step_x = mul(true_pixel_to_tex_uv,
        float2(filter_space_offset_step.x, 0.0));
    const float2 uv_offset_step_y = mul(true_pixel_to_tex_uv,
        float2(0.0, filter_space_offset_step.y));
    //  Get a starting sample location:
    const float2 sample0_uv_offset = -grid_radius_in_samples *
        (uv_offset_step_x + uv_offset_step_y);
    const float2 sample0_uv = tex_uv + sample0_uv_offset;
    //  Load, weight, and sum [linearized] samples:
    float3 sum = float3(0.0, 0.0, 0.0);
    const float3 weight_sum_inv = float3(1.0)/weight_sum;
    for(int i = 0; i < grid_size; ++i)
    {
        const float2 row_i_first_sample_uv =
            sample0_uv + i * uv_offset_step_y;
        for(int j = 0; j < grid_size; ++j)
        {
            const float2 sample_uv =
                row_i_first_sample_uv + j * uv_offset_step_x;
            sum += weights[i*grid_size + j] *
                tex2Daa_tiled_linearize(tex, sample_uv).rgb;
        }
    }
    return sum * weight_sum_inv;
}


///////////////////////  ANTIALIASING CODEPATH SELECTION  //////////////////////

inline float3 tex2Daa(const sampler2D tex, const float2 tex_uv,
    const float2x2 pixel_to_tex_uv, const float frame)
{
#ifdef DEBUG
	return tex2Daa_subpixel_weights_only(
            tex, tex_uv, pixel_to_tex_uv);
#else
	//  Statically switch between antialiasing modes/levels:
    return (aa_level < 0.5) ? tex2D_linearize(tex, tex_uv).rgb :
        (aa_level < 3.5) ? tex2Daa_subpixel_weights_only(
            tex, tex_uv, pixel_to_tex_uv) :
        (aa_level < 4.5) ? tex2Daa4x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 5.5) ? tex2Daa5x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 6.5) ? tex2Daa6x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 7.5) ? tex2Daa7x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 11.5) ? tex2Daa8x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 15.5) ? tex2Daa12x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 19.5) ? tex2Daa16x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 23.5) ? tex2Daa20x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 253.5) ? tex2Daa24x(tex, tex_uv, pixel_to_tex_uv, frame) :
        (aa_level < 254.5) ? tex2Daa_debug_16x_regular(
            tex, tex_uv, pixel_to_tex_uv, frame) :
        tex2Daa_debug_dynamic(tex, tex_uv, pixel_to_tex_uv, frame);
#endif
}


#endif  //  TEX2DANTIALIAS_H

/////////////////////////  END TEX2DANTIALIAS  /////////////////////////

//#include "geometry-functions.h"

/////////////////////////  BEGIN GEOMETRY-FUNCTIONS  /////////////////////////

#ifndef GEOMETRY_FUNCTIONS_H
#define GEOMETRY_FUNCTIONS_H

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////

//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA


//////////////////////////////////  INCLUDES  //////////////////////////////////

// already included elsewhere
//#include "../user-settings.h"
//#include "derived-settings-and-constants.h"
//#include "bind-shader-h"


////////////////////////////  MACROS AND CONSTANTS  ////////////////////////////

//  Curvature-related constants:
#define MAX_POINT_CLOUD_SIZE 9


/////////////////////////////  CURVATURE FUNCTIONS /////////////////////////////

float2 quadratic_solve(const float a, const float b_over_2, const float c)
{
    //  Requires:   1.) a, b, and c are quadratic formula coefficients
    //              2.) b_over_2 = b/2.0 (simplifies terms to factor 2 out)
    //              3.) b_over_2 must be guaranteed < 0.0 (avoids a branch)
    //  Returns:    Returns float2(first_solution, discriminant), so the caller
    //              can choose how to handle the "no intersection" case.  The
    //              Kahan or Citardauq formula is used for numerical robustness.
    const float discriminant = b_over_2*b_over_2 - a*c;
    const float solution0 = c/(-b_over_2 + sqrt(discriminant));
    return float2(solution0, discriminant);
}

float2 intersect_sphere(const float3 view_vec, const float3 eye_pos_vec)
{
    //  Requires:   1.) view_vec and eye_pos_vec are 3D vectors in the sphere's
    //                  local coordinate frame (eye_pos_vec is a position, i.e.
    //                  a vector from the origin to the eye/camera)
    //              2.) geom_radius is a global containing the sphere's radius
    //  Returns:    Cast a ray of direction view_vec from eye_pos_vec at a
    //              sphere of radius geom_radius, and return the distance to
    //              the first intersection in units of length(view_vec).
    //              http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
    //  Quadratic formula coefficients (b_over_2 is guaranteed negative):
    const float a = dot(view_vec, view_vec);
    const float b_over_2 = dot(view_vec, eye_pos_vec);  //  * 2.0 factored out
    const float c = dot(eye_pos_vec, eye_pos_vec) - geom_radius*geom_radius;
    return quadratic_solve(a, b_over_2, c);
}

float2 intersect_cylinder(const float3 view_vec, const float3 eye_pos_vec)
{
    //  Requires:   1.) view_vec and eye_pos_vec are 3D vectors in the sphere's
    //                  local coordinate frame (eye_pos_vec is a position, i.e.
    //                  a vector from the origin to the eye/camera)
    //              2.) geom_radius is a global containing the cylinder's radius
    //  Returns:    Cast a ray of direction view_vec from eye_pos_vec at a
    //              cylinder of radius geom_radius, and return the distance to
    //              the first intersection in units of length(view_vec).  The
    //              derivation of the coefficients is in Christer Ericson's
    //              Real-Time Collision Detection, p. 195-196, and this version
    //              uses LaGrange's identity to reduce operations.
    //  Arbitrary "cylinder top" reference point for an infinite cylinder:
    const float3 cylinder_top_vec = float3(0.0, geom_radius, 0.0);
    const float3 cylinder_axis_vec = float3(0.0, 1.0, 0.0);//float3(0.0, 2.0*geom_radius, 0.0);
    const float3 top_to_eye_vec = eye_pos_vec - cylinder_top_vec;
    const float3 axis_x_view = cross(cylinder_axis_vec, view_vec);
    const float3 axis_x_top_to_eye = cross(cylinder_axis_vec, top_to_eye_vec);
    //  Quadratic formula coefficients (b_over_2 is guaranteed negative):
    const float a = dot(axis_x_view, axis_x_view);
    const float b_over_2 = dot(axis_x_top_to_eye, axis_x_view);
    const float c = dot(axis_x_top_to_eye, axis_x_top_to_eye) -
        geom_radius*geom_radius;//*dot(cylinder_axis_vec, cylinder_axis_vec);
    return quadratic_solve(a, b_over_2, c);
}

float2 cylinder_xyz_to_uv(const float3 intersection_pos_local,
    const float2 geom_aspect)
{
    //  Requires:   An xyz intersection position on a cylinder.
    //  Returns:    video_uv coords mapped to range [-0.5, 0.5]
    //  Mapping:    Define square_uv.x to be the signed arc length in xz-space,
    //              and define square_uv.y = -intersection_pos_local.y (+v = -y).
    //  Start with a numerically robust arc length calculation.
    const float angle_from_image_center = atan2(intersection_pos_local.x,
        intersection_pos_local.z);
    const float signed_arc_len = angle_from_image_center * geom_radius;
    //  Get a uv-mapping where [-0.5, 0.5] maps to a "square" area, then divide
    //  by the aspect ratio to stretch the mapping appropriately:
    const float2 square_uv = float2(signed_arc_len, -intersection_pos_local.y);
    const float2 video_uv = square_uv / geom_aspect;
    return video_uv;
}

float3 cylinder_uv_to_xyz(const float2 video_uv, const float2 geom_aspect)
{
    //  Requires:   video_uv coords mapped to range [-0.5, 0.5]
    //  Returns:    An xyz intersection position on a cylinder.  This is the
    //              inverse of cylinder_xyz_to_uv().
    //  Expand video_uv by the aspect ratio to get proportionate x/y lengths,
    //  then calculate an xyz position for the cylindrical mapping above.
    const float2 square_uv = video_uv * geom_aspect;
    const float arc_len = square_uv.x;
    const float angle_from_image_center = arc_len / geom_radius;
    const float x_pos = sin(angle_from_image_center) * geom_radius;
    const float z_pos = cos(angle_from_image_center) * geom_radius;
    //  Or: z = sqrt(geom_radius**2 - x**2)
    //  Or: z = geom_radius/sqrt(1.0 + tan(angle)**2), x = z * tan(angle)
    const float3 intersection_pos_local = float3(x_pos, -square_uv.y, z_pos);
    return intersection_pos_local;
}

float2 sphere_xyz_to_uv(const float3 intersection_pos_local,
    const float2 geom_aspect)
{
    //  Requires:   An xyz intersection position on a sphere.
    //  Returns:    video_uv coords mapped to range [-0.5, 0.5]
    //  Mapping:    First define square_uv.x/square_uv.y ==
    //              intersection_pos_local.x/intersection_pos_local.y.  Then,
    //              length(square_uv) is the arc length from the image center
    //              at (0.0, 0.0, geom_radius) along the tangent great circle.
    //              Credit for this mapping goes to cgwg: I never managed to
    //              understand his code, but he told me his mapping was based on
    //              great circle distances when I asked him about it, which
    //              informed this very similar (almost identical) mapping.
    //  Start with a numerically robust arc length calculation between the ray-
    //  sphere intersection point and the image center using a method posted by
    //  Roger Stafford on comp.soft-sys.matlab:
    //  https://groups.google.com/d/msg/comp.soft-sys.matlab/zNbUui3bjcA/c0HV_bHSx9cJ
    const float3 image_center_pos_local = float3(0.0, 0.0, geom_radius);
    const float cp_len =
        length(cross(intersection_pos_local, image_center_pos_local));
    const float dp = dot(intersection_pos_local, image_center_pos_local);
    const float angle_from_image_center = atan2(cp_len, dp);
    const float arc_len = angle_from_image_center * geom_radius;
    //  Get a uv-mapping where [-0.5, 0.5] maps to a "square" area, then divide
    //  by the aspect ratio to stretch the mapping appropriately:
    const float2 square_uv_unit = normalize(float2(intersection_pos_local.x,
        -intersection_pos_local.y));
    const float2 square_uv = arc_len * square_uv_unit;
    const float2 video_uv = square_uv / geom_aspect;
    return video_uv;
}

float3 sphere_uv_to_xyz(const float2 video_uv, const float2 geom_aspect)
{
    //  Requires:   video_uv coords mapped to range [-0.5, 0.5]
    //  Returns:    An xyz intersection position on a sphere.  This is the
    //              inverse of sphere_xyz_to_uv().
    //  Expand video_uv by the aspect ratio to get proportionate x/y lengths,
    //  then calculate an xyz position for the spherical mapping above.
    const float2 square_uv = video_uv * geom_aspect;
    //  Using length or sqrt here butchers the framerate on my 8800GTS if
    //  this function is called too many times, and so does taking the max
    //  component of square_uv/square_uv_unit (program length threshold?).
    //float arc_len = length(square_uv);
    const float2 square_uv_unit = normalize(square_uv);
    const float arc_len = square_uv.y/square_uv_unit.y;
    const float angle_from_image_center = arc_len / geom_radius;
    const float xy_dist_from_sphere_center =
        sin(angle_from_image_center) * geom_radius;
    //float2 xy_pos = xy_dist_from_sphere_center * (square_uv/FIX_ZERO(arc_len));
    const float2 xy_pos = xy_dist_from_sphere_center * square_uv_unit;
    const float z_pos = cos(angle_from_image_center) * geom_radius;
    const float3 intersection_pos_local = float3(xy_pos.x, -xy_pos.y, z_pos);
    return intersection_pos_local;
}

float2 sphere_alt_xyz_to_uv(const float3 intersection_pos_local,
    const float2 geom_aspect)
{
    //  Requires:   An xyz intersection position on a cylinder.
    //  Returns:    video_uv coords mapped to range [-0.5, 0.5]
    //  Mapping:    Define square_uv.x to be the signed arc length in xz-space,
    //              and define square_uv.y == signed arc length in yz-space.
    //  See cylinder_xyz_to_uv() for implementation details (very similar).
    const float2 angle_from_image_center = atan2(
        float2(intersection_pos_local.x, -intersection_pos_local.y),
        intersection_pos_local.zz);
    const float2 signed_arc_len = angle_from_image_center * geom_radius;
    const float2 video_uv = signed_arc_len / geom_aspect;
    return video_uv;
}

float3 sphere_alt_uv_to_xyz(const float2 video_uv, const float2 geom_aspect)
{
    //  Requires:   video_uv coords mapped to range [-0.5, 0.5]
    //  Returns:    An xyz intersection position on a sphere.  This is the
    //              inverse of sphere_alt_xyz_to_uv().
    //  See cylinder_uv_to_xyz() for implementation details (very similar).
    const float2 square_uv = video_uv * geom_aspect;
    const float2 arc_len = square_uv;
    const float2 angle_from_image_center = arc_len / geom_radius;
    const float2 xy_pos = sin(angle_from_image_center) * geom_radius;
    const float z_pos = sqrt(geom_radius*geom_radius - dot(xy_pos, xy_pos));
    return float3(xy_pos.x, -xy_pos.y, z_pos);
}

inline float2 intersect(const float3 view_vec_local, const float3 eye_pos_local,
    const float geom_mode)
{
    return geom_mode < 2.5 ? intersect_sphere(view_vec_local, eye_pos_local) :
        intersect_cylinder(view_vec_local, eye_pos_local);
}

inline float2 xyz_to_uv(const float3 intersection_pos_local,
    const float2 geom_aspect, const float geom_mode)
{
    return geom_mode < 1.5 ?
            sphere_xyz_to_uv(intersection_pos_local, geom_aspect) :
        geom_mode < 2.5 ?
            sphere_alt_xyz_to_uv(intersection_pos_local, geom_aspect) :
            cylinder_xyz_to_uv(intersection_pos_local, geom_aspect);
}

inline float3 uv_to_xyz(const float2 uv, const float2 geom_aspect,
    const float geom_mode)
{
    return geom_mode < 1.5 ? sphere_uv_to_xyz(uv, geom_aspect) :
        geom_mode < 2.5 ? sphere_alt_uv_to_xyz(uv, geom_aspect) :
        cylinder_uv_to_xyz(uv, geom_aspect);
}

float2 view_vec_to_uv(const float3 view_vec_local, const float3 eye_pos_local,
    const float2 geom_aspect, const float geom_mode, out float3 intersection_pos)
{
    //  Get the intersection point on the primitive, given an eye position
    //  and view vector already in its local coordinate frame:
    const float2 intersect_dist_and_discriminant = intersect(view_vec_local,
        eye_pos_local, geom_mode);
    const float3 intersection_pos_local = eye_pos_local +
        view_vec_local * intersect_dist_and_discriminant.x;
    //  Save the intersection position to an output parameter:
    intersection_pos = intersection_pos_local;
    //  Transform into uv coords, but give out-of-range coords if the
    //  view ray doesn't intersect the primitive in the first place:
    return intersect_dist_and_discriminant.y > 0.005 ?
        xyz_to_uv(intersection_pos_local, geom_aspect, geom_mode) : float2(1.0);
}

float3 get_ideal_global_eye_pos_for_points(float3 eye_pos,
    const float2 geom_aspect, const float3 global_coords[MAX_POINT_CLOUD_SIZE],
    const int num_points)
{
    //  Requires:   Parameters:
    //              1.) Starting eye_pos is a global 3D position at which the
    //                  camera contains all points in global_coords[] in its FOV
    //              2.) geom_aspect = get_aspect_vector(
    //                      output_size.x / output_size.y);
    //              3.) global_coords is a point cloud containing global xyz
    //                  coords of extreme points on the simulated CRT screen.
    //              Globals:
    //              1.) geom_view_dist must be > 0.0.  It controls the "near
    //                  plane" used to interpret flat_video_uv as a view
    //                  vector, which controls the field of view (FOV).
    //              Eyespace coordinate frame: +x = right, +y = up, +z = back
    //  Returns:    Return an eye position at which the point cloud spans as
    //              much of the screen as possible (given the FOV controlled by
    //              geom_view_dist) without being cropped or sheared.
    //  Algorithm:
    //  1.) Move the eye laterally to a point which attempts to maximize the
    //      the amount we can move forward without clipping the CRT screen.
    //  2.) Move forward by as much as possible without clipping the CRT.
    //  Get the allowed movement range by solving for the eye_pos offsets
    //  that result in each point being projected to a screen edge/corner in
    //  pseudo-normalized device coords (where xy ranges from [-0.5, 0.5]
    //  and z = eyespace z):
    //      pndc_coord = float3(float2(eyespace_xyz.x, -eyespace_xyz.y)*
    //      geom_view_dist / (geom_aspect * -eyespace_xyz.z), eyespace_xyz.z);
    //  Notes:
    //  The field of view is controlled by geom_view_dist's magnitude relative to
    //  the view vector's x and y components:
    //      view_vec.xy ranges from [-0.5, 0.5] * geom_aspect
    //      view_vec.z = -geom_view_dist
    //  But for the purposes of perspective divide, it should be considered:
    //      view_vec.xy ranges from [-0.5, 0.5] * geom_aspect / geom_view_dist
    //      view_vec.z = -1.0
    static const int max_centering_iters = 1;  //  Keep for easy testing.
    for(int iter = 0; iter < max_centering_iters; iter++)
    {
        //  0.) Get the eyespace coordinates of our point cloud:
        float3 eyespace_coords[MAX_POINT_CLOUD_SIZE];
        for(int i = 0; i < num_points; i++)
        {
            eyespace_coords[i] = global_coords[i] - eye_pos;
        }
        //  1a.)For each point, find out how far we can move eye_pos in each
        //      lateral direction without the point clipping the frustum.
        //      Eyespace +y = up, screenspace +y = down, so flip y after
        //      applying the eyespace offset (on the way to "clip space").
        //  Solve for two offsets per point based on:
        //      (eyespace_xyz.xy - offset_dr) * float2(1.0, -1.0) *
        //      geom_view_dist / (geom_aspect * -eyespace_xyz.z) = float2(-0.5)
        //      (eyespace_xyz.xy - offset_dr) * float2(1.0, -1.0) *
        //      geom_view_dist / (geom_aspect * -eyespace_xyz.z) = float2(0.5)
        //  offset_ul and offset_dr represent the farthest we can move the
        //  eye_pos up-left and down-right.  Save the min of all offset_dr's
        //  and the max of all offset_ul's (since it's negative).
        float abs_radius = abs(geom_radius);  //  In case anyone gets ideas. ;)
        float2 offset_dr_min = float2(10.0 * abs_radius, 10.0 * abs_radius);
        float2 offset_ul_max = float2(-10.0 * abs_radius, -10.0 * abs_radius);
        for(int i = 0; i < num_points; i++)
        {
            static const float2 flipy = float2(1.0, -1.0);
            float3 eyespace_xyz = eyespace_coords[i];
            float2 offset_dr = eyespace_xyz.xy - float2(-0.5) *
                (geom_aspect * -eyespace_xyz.z) / (geom_view_dist * flipy);
            float2 offset_ul = eyespace_xyz.xy - float2(0.5) *
                (geom_aspect * -eyespace_xyz.z) / (geom_view_dist * flipy);
            offset_dr_min = min(offset_dr_min, offset_dr);
            offset_ul_max = max(offset_ul_max, offset_ul);
        }
        //  1b.)Update eye_pos: Adding the average of offset_ul_max and
        //      offset_dr_min gives it equal leeway on the top vs. bottom
        //      and left vs. right.  Recalculate eyespace_coords accordingly.
        float2 center_offset = 0.5 * (offset_ul_max + offset_dr_min);
        eye_pos.xy += center_offset;
        for(int i = 0; i < num_points; i++)
        {
            eyespace_coords[i] = global_coords[i] - eye_pos;
        }
        //  2a.)For each point, find out how far we can move eye_pos forward
        //      without the point clipping the frustum.  Flip the y
        //      direction in advance (matters for a later step, not here).
        //      Solve for four offsets per point based on:
        //      eyespace_xyz_flipy.x * geom_view_dist /
        //          (geom_aspect.x * (offset_z - eyespace_xyz_flipy.z)) =-0.5
        //      eyespace_xyz_flipy.y * geom_view_dist /
        //          (geom_aspect.y * (offset_z - eyespace_xyz_flipy.z)) =-0.5
        //      eyespace_xyz_flipy.x * geom_view_dist /
        //          (geom_aspect.x * (offset_z - eyespace_xyz_flipy.z)) = 0.5
        //      eyespace_xyz_flipy.y * geom_view_dist /
        //          (geom_aspect.y * (offset_z - eyespace_xyz_flipy.z)) = 0.5
        //      We'll vectorize the actual computation.  Take the maximum of
        //      these four for a single offset, and continue taking the max
        //      for every point (use max because offset.z is negative).
        float offset_z_max = -10.0 * geom_radius * geom_view_dist;
        for(int i = 0; i < num_points; i++)
        {
            float3 eyespace_xyz_flipy = eyespace_coords[i] *
                float3(1.0, -1.0, 1.0);
            float4 offset_zzzz = eyespace_xyz_flipy.zzzz +
                (eyespace_xyz_flipy.xyxy * geom_view_dist) /
                (float4(-0.5, -0.5, 0.5, 0.5) * float4(geom_aspect, geom_aspect));
            //  Ignore offsets that push positive x/y values to opposite
            //  boundaries, and vice versa, and don't let the camera move
            //  past a point in the dead center of the screen:
            offset_z_max = (eyespace_xyz_flipy.x < 0.0) ?
                max(offset_z_max, offset_zzzz.x) : offset_z_max;
            offset_z_max = (eyespace_xyz_flipy.y < 0.0) ?
                max(offset_z_max, offset_zzzz.y) : offset_z_max;
            offset_z_max = (eyespace_xyz_flipy.x > 0.0) ?
                max(offset_z_max, offset_zzzz.z) : offset_z_max;
            offset_z_max = (eyespace_xyz_flipy.y > 0.0) ?
                max(offset_z_max, offset_zzzz.w) : offset_z_max;
            offset_z_max = max(offset_z_max, eyespace_xyz_flipy.z);
        }
        //  2b.)Update eye_pos: Add the maximum (smallest negative) z offset.
        eye_pos.z += offset_z_max;
    }
    return eye_pos;
}

float3 get_ideal_global_eye_pos(const float3x3 local_to_global,
    const float2 geom_aspect, const float geom_mode)
{
    //  Start with an initial eye_pos that includes the entire primitive
    //  (sphere or cylinder) in its field-of-view:
    const float3 high_view = float3(0.0, geom_aspect.y, -geom_view_dist);
    const float3 low_view = high_view * float3(1.0, -1.0, 1.0);
    const float len_sq = dot(high_view, high_view);
    const float fov = abs(acos(dot(high_view, low_view)/len_sq));
    //  Trigonometry/similar triangles say distance = geom_radius/sin(fov/2):
    const float eye_z_spherical = geom_radius/sin(fov*0.5);
    const float3 eye_pos = geom_mode < 2.5 ?
        float3(0.0, 0.0, eye_z_spherical) :
        float3(0.0, 0.0, max(geom_view_dist, eye_z_spherical));

    //  Get global xyz coords of extreme sample points on the simulated CRT
    //  screen.  Start with the center, edge centers, and corners of the
    //  video image.  We can't ignore backfacing points: They're occluded
    //  by closer points on the primitive, but they may NOT be occluded by
    //  the convex hull of the remaining samples (i.e. the remaining convex
    //  hull might not envelope points that do occlude a back-facing point.)
    static const int num_points = MAX_POINT_CLOUD_SIZE;
    float3 global_coords[MAX_POINT_CLOUD_SIZE];
    global_coords[0] = mul(local_to_global, uv_to_xyz(float2(0.0, 0.0), geom_aspect, geom_mode));
    global_coords[1] = mul(local_to_global, uv_to_xyz(float2(0.0, -0.5), geom_aspect, geom_mode));
    global_coords[2] = mul(local_to_global, uv_to_xyz(float2(0.0, 0.5), geom_aspect, geom_mode));
    global_coords[3] = mul(local_to_global, uv_to_xyz(float2(-0.5, 0.0), geom_aspect, geom_mode));
    global_coords[4] = mul(local_to_global, uv_to_xyz(float2(0.5, 0.0), geom_aspect, geom_mode));
    global_coords[5] = mul(local_to_global, uv_to_xyz(float2(-0.5, -0.5), geom_aspect, geom_mode));
    global_coords[6] = mul(local_to_global, uv_to_xyz(float2(0.5, -0.5), geom_aspect, geom_mode));
    global_coords[7] = mul(local_to_global, uv_to_xyz(float2(-0.5, 0.5), geom_aspect, geom_mode));
    global_coords[8] = mul(local_to_global, uv_to_xyz(float2(0.5, 0.5), geom_aspect, geom_mode));
    //  Adding more inner image points could help in extreme cases, but too many
    //  points will kille the framerate.  For safety, default to the initial
    //  eye_pos if any z coords are negative:
    float num_negative_z_coords = 0.0;
    for(int i = 0; i < num_points; i++)
    {
        num_negative_z_coords += float(global_coords[0].z < 0.0);
    }
    //  Outsource the optimized eye_pos calculation:
    return num_negative_z_coords > 0.5 ? eye_pos :
        get_ideal_global_eye_pos_for_points(eye_pos, geom_aspect,
            global_coords, num_points);
}

float3x3 get_pixel_to_object_matrix(const float3x3 global_to_local,
    const float3 eye_pos_local, const float3 view_vec_global,
    const float3 intersection_pos_local, const float3 normal,
    const float2 output_size_inv)
{
    //  Requires:   See get_curved_video_uv_coords_and_tangent_matrix for
    //              descriptions of each parameter.
    //  Returns:    Return a transformation matrix from 2D pixel-space vectors
    //              (where (+1.0, +1.0) is a vector to one pixel down-right,
    //              i.e. same directionality as uv texels) to 3D object-space
    //              vectors in the CRT's local coordinate frame (right-handed)
    //              ***which are tangent to the CRT surface at the intersection
    //              position.***  (Basically, we want to convert pixel-space
    //              vectors to 3D vectors along the CRT's surface, for later
    //              conversion to uv vectors.)
    //  Shorthand inputs:
    const float3 pos = intersection_pos_local;
    const float3 eye_pos = eye_pos_local;
    //  Get a piecewise-linear matrix transforming from "pixelspace" offset
    //  vectors (1.0 = one pixel) to object space vectors in the tangent
    //  plane (faster than finding 3 view-object intersections).
    //  1.) Get the local view vecs for the pixels to the right and down:
    const float3 view_vec_right_global = view_vec_global +
        float3(output_size_inv.x, 0.0, 0.0);
    const float3 view_vec_down_global = view_vec_global +
        float3(0.0, -output_size_inv.y, 0.0);
    const float3 view_vec_right_local =
        mul(global_to_local, view_vec_right_global);
    const float3 view_vec_down_local =
        mul(global_to_local, view_vec_down_global);
    //  2.) Using the true intersection point, intersect the neighboring
    //      view vectors with the tangent plane:
    const float3 intersection_vec_dot_normal = float3(dot(pos - eye_pos, normal), dot(pos - eye_pos, normal), dot(pos - eye_pos, normal));
    const float3 right_pos = eye_pos + (intersection_vec_dot_normal /
        dot(view_vec_right_local, normal))*view_vec_right_local;
    const float3 down_pos = eye_pos + (intersection_vec_dot_normal /
        dot(view_vec_down_local, normal))*view_vec_down_local;
    //  3.) Subtract the original intersection pos from its neighbors; the
    //      resulting vectors are object-space vectors tangent to the plane.
    //      These vectors are the object-space transformations of (1.0, 0.0)
    //      and (0.0, 1.0) pixel offsets, so they form the first two basis
    //      vectors of a pixelspace to object space transformation.  This
    //      transformation is 2D to 3D, so use (0, 0, 0) for the third vector.
    const float3 object_right_vec = right_pos - pos;
    const float3 object_down_vec = down_pos - pos;
    const float3x3 pixel_to_object = float3x3(
        object_right_vec.x, object_down_vec.x, 0.0,
        object_right_vec.y, object_down_vec.y, 0.0,
        object_right_vec.z, object_down_vec.z, 0.0);
    return pixel_to_object;
}

float3x3 get_object_to_tangent_matrix(const float3 intersection_pos_local,
    const float3 normal, const float2 geom_aspect, const float geom_mode)
{
    //  Requires:   See get_curved_video_uv_coords_and_tangent_matrix for
    //              descriptions of each parameter.
    //  Returns:    Return a transformation matrix from 3D object-space vectors
    //              in the CRT's local coordinate frame (right-handed, +y = up)
    //              to 2D video_uv vectors (+v = down).
    //  Description:
    //  The TBN matrix formed by the [tangent, bitangent, normal] basis
    //  vectors transforms ordinary vectors from tangent->object space.
    //  The cotangent matrix formed by the [cotangent, cobitangent, normal]
    //  basis vectors transforms normal vectors (covectors) from
    //  tangent->object space.  It's the inverse-transpose of the TBN matrix.
    //  We want the inverse of the TBN matrix (transpose of the cotangent
    //  matrix), which transforms ordinary vectors from object->tangent space.
    //  Start by calculating the relevant basis vectors in accordance with
    //  Christian Schüler's blog post "Followup: Normal Mapping Without
    //  Precomputed Tangents":  http://www.thetenthplanet.de/archives/1180
    //  With our particular uv mapping, the scale of the u and v directions
    //  is determined entirely by the aspect ratio for cylindrical and ordinary
    //  spherical mappings, and so tangent and bitangent lengths are also
    //  determined by it (the alternate mapping is more complex).  Therefore, we
    //  must ensure appropriate cotangent and cobitangent lengths as well.
    //  Base these off the uv<=>xyz mappings for each primitive.
    const float3 pos = intersection_pos_local;
    static const float3 x_vec = float3(1.0, 0.0, 0.0);
    static const float3 y_vec = float3(0.0, 1.0, 0.0);
    //  The tangent and bitangent vectors correspond with increasing u and v,
    //  respectively.  Mathematically we'd base the cotangent/cobitangent on
    //  those, but we'll compute the cotangent/cobitangent directly when we can.
    float3 cotangent_unscaled, cobitangent_unscaled;
    //  geom_mode should be constant-folded without RUNTIME_GEOMETRY_MODE.
    if(geom_mode < 1.5)
    {
        //  Sphere:
        //  tangent = normalize(cross(normal, cross(x_vec, pos))) * geom_aspect.x
        //  bitangent = normalize(cross(cross(y_vec, pos), normal)) * geom_aspect.y
        //  inv_determinant = 1.0/length(cross(bitangent, tangent))
        //  cotangent = cross(normal, bitangent) * inv_determinant
        //            == normalize(cross(y_vec, pos)) * geom_aspect.y * inv_determinant
        //  cobitangent = cross(tangent, normal) * inv_determinant
        //            == normalize(cross(x_vec, pos)) * geom_aspect.x * inv_determinant
        //  Simplified (scale by inv_determinant below):
        cotangent_unscaled = normalize(cross(y_vec, pos)) * geom_aspect.y;
        cobitangent_unscaled = normalize(cross(x_vec, pos)) * geom_aspect.x;
    }
    else if(geom_mode < 2.5)
    {
        //  Sphere, alternate mapping:
        //  This mapping works a bit like the cylindrical mapping in two
        //  directions, which makes the lengths and directions more complex.
        //  Unfortunately, I can't find much of a shortcut:
        const float3 tangent = normalize(
            cross(y_vec, float3(pos.x, 0.0, pos.z))) * geom_aspect.x;
        const float3 bitangent = normalize(
            cross(x_vec, float3(0.0, pos.yz))) * geom_aspect.y;
        cotangent_unscaled = cross(normal, bitangent);
        cobitangent_unscaled = cross(tangent, normal);
    }
    else
    {
        //  Cylinder:
        //  tangent = normalize(cross(y_vec, normal)) * geom_aspect.x;
        //  bitangent = float3(0.0, -geom_aspect.y, 0.0);
        //  inv_determinant = 1.0/length(cross(bitangent, tangent))
        //  cotangent = cross(normal, bitangent) * inv_determinant
        //            == normalize(cross(y_vec, pos)) * geom_aspect.y * inv_determinant
        //  cobitangent = cross(tangent, normal) * inv_determinant
        //            == float3(0.0, -geom_aspect.x, 0.0) * inv_determinant
        cotangent_unscaled = cross(y_vec, normal) * geom_aspect.y;
        cobitangent_unscaled = float3(0.0, -geom_aspect.x, 0.0);
    }
    const float3 computed_normal =
        cross(cobitangent_unscaled, cotangent_unscaled);
    const float inv_determinant = rsqrt(dot(computed_normal, computed_normal));
    const float3 cotangent = cotangent_unscaled * inv_determinant;
    const float3 cobitangent = cobitangent_unscaled * inv_determinant;
    //  The [cotangent, cobitangent, normal] column vecs form the cotangent
    //  frame, i.e. the inverse-transpose TBN matrix.  Get its transpose:
    const float3x3 object_to_tangent = float3x3(cotangent, cobitangent, normal);
    return object_to_tangent;
}

float2 get_curved_video_uv_coords_and_tangent_matrix(
    const float2 flat_video_uv, const float3 eye_pos_local,
    const float2 output_size_inv, const float2 geom_aspect,
    const float geom_mode, const float3x3 global_to_local,
    out float2x2 pixel_to_tangent_video_uv)
{
    //  Requires:   Parameters:
    //              1.) flat_video_uv coords are in range [0.0, 1.0], where
    //                  (0.0, 0.0) is the top-left corner of the screen and
    //                  (1.0, 1.0) is the bottom-right corner.
    //              2.) eye_pos_local is the 3D camera position in the simulated
    //                  CRT's local coordinate frame.  For best results, it must
    //                  be computed based on the same geom_view_dist used here.
    //              3.) output_size_inv = float2(1.0)/output_size
    //              4.) geom_aspect = get_aspect_vector(
    //                      output_size.x / output_size.y);
    //              5.) geom_mode is a static or runtime mode setting:
    //                  0 = off, 1 = sphere, 2 = sphere alt., 3 = cylinder
    //              6.) global_to_local is a 3x3 matrix transforming (ordinary)
    //                  worldspace vectors to the CRT's local coordinate frame
    //              Globals:
    //              1.) geom_view_dist must be > 0.0.  It controls the "near
    //                  plane" used to interpret flat_video_uv as a view
    //                  vector, which controls the field of view (FOV).
    //  Returns:    Return final uv coords in [0.0, 1.0], and return a pixel-
    //              space to video_uv tangent-space matrix in the out parameter.
    //              (This matrix assumes pixel-space +y = down, like +v = down.)
    //              We'll transform flat_video_uv into a view vector, project
    //              the view vector from the camera/eye, intersect with a sphere
    //              or cylinder representing the simulated CRT, and convert the
    //              intersection position into final uv coords and a local
    //              transformation matrix.
    //  First get the 3D view vector (geom_aspect and geom_view_dist are globals):
    //  1.) Center uv around (0.0, 0.0) and make (-0.5, -0.5) and (0.5, 0.5)
    //      correspond to the top-left/bottom-right output screen corners.
    //  2.) Multiply by geom_aspect to preemptively "undo" Retroarch's screen-
    //      space 2D aspect correction.  We'll reapply it in uv-space.
    //  3.) (x, y) = (u, -v), because +v is down in 2D screenspace, but +y
    //      is up in 3D worldspace (enforce a right-handed system).
    //  4.) The view vector z controls the "near plane" distance and FOV.
    //      For the effect of "looking through a window" at a CRT, it should be
    //      set equal to the user's distance from their physical screen, in
    //      units of the viewport's physical diagonal size.
    const float2 view_uv = (flat_video_uv - float2(0.5)) * geom_aspect;
    const float3 view_vec_global =
        float3(view_uv.x, -view_uv.y, -geom_view_dist);
    //  Transform the view vector into the CRT's local coordinate frame, convert
    //  to video_uv coords, and get the local 3D intersection position:
    const float3 view_vec_local = mul(global_to_local, view_vec_global);
    float3 pos;
    const float2 centered_uv = view_vec_to_uv(
        view_vec_local, eye_pos_local, geom_aspect, geom_mode, pos);
    const float2 video_uv = centered_uv + float2(0.5);
    //  Get a pixel-to-tangent-video-uv matrix.  The caller could deal with
    //  all but one of these cases, but that would be more complicated.
    #ifdef DRIVERS_ALLOW_DERIVATIVES
        //  Derivatives obtain a matrix very fast, but the direction of pixel-
        //  space +y seems to depend on the pass.  Enforce the correct direction
        //  on a best-effort basis (but it shouldn't matter for antialiasing).
        const float2 duv_dx = ddx(video_uv);
        const float2 duv_dy = ddy(video_uv);
        #ifdef LAST_PASS
            pixel_to_tangent_video_uv = float2x2(
                duv_dx.x, duv_dy.x,
                -duv_dx.y, -duv_dy.y);
        #else
            pixel_to_tangent_video_uv = float2x2(
                duv_dx.x, duv_dy.x,
                duv_dx.y, duv_dy.y);
        #endif
    #else
        //  Manually define a transformation matrix.  We'll assume pixel-space
        //  +y = down, just like +v = down.
        if(geom_force_correct_tangent_matrix)
        {
            //  Get the surface normal based on the local intersection position:
            const float3 normal_base = geom_mode < 2.5 ? pos :
                float3(pos.x, 0.0, pos.z);
            const float3 normal = normalize(normal_base);
            //  Get pixel-to-object and object-to-tangent matrices and combine
            //  them into a 2x2 pixel-to-tangent matrix for video_uv offsets:
            const float3x3 pixel_to_object = get_pixel_to_object_matrix(
                global_to_local, eye_pos_local, view_vec_global, pos, normal,
                output_size_inv);
            const float3x3 object_to_tangent = get_object_to_tangent_matrix(
                pos, normal, geom_aspect, geom_mode);
            const float3x3 pixel_to_tangent3x3 =
                mul(object_to_tangent, pixel_to_object);
            pixel_to_tangent_video_uv = float2x2(
                pixel_to_tangent3x3[0][0], pixel_to_tangent3x3[0][1], pixel_to_tangent3x3[1][0], pixel_to_tangent3x3[1][1]);//._m00_m01_m10_m11); //TODO/FIXME: needs to correct for column-major??
        }
        else
        {
            //  Ignore curvature, and just consider flat scaling.  The
            //  difference is only apparent with strong curvature:
            pixel_to_tangent_video_uv = float2x2(
                output_size_inv.x, 0.0, 0.0, output_size_inv.y);
        }
    #endif
    return video_uv;
}

float get_border_dim_factor(const float2 video_uv, const float2 geom_aspect)
{
    //  COPYRIGHT NOTE FOR THIS FUNCTION:
    //  Copyright (C) 2010-2012 cgwg, 2014 TroggleMonkey
    //  This function uses an algorithm first coded in several of cgwg's GPL-
    //  licensed lines in crt-geom-curved.cg and its ancestors.  The line
    //  between algorithm and code is nearly indistinguishable here, so it's
    //  unclear whether I could even release this project under a non-GPL
    //  license with this function included.

    //  Calculate border_dim_factor from the proximity to uv-space image
    //  borders; geom_aspect/border_size/border/darkness/border_compress are globals:
    const float2 edge_dists = min(video_uv, float2(1.0) - video_uv) *
        geom_aspect;
    const float2 border_penetration =
        max(float2(border_size) - edge_dists, float2(0.0));
    const float penetration_ratio = length(border_penetration)/border_size;
    const float border_escape_ratio = max(1.0 - penetration_ratio, 0.0);
    const float border_dim_factor =
        pow(border_escape_ratio, border_darkness) * max(1.0, border_compress);
    return min(border_dim_factor, 1.0);
}



#endif  //  GEOMETRY_FUNCTIONS_H

/////////////////////////  END GEOMETRY-FUNCTIONS  /////////////////////////

///////////////////////////////////  HELPERS  //////////////////////////////////

float2x2 mul_scale(float2 scale, float2x2 matrix)
{
    //float2x2 scale_matrix = float2x2(scale.x, 0.0, 0.0, scale.y);
    //return mul(scale_matrix, matrix);
    float4 intermed = float4(matrix[0][0],matrix[0][1],matrix[1][0],matrix[1][1]) * scale.xxyy;
    return float2x2(intermed.x, intermed.y, intermed.z, intermed.w);
}

#undef COMPAT_PRECISION
#undef COMPAT_TEXTURE

void main() {
    //  Localize some parameters:
    const float2 geom_aspect = geom_aspect_and_overscan.xy;
    const float2 geom_overscan = geom_aspect_and_overscan.zw;
    const float2 video_size_inv = video_and_texture_size_inv.xy;
    const float2 texture_size_inv = video_and_texture_size_inv.zw;
    //const float2 output_size_inv = output_size_inv;
    #ifdef RUNTIME_GEOMETRY_TILT
        const float3x3 global_to_local = float3x3(global_to_local_row0,
            global_to_local_row1, global_to_local_row2);
    #else
        static const float3x3 global_to_local = geom_global_to_local_static;
    #endif
    #ifdef RUNTIME_GEOMETRY_MODE
        const float geom_mode = geom_mode_runtime;
    #else
        static const float geom_mode = geom_mode_static;
    #endif

    //  Get flat and curved texture coords for the current fragment point sample
    //  and a pixel_to_tangent_video_uv matrix for transforming pixel offsets:
    //  video_uv = relative position in video frame, mapped to [0.0, 1.0] range
    //  tex_uv = relative position in padded texture, mapped to [0.0, 1.0] range
    const float2 flat_video_uv = tex_uv * (texture_size * video_size_inv);
    float2x2 pixel_to_video_uv;
    float2 video_uv_no_geom_overscan;
    if(geom_mode > 0.5)
    {
        video_uv_no_geom_overscan =
            get_curved_video_uv_coords_and_tangent_matrix(flat_video_uv,
                eye_pos_local, output_size_inv, geom_aspect,
                geom_mode, global_to_local, pixel_to_video_uv);
    }
    else
    {
        video_uv_no_geom_overscan = flat_video_uv;
        pixel_to_video_uv = float2x2(
            output_size_inv.x, 0.0, 0.0, output_size_inv.y);
    }
    //  Correct for overscan here (not in curvature code):
    const float2 video_uv =
        (video_uv_no_geom_overscan - float2(0.5, 0.5))/geom_overscan + float2(0.5, 0.5);
    const float2 tex_uv = video_uv * (video_size * texture_size_inv);

    //  Get a matrix transforming pixel vectors to tex_uv vectors:
    const float2x2 pixel_to_tex_uv =
        mul_scale(video_size * texture_size_inv /
            geom_aspect_and_overscan.zw, pixel_to_video_uv);

    //  Sample!  Skip antialiasing if aa_level < 0.5 or both of these hold:
    //  1.) Geometry/curvature isn't used
    //  2.) Overscan == float2(1.0, 1.0)
    //  Skipping AA is sharper, but it's only faster with dynamic branches.
    const float2 abs_aa_r_offset = abs(get_aa_subpixel_r_offset());
    const bool need_subpixel_aa = abs_aa_r_offset.x + abs_aa_r_offset.y > 0.0;
    float3 color;
    if(aa_level > 0.5 && (geom_mode > 0.5 || any(bool2((geom_overscan.x != 1.0), (geom_overscan.y != 1.0)))))
    {
        //  Sample the input with antialiasing (due to sharp phosphors, etc.):
        color = tex2Daa(input_texture, tex_uv, pixel_to_tex_uv, float(frame_count));
    }

    else if(aa_level > 0.5 && need_subpixel_aa)
    {
        //  Sample at each subpixel location:
        color = tex2Daa_subpixel_weights_only(
            input_texture, tex_uv, pixel_to_tex_uv);
    }
    else
    {
        color = tex2D_linearize(input_texture, tex_uv).rgb;
    }

    //  Dim borders and output the final result:
    const float border_dim_factor = get_border_dim_factor(video_uv, geom_aspect);
    const float3 final_color = color * border_dim_factor;

    FragColor = encode_output(float4(final_color, 1.0));
}