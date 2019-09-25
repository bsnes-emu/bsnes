#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 vTexCoord;
   vec2 tex_uv;
   vec2 blur_dxdy;
   vec2 uv_scanline_step;
   float estimated_viewport_size_x;
   vec2 texture_size_inv;
   vec2 tex_uv_to_pixel_scale;
};

uniform vec4 targetSize;
uniform vec4 sourceSize[];
uniform int phase;

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
#define geom_mode_runtime 0.000000
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
#define atan2(x,y) atan(y,x)
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

#define ORIG_LINEARIZEDvideo_size sourceSize[1].xy
#define ORIG_LINEARIZEDtexture_size sourceSize[1].xy

///////////////////////////////  VERTEX INCLUDES  ///////////////////////////////

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

//#include "scanline-functions.h"

/////////////////////////////  BEGIN SCANLINE-FUNCTIONS  ////////////////////////////

#ifndef SCANLINE_FUNCTIONS_H
#define SCANLINE_FUNCTIONS_H

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

//#include "../../../../include/special-functions.h"

///////////////////////////  BEGIN SPECIAL-FUNCTIONS  //////////////////////////

#ifndef SPECIAL_FUNCTIONS_H
#define SPECIAL_FUNCTIONS_H

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

//  This file implements the following mathematical special functions:
//  1.) erf() = 2/sqrt(pi) * indefinite_integral(e**(-x**2))
//  2.) gamma(s), a real-numbered extension of the integer factorial function
//  It also implements normalized_ligamma(s, z), a normalized lower incomplete
//  gamma function for s < 0.5 only.  Both gamma() and normalized_ligamma() can
//  be called with an _impl suffix to use an implementation version with a few
//  extra precomputed parameters (which may be useful for the caller to reuse).
//  See below for details.
//
//  Design Rationale:
//  Pretty much every line of code in this file is duplicated four times for
//  different input types (float4/float3/float2/float).  This is unfortunate,
//  but Cg doesn't allow function templates.  Macros would be far less verbose,
//  but they would make the code harder to document and read.  I don't expect
//  these functions will require a whole lot of maintenance changes unless
//  someone ever has need for more robust incomplete gamma functions, so code
//  duplication seems to be the lesser evil in this case.


///////////////////////////  GAUSSIAN ERROR FUNCTION  //////////////////////////

float4 erf6(float4 x)
{
    //  Requires:   x is the standard parameter to erf().
    //  Returns:    Return an Abramowitz/Stegun approximation of erf(), where:
    //                  erf(x) = 2/sqrt(pi) * integral(e**(-x**2))
    //              This approximation has a max absolute error of 2.5*10**-5
    //              with solid numerical robustness and efficiency.  See:
	//                  https://en.wikipedia.org/wiki/Error_function#Approximation_with_elementary_functions
	static const float4 one = float4(1.0);
	const float4 sign_x = sign(x);
	const float4 t = one/(one + 0.47047*abs(x));
	const float4 result = one - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

float3 erf6(const float3 x)
{
    //  Float3 version:
	static const float3 one = float3(1.0);
	const float3 sign_x = sign(x);
	const float3 t = one/(one + 0.47047*abs(x));
	const float3 result = one - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

float2 erf6(const float2 x)
{
    //  Float2 version:
	static const float2 one = float2(1.0);
	const float2 sign_x = sign(x);
	const float2 t = one/(one + 0.47047*abs(x));
	const float2 result = one - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

float erf6(const float x)
{
    //  Float version:
	const float sign_x = sign(x);
	const float t = 1.0/(1.0 + 0.47047*abs(x));
	const float result = 1.0 - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

float4 erft(const float4 x)
{
    //  Requires:   x is the standard parameter to erf().
    //  Returns:    Approximate erf() with the hyperbolic tangent.  The error is
    //              visually noticeable, but it's blazing fast and perceptually
    //              close...at least on ATI hardware.  See:
    //                  http://www.maplesoft.com/applications/view.aspx?SID=5525&view=html
    //  Warning:    Only use this if your hardware drivers correctly implement
    //              tanh(): My nVidia 8800GTS returns garbage output.
	return tanh(1.202760580 * x);
}

float3 erft(const float3 x)
{
    //  Float3 version:
	return tanh(1.202760580 * x);
}

float2 erft(const float2 x)
{
    //  Float2 version:
	return tanh(1.202760580 * x);
}

float erft(const float x)
{
    //  Float version:
	return tanh(1.202760580 * x);
}

inline float4 erf(const float4 x)
{
    //  Requires:   x is the standard parameter to erf().
    //  Returns:    Some approximation of erf(x), depending on user settings.
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

inline float3 erf(const float3 x)
{
    //  Float3 version:
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

inline float2 erf(const float2 x)
{
    //  Float2 version:
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

inline float erf(const float x)
{
    //  Float version:
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}


///////////////////////////  COMPLETE GAMMA FUNCTION  //////////////////////////

float4 gamma_impl(const float4 s, const float4 s_inv)
{
    //  Requires:   1.) s is the standard parameter to the gamma function, and
    //                  it should lie in the [0, 36] range.
    //              2.) s_inv = 1.0/s.  This implementation function requires
    //                  the caller to precompute this value, giving users the
    //                  opportunity to reuse it.
    //  Returns:    Return approximate gamma function (real-numbered factorial)
    //              output using the Lanczos approximation with two coefficients
    //              calculated using Paul Godfrey's method here:
    //                  http://my.fit.edu/~gabdo/gamma.txt
    //              An optimal g value for s in [0, 36] is ~1.12906830989, with
    //              a maximum relative error of 0.000463 for 2**16 equally
    //              evals.  We could use three coeffs (0.0000346 error) without
    //              hurting latency, but this allows more parallelism with
    //              outside instructions.
	static const float4 g = float4(1.12906830989);
	static const float4 c0 = float4(0.8109119309638332633713423362694399653724431);
	static const float4 c1 = float4(0.4808354605142681877121661197951496120000040);
	static const float4 e = float4(2.71828182845904523536028747135266249775724709);
	const float4 sph = s + float4(0.5);
	const float4 lanczos_sum = c0 + c1/(s + float4(1.0));
	const float4 base = (sph + g)/e;  //  or (s + g + float4(0.5))/e
	//  gamma(s + 1) = base**sph * lanczos_sum; divide by s for gamma(s).
	//  This has less error for small s's than (s -= 1.0) at the beginning.
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

float3 gamma_impl(const float3 s, const float3 s_inv)
{
    //  Float3 version:
	static const float3 g = float3(1.12906830989);
	static const float3 c0 = float3(0.8109119309638332633713423362694399653724431);
	static const float3 c1 = float3(0.4808354605142681877121661197951496120000040);
	static const float3 e = float3(2.71828182845904523536028747135266249775724709);
	const float3 sph = s + float3(0.5);
	const float3 lanczos_sum = c0 + c1/(s + float3(1.0));
	const float3 base = (sph + g)/e;
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

float2 gamma_impl(const float2 s, const float2 s_inv)
{
    //  Float2 version:
	static const float2 g = float2(1.12906830989);
	static const float2 c0 = float2(0.8109119309638332633713423362694399653724431);
	static const float2 c1 = float2(0.4808354605142681877121661197951496120000040);
	static const float2 e = float2(2.71828182845904523536028747135266249775724709);
	const float2 sph = s + float2(0.5);
	const float2 lanczos_sum = c0 + c1/(s + float2(1.0));
	const float2 base = (sph + g)/e;
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

float gamma_impl(const float s, const float s_inv)
{
    //  Float version:
	static const float g = 1.12906830989;
	static const float c0 = 0.8109119309638332633713423362694399653724431;
	static const float c1 = 0.4808354605142681877121661197951496120000040;
	static const float e = 2.71828182845904523536028747135266249775724709;
	const float sph = s + 0.5;
	const float lanczos_sum = c0 + c1/(s + 1.0);
	const float base = (sph + g)/e;
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

float4 gamma(const float4 s)
{
    //  Requires:   s is the standard parameter to the gamma function, and it
    //              should lie in the [0, 36] range.
    //  Returns:    Return approximate gamma function output with a maximum
    //              relative error of 0.000463.  See gamma_impl for details.
	return gamma_impl(s, float4(1.0)/s);
}

float3 gamma(const float3 s)
{
    //  Float3 version:
	return gamma_impl(s, float3(1.0)/s);
}

float2 gamma(const float2 s)
{
    //  Float2 version:
	return gamma_impl(s, float2(1.0)/s);
}

float gamma(const float s)
{
    //  Float version:
	return gamma_impl(s, 1.0/s);
}


////////////////  INCOMPLETE GAMMA FUNCTIONS (RESTRICTED INPUT)  ///////////////

//  Lower incomplete gamma function for small s and z (implementation):
float4 ligamma_small_z_impl(const float4 s, const float4 z, const float4 s_inv)
{
    //  Requires:   1.) s < ~0.5
    //              2.) z <= ~0.775075
    //              3.) s_inv = 1.0/s (precomputed for outside reuse)
    //  Returns:    A series representation for the lower incomplete gamma
    //              function for small s and small z (4 terms).
    //  The actual "rolled up" summation looks like:
	//      last_sign = 1.0; last_pow = 1.0; last_factorial = 1.0;
	//      sum = last_sign * last_pow / ((s + k) * last_factorial)
	//      for(int i = 0; i < 4; ++i)
	//      {
	//          last_sign *= -1.0; last_pow *= z; last_factorial *= i;
	//          sum += last_sign * last_pow / ((s + k) * last_factorial);
	//      }
	//  Unrolled, constant-unfolded and arranged for madds and parallelism:
	const float4 scale = pow(z, s);
	float4 sum = s_inv;  //  Summation iteration 0 result
	//  Summation iterations 1, 2, and 3:
	const float4 z_sq = z*z;
	const float4 denom1 = s + float4(1.0);
	const float4 denom2 = 2.0*s + float4(4.0);
	const float4 denom3 = 6.0*s + float4(18.0);
	//float4 denom4 = 24.0*s + float4(96.0);
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	//sum += z_sq * z_sq / denom4;
	//  Scale and return:
	return scale * sum;
}

float3 ligamma_small_z_impl(const float3 s, const float3 z, const float3 s_inv)
{
    //  Float3 version:
	const float3 scale = pow(z, s);
	float3 sum = s_inv;
	const float3 z_sq = z*z;
	const float3 denom1 = s + float3(1.0);
	const float3 denom2 = 2.0*s + float3(4.0);
	const float3 denom3 = 6.0*s + float3(18.0);
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	return scale * sum;
}

float2 ligamma_small_z_impl(const float2 s, const float2 z, const float2 s_inv)
{
    //  Float2 version:
	const float2 scale = pow(z, s);
	float2 sum = s_inv;
	const float2 z_sq = z*z;
	const float2 denom1 = s + float2(1.0);
	const float2 denom2 = 2.0*s + float2(4.0);
	const float2 denom3 = 6.0*s + float2(18.0);
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	return scale * sum;
}

float ligamma_small_z_impl(const float s, const float z, const float s_inv)
{
    //  Float version:
	const float scale = pow(z, s);
	float sum = s_inv;
	const float z_sq = z*z;
	const float denom1 = s + 1.0;
	const float denom2 = 2.0*s + 4.0;
	const float denom3 = 6.0*s + 18.0;
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	return scale * sum;
}

//  Upper incomplete gamma function for small s and large z (implementation):
float4 uigamma_large_z_impl(const float4 s, const float4 z)
{
    //  Requires:   1.) s < ~0.5
    //              2.) z > ~0.775075
    //  Returns:    Gauss's continued fraction representation for the upper
    //              incomplete gamma function (4 terms).
	//  The "rolled up" continued fraction looks like this.  The denominator
    //  is truncated, and it's calculated "from the bottom up:"
	//      denom = float4('inf');
	//      float4 one = float4(1.0);
	//      for(int i = 4; i > 0; --i)
	//      {
	//          denom = ((i * 2.0) - one) + z - s + (i * (s - i))/denom;
	//      }
	//  Unrolled and constant-unfolded for madds and parallelism:
	const float4 numerator = pow(z, s) * exp(-z);
	float4 denom = float4(7.0) + z - s;
	denom = float4(5.0) + z - s + (3.0*s - float4(9.0))/denom;
	denom = float4(3.0) + z - s + (2.0*s - float4(4.0))/denom;
	denom = float4(1.0) + z - s + (s - float4(1.0))/denom;
	return numerator / denom;
}

float3 uigamma_large_z_impl(const float3 s, const float3 z)
{
    //  Float3 version:
	const float3 numerator = pow(z, s) * exp(-z);
	float3 denom = float3(7.0) + z - s;
	denom = float3(5.0) + z - s + (3.0*s - float3(9.0))/denom;
	denom = float3(3.0) + z - s + (2.0*s - float3(4.0))/denom;
	denom = float3(1.0) + z - s + (s - float3(1.0))/denom;
	return numerator / denom;
}

float2 uigamma_large_z_impl(const float2 s, const float2 z)
{
    //  Float2 version:
	const float2 numerator = pow(z, s) * exp(-z);
	float2 denom = float2(7.0) + z - s;
	denom = float2(5.0) + z - s + (3.0*s - float2(9.0))/denom;
	denom = float2(3.0) + z - s + (2.0*s - float2(4.0))/denom;
	denom = float2(1.0) + z - s + (s - float2(1.0))/denom;
	return numerator / denom;
}

float uigamma_large_z_impl(const float s, const float z)
{
    //  Float version:
	const float numerator = pow(z, s) * exp(-z);
	float denom = 7.0 + z - s;
	denom = 5.0 + z - s + (3.0*s - 9.0)/denom;
	denom = 3.0 + z - s + (2.0*s - 4.0)/denom;
	denom = 1.0 + z - s + (s - 1.0)/denom;
	return numerator / denom;
}

//  Normalized lower incomplete gamma function for small s (implementation):
float4 normalized_ligamma_impl(const float4 s, const float4 z,
    const float4 s_inv, const float4 gamma_s_inv)
{
    //  Requires:   1.) s < ~0.5
    //              2.) s_inv = 1/s (precomputed for outside reuse)
    //              3.) gamma_s_inv = 1/gamma(s) (precomputed for outside reuse)
    //  Returns:    Approximate the normalized lower incomplete gamma function
    //              for s < 0.5.  Since we only care about s < 0.5, we only need
    //              to evaluate two branches (not four) based on z.  Each branch
    //              uses four terms, with a max relative error of ~0.00182.  The
    //              branch threshold and specifics were adapted for fewer terms
    //              from Gil/Segura/Temme's paper here:
    //                  http://oai.cwi.nl/oai/asset/20433/20433B.pdf
	//  Evaluate both branches: Real branches test slower even when available.
	static const float4 thresh = float4(0.775075);
	bool4 z_is_large;
	z_is_large.x = z.x > thresh.x;
	z_is_large.y = z.y > thresh.y;
	z_is_large.z = z.z > thresh.z;
	z_is_large.w = z.w > thresh.w;
	const float4 large_z = float4(1.0) - uigamma_large_z_impl(s, z) * gamma_s_inv;
	const float4 small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	//  Combine the results from both branches:
	bool4 inverse_z_is_large = not(z_is_large);
	return large_z * float4(z_is_large) + small_z * float4(inverse_z_is_large);
}

float3 normalized_ligamma_impl(const float3 s, const float3 z,
    const float3 s_inv, const float3 gamma_s_inv)
{
    //  Float3 version:
	static const float3 thresh = float3(0.775075);
	bool3 z_is_large;
	z_is_large.x = z.x > thresh.x;
	z_is_large.y = z.y > thresh.y;
	z_is_large.z = z.z > thresh.z;
	const float3 large_z = float3(1.0) - uigamma_large_z_impl(s, z) * gamma_s_inv;
	const float3 small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	bool3 inverse_z_is_large = not(z_is_large);
	return large_z * float3(z_is_large) + small_z * float3(inverse_z_is_large);
}

float2 normalized_ligamma_impl(const float2 s, const float2 z,
    const float2 s_inv, const float2 gamma_s_inv)
{
    //  Float2 version:
	static const float2 thresh = float2(0.775075);
	bool2 z_is_large;
	z_is_large.x = z.x > thresh.x;
	z_is_large.y = z.y > thresh.y;
	const float2 large_z = float2(1.0) - uigamma_large_z_impl(s, z) * gamma_s_inv;
	const float2 small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	bool2 inverse_z_is_large = not(z_is_large);
	return large_z * float2(z_is_large) + small_z * float2(inverse_z_is_large);
}

float normalized_ligamma_impl(const float s, const float z,
    const float s_inv, const float gamma_s_inv)
{
    //  Float version:
	static const float thresh = 0.775075;
	const bool z_is_large = z > thresh;
	const float large_z = 1.0 - uigamma_large_z_impl(s, z) * gamma_s_inv;
	const float small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	return large_z * float(z_is_large) + small_z * float(!z_is_large);
}

//  Normalized lower incomplete gamma function for small s:
float4 normalized_ligamma(const float4 s, const float4 z)
{
    //  Requires:   s < ~0.5
    //  Returns:    Approximate the normalized lower incomplete gamma function
    //              for s < 0.5.  See normalized_ligamma_impl() for details.
	const float4 s_inv = float4(1.0)/s;
	const float4 gamma_s_inv = float4(1.0)/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

float3 normalized_ligamma(const float3 s, const float3 z)
{
    //  Float3 version:
	const float3 s_inv = float3(1.0)/s;
	const float3 gamma_s_inv = float3(1.0)/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

float2 normalized_ligamma(const float2 s, const float2 z)
{
    //  Float2 version:
	const float2 s_inv = float2(1.0)/s;
	const float2 gamma_s_inv = float2(1.0)/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

float normalized_ligamma(const float s, const float z)
{
    //  Float version:
	const float s_inv = 1.0/s;
	const float gamma_s_inv = 1.0/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

#endif  //  SPECIAL_FUNCTIONS_H

////////////////////////////  END SPECIAL-FUNCTIONS  ///////////////////////////

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
//#define tex2D_linearize(C, D) decode_input(vec4(texture(C, D)))
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

////////////////////////////////  END INCLUDES  ////////////////////////////////

/////////////////////////////  SCANLINE FUNCTIONS  /////////////////////////////

inline float3 get_gaussian_sigma(const float3 color, const float sigma_range)
{
    //  Requires:   Globals:
    //              1.) beam_min_sigma and beam_max_sigma are global floats
    //                  containing the desired minimum and maximum beam standard
    //                  deviations, for dim and bright colors respectively.
    //              2.) beam_max_sigma must be > 0.0
    //              3.) beam_min_sigma must be in (0.0, beam_max_sigma]
    //              4.) beam_spot_power must be defined as a global float.
    //              Parameters:
    //              1.) color is the underlying source color along a scanline
    //              2.) sigma_range = beam_max_sigma - beam_min_sigma; we take
    //                  sigma_range as a parameter to avoid repeated computation
    //                  when beam_{min, max}_sigma are runtime shader parameters
    //  Optional:   Users may set beam_spot_shape_function to 1 to define the
    //              inner f(color) subfunction (see below) as:
    //                  f(color) = sqrt(1.0 - (color - 1.0)*(color - 1.0))
    //              Otherwise (technically, if beam_spot_shape_function < 0.5):
    //                  f(color) = pow(color, beam_spot_power)
    //  Returns:    The standard deviation of the Gaussian beam for "color:"
    //                  sigma = beam_min_sigma + sigma_range * f(color)
    //  Details/Discussion:
    //  The beam's spot shape vaguely resembles an aspect-corrected f() in the
    //  range [0, 1] (not quite, but it's related).  f(color) = color makes
    //  spots look like diamonds, and a spherical function or cube balances
    //  between variable width and a soft/realistic shape.   A beam_spot_power
    //  > 1.0 can produce an ugly spot shape and more initial clipping, but the
    //  final shape also differs based on the horizontal resampling filter and
    //  the phosphor bloom.  For instance, resampling horizontally in nonlinear
    //  light and/or with a sharp (e.g. Lanczos) filter will sharpen the spot
    //  shape, but a sixth root is still quite soft.  A power function (default
    //  1.0/3.0 beam_spot_power) is most flexible, but a fixed spherical curve
    //  has the highest variability without an awful spot shape.
    //
    //  beam_min_sigma affects scanline sharpness/aliasing in dim areas, and its
    //  difference from beam_max_sigma affects beam width variability.  It only
    //  affects clipping [for pure Gaussians] if beam_spot_power > 1.0 (which is
    //  a conservative estimate for a more complex constraint).
    //
    //  beam_max_sigma affects clipping and increasing scanline width/softness
    //  as color increases.  The wider this is, the more scanlines need to be
    //  evaluated to avoid distortion.  For a pure Gaussian, the max_beam_sigma
    //  at which the first unused scanline always has a weight < 1.0/255.0 is:
    //      num scanlines = 2, max_beam_sigma = 0.2089; distortions begin ~0.34
    //      num scanlines = 3, max_beam_sigma = 0.3879; distortions begin ~0.52
    //      num scanlines = 4, max_beam_sigma = 0.5723; distortions begin ~0.70
    //      num scanlines = 5, max_beam_sigma = 0.7591; distortions begin ~0.89
    //      num scanlines = 6, max_beam_sigma = 0.9483; distortions begin ~1.08
    //  Generalized Gaussians permit more leeway here as steepness increases.
    if(beam_spot_shape_function < 0.5)
    {
        //  Use a power function:
        return float3(beam_min_sigma) + sigma_range *
            pow(color, float3(beam_spot_power));
    }
    else
    {
        //  Use a spherical function:
        const float3 color_minus_1 = color - float3(1.0);
        return float3(beam_min_sigma) + sigma_range *
            sqrt(float3(1.0) - color_minus_1*color_minus_1);
    }
}

inline float3 get_generalized_gaussian_beta(const float3 color,
    const float shape_range)
{
    //  Requires:   Globals:
    //              1.) beam_min_shape and beam_max_shape are global floats
    //                  containing the desired min/max generalized Gaussian
    //                  beta parameters, for dim and bright colors respectively.
    //              2.) beam_max_shape must be >= 2.0
    //              3.) beam_min_shape must be in [2.0, beam_max_shape]
    //              4.) beam_shape_power must be defined as a global float.
    //              Parameters:
    //              1.) color is the underlying source color along a scanline
    //              2.) shape_range = beam_max_shape - beam_min_shape; we take
    //                  shape_range as a parameter to avoid repeated computation
    //                  when beam_{min, max}_shape are runtime shader parameters
    //  Returns:    The type-I generalized Gaussian "shape" parameter beta for
    //              the given color.
    //  Details/Discussion:
    //  Beta affects the scanline distribution as follows:
    //  a.) beta < 2.0 narrows the peak to a spike with a discontinuous slope
    //  b.) beta == 2.0 just degenerates to a Gaussian
    //  c.) beta > 2.0 flattens and widens the peak, then drops off more steeply
    //      than a Gaussian.  Whereas high sigmas widen and soften peaks, high
    //      beta widen and sharpen peaks at the risk of aliasing.
    //  Unlike high beam_spot_powers, high beam_shape_powers actually soften shape
    //  transitions, whereas lower ones sharpen them (at the risk of aliasing).
    return beam_min_shape + shape_range * pow(color, float3(beam_shape_power));
}

float3 scanline_gaussian_integral_contrib(const float3 dist,
    const float3 color, const float pixel_height, const float sigma_range)
{
    //  Requires:   1.) dist is the distance of the [potentially separate R/G/B]
    //                  point(s) from a scanline in units of scanlines, where
    //                  1.0 means the sample point straddles the next scanline.
    //              2.) color is the underlying source color along a scanline.
    //              3.) pixel_height is the output pixel height in scanlines.
    //              4.) Requirements of get_gaussian_sigma() must be met.
    //  Returns:    Return a scanline's light output over a given pixel.
    //  Details:
    //  The CRT beam profile follows a roughly Gaussian distribution which is
    //  wider for bright colors than dark ones.  The integral over the full
    //  range of a Gaussian function is always 1.0, so we can vary the beam
    //  with a standard deviation without affecting brightness.  'x' = distance:
    //      gaussian sample = 1/(sigma*sqrt(2*pi)) * e**(-(x**2)/(2*sigma**2))
    //      gaussian integral = 0.5 (1.0 + erf(x/(sigma * sqrt(2))))
    //  Use a numerical approximation of the "error function" (the Gaussian
    //  indefinite integral) to find the definite integral of the scanline's
    //  average brightness over a given pixel area.  Even if curved coords were
    //  used in this pass, a flat scalar pixel height works almost as well as a
    //  pixel height computed from a full pixel-space to scanline-space matrix.
    const float3 sigma = get_gaussian_sigma(color, sigma_range);
    const float3 ph_offset = float3(pixel_height * 0.5);
    const float3 denom_inv = 1.0/(sigma*sqrt(2.0));
    const float3 integral_high = erf((dist + ph_offset)*denom_inv);
    const float3 integral_low = erf((dist - ph_offset)*denom_inv);
    return color * 0.5*(integral_high - integral_low)/pixel_height;
}

float3 scanline_generalized_gaussian_integral_contrib(float3 dist,
    float3 color, float pixel_height, float sigma_range,
    float shape_range)
{
    //  Requires:   1.) Requirements of scanline_gaussian_integral_contrib()
    //                  must be met.
    //              2.) Requirements of get_gaussian_sigma() must be met.
    //              3.) Requirements of get_generalized_gaussian_beta() must be
    //                  met.
    //  Returns:    Return a scanline's light output over a given pixel.
    //  A generalized Gaussian distribution allows the shape (beta) to vary
    //  as well as the width (alpha).  "gamma" refers to the gamma function:
    //      generalized sample =
    //          beta/(2*alpha*gamma(1/beta)) * e**(-(|x|/alpha)**beta)
    //  ligamma(s, z) is the lower incomplete gamma function, for which we only
    //  implement two of four branches (because we keep 1/beta <= 0.5):
    //      generalized integral = 0.5 + 0.5* sign(x) *
    //          ligamma(1/beta, (|x|/alpha)**beta)/gamma(1/beta)
    //  See get_generalized_gaussian_beta() for a discussion of beta.
    //  We base alpha on the intended Gaussian sigma, but it only strictly
    //  models models standard deviation at beta == 2, because the standard
    //  deviation depends on both alpha and beta (keeping alpha independent is
    //  faster and preserves intuitive behavior and a full spectrum of results).
    const float3 alpha = sqrt(2.0) * get_gaussian_sigma(color, sigma_range);
    const float3 beta = get_generalized_gaussian_beta(color, shape_range);
    const float3 alpha_inv = float3(1.0)/alpha;
    const float3 s = float3(1.0)/beta;
    const float3 ph_offset = float3(pixel_height * 0.5);
    //  Pass beta to gamma_impl to avoid repeated divides.  Similarly pass
    //  beta (i.e. 1/s) and 1/gamma(s) to normalized_ligamma_impl.
    const float3 gamma_s_inv = float3(1.0)/gamma_impl(s, beta);
    const float3 dist1 = dist + ph_offset;
    const float3 dist0 = dist - ph_offset;
    const float3 integral_high = sign(dist1) * normalized_ligamma_impl(
        s, pow(abs(dist1)*alpha_inv, beta), beta, gamma_s_inv);
    const float3 integral_low = sign(dist0) * normalized_ligamma_impl(
        s, pow(abs(dist0)*alpha_inv, beta), beta, gamma_s_inv);
    return color * 0.5*(integral_high - integral_low)/pixel_height;
}

float3 scanline_gaussian_sampled_contrib(const float3 dist, const float3 color,
    const float pixel_height, const float sigma_range)
{
    //  See scanline_gaussian integral_contrib() for detailed comments!
    //  gaussian sample = 1/(sigma*sqrt(2*pi)) * e**(-(x**2)/(2*sigma**2))
    const float3 sigma = get_gaussian_sigma(color, sigma_range);
    //  Avoid repeated divides:
    const float3 sigma_inv = float3(1.0)/sigma;
    const float3 inner_denom_inv = 0.5 * sigma_inv * sigma_inv;
    const float3 outer_denom_inv = sigma_inv/sqrt(2.0*pi);
    if(beam_antialias_level > 0.5)
    {
        //  Sample 1/3 pixel away in each direction as well:
        const float3 sample_offset = float3(pixel_height/3.0);
        const float3 dist2 = dist + sample_offset;
        const float3 dist3 = abs(dist - sample_offset);
        //  Average three pure Gaussian samples:
        const float3 scale = color/3.0 * outer_denom_inv;
        const float3 weight1 = exp(-(dist*dist)*inner_denom_inv);
        const float3 weight2 = exp(-(dist2*dist2)*inner_denom_inv);
        const float3 weight3 = exp(-(dist3*dist3)*inner_denom_inv);
        return scale * (weight1 + weight2 + weight3);
    }
    else
    {
        return color*exp(-(dist*dist)*inner_denom_inv)*outer_denom_inv;
    }
}

float3 scanline_generalized_gaussian_sampled_contrib(float3 dist,
    float3 color, float pixel_height, float sigma_range,
    float shape_range)
{
    //  See scanline_generalized_gaussian_integral_contrib() for details!
    //  generalized sample =
    //      beta/(2*alpha*gamma(1/beta)) * e**(-(|x|/alpha)**beta)
    const float3 alpha = sqrt(2.0) * get_gaussian_sigma(color, sigma_range);
    const float3 beta = get_generalized_gaussian_beta(color, shape_range);
    //  Avoid repeated divides:
    const float3 alpha_inv = float3(1.0)/alpha;
    const float3 beta_inv = float3(1.0)/beta;
    const float3 scale = color * beta * 0.5 * alpha_inv /
        gamma_impl(beta_inv, beta);
    if(beam_antialias_level > 0.5)
    {
        //  Sample 1/3 pixel closer to and farther from the scanline too.
        const float3 sample_offset = float3(pixel_height/3.0);
        const float3 dist2 = dist + sample_offset;
        const float3 dist3 = abs(dist - sample_offset);
        //  Average three generalized Gaussian samples:
        const float3 weight1 = exp(-pow(abs(dist*alpha_inv), beta));
        const float3 weight2 = exp(-pow(abs(dist2*alpha_inv), beta));
        const float3 weight3 = exp(-pow(abs(dist3*alpha_inv), beta));
        return scale/3.0 * (weight1 + weight2 + weight3);
    }
    else
    {
        return scale * exp(-pow(abs(dist*alpha_inv), beta));
    }
}

inline float3 scanline_contrib(float3 dist, float3 color,
    float pixel_height, const float sigma_range, const float shape_range)
{
    //  Requires:   1.) Requirements of scanline_gaussian_integral_contrib()
    //                  must be met.
    //              2.) Requirements of get_gaussian_sigma() must be met.
    //              3.) Requirements of get_generalized_gaussian_beta() must be
    //                  met.
    //  Returns:    Return a scanline's light output over a given pixel, using
    //              a generalized or pure Gaussian distribution and sampling or
    //              integrals as desired by user codepath choices.
    if(beam_generalized_gaussian)
    {
        if(beam_antialias_level > 1.5)
        {
            return scanline_generalized_gaussian_integral_contrib(
                dist, color, pixel_height, sigma_range, shape_range);
        }
        else
        {
            return scanline_generalized_gaussian_sampled_contrib(
                dist, color, pixel_height, sigma_range, shape_range);
        }
    }
    else
    {
        if(beam_antialias_level > 1.5)
        {
            return scanline_gaussian_integral_contrib(
                dist, color, pixel_height, sigma_range);
        }
        else
        {
            return scanline_gaussian_sampled_contrib(
                dist, color, pixel_height, sigma_range);
        }
    }
}

inline float3 get_raw_interpolated_color(const float3 color0,
    const float3 color1, const float3 color2, const float3 color3,
    const float4 weights)
{
    //  Use max to avoid bizarre artifacts from negative colors:
    return max(mul(weights, float4x3(color0, color1, color2, color3)), 0.0);
}

float3 get_interpolated_linear_color(const float3 color0, const float3 color1,
    const float3 color2, const float3 color3, const float4 weights)
{
    //  Requires:   1.) Requirements of include/gamma-management.h must be met:
    //                  intermediate_gamma must be globally defined, and input
    //                  colors are interpreted as linear RGB unless you #define
    //                  GAMMA_ENCODE_EVERY_FBO (in which case they are
    //                  interpreted as gamma-encoded with intermediate_gamma).
    //              2.) color0-3 are colors sampled from a texture with tex2D().
    //                  They are interpreted as defined in requirement 1.
    //              3.) weights contains weights for each color, summing to 1.0.
    //              4.) beam_horiz_linear_rgb_weight must be defined as a global
    //                  float in [0.0, 1.0] describing how much blending should
    //                  be done in linear RGB (rest is gamma-corrected RGB).
    //              5.) RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE must be #defined
    //                  if beam_horiz_linear_rgb_weight is anything other than a
    //                  static constant, or we may try branching at runtime
    //                  without dynamic branches allowed (slow).
    //  Returns:    Return an interpolated color lookup between the four input
    //              colors based on the weights in weights.  The final color will
    //              be a linear RGB value, but the blending will be done as
    //              indicated above.
    const float intermediate_gamma = get_intermediate_gamma();
    //  Branch if beam_horiz_linear_rgb_weight is static (for free) or if the
    //  profile allows dynamic branches (faster than computing extra pows):
    #ifndef RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE
        #define SCANLINES_BRANCH_FOR_LINEAR_RGB_WEIGHT
    #else
        #ifdef DRIVERS_ALLOW_DYNAMIC_BRANCHES
            #define SCANLINES_BRANCH_FOR_LINEAR_RGB_WEIGHT
        #endif
    #endif
    #ifdef SCANLINES_BRANCH_FOR_LINEAR_RGB_WEIGHT
        //  beam_horiz_linear_rgb_weight is static, so we can branch:
        #ifdef GAMMA_ENCODE_EVERY_FBO
            const float3 gamma_mixed_color = pow(get_raw_interpolated_color(
                color0, color1, color2, color3, weights), float3(intermediate_gamma));
            if(beam_horiz_linear_rgb_weight > 0.0)
            {
                const float3 linear_mixed_color = get_raw_interpolated_color(
                    pow(color0, float3(intermediate_gamma)),
                    pow(color1, float3(intermediate_gamma)),
                    pow(color2, float3(intermediate_gamma)),
                    pow(color3, float3(intermediate_gamma)),
                    weights);
                return lerp(gamma_mixed_color, linear_mixed_color,
                    beam_horiz_linear_rgb_weight);
            }
            else
            {
                return gamma_mixed_color;
            }
        #else
            const float3 linear_mixed_color = get_raw_interpolated_color(
                color0, color1, color2, color3, weights);
            if(beam_horiz_linear_rgb_weight < 1.0)
            {
                const float3 gamma_mixed_color = get_raw_interpolated_color(
                    pow(color0, float3(1.0/intermediate_gamma)),
                    pow(color1, float3(1.0/intermediate_gamma)),
                    pow(color2, float3(1.0/intermediate_gamma)),
                    pow(color3, float3(1.0/intermediate_gamma)),
                    weights);
                return lerp(gamma_mixed_color, linear_mixed_color,
                    beam_horiz_linear_rgb_weight);
            }
            else
            {
                return linear_mixed_color;
            }
        #endif  //  GAMMA_ENCODE_EVERY_FBO
    #else
        #ifdef GAMMA_ENCODE_EVERY_FBO
            //  Inputs: color0-3 are colors in gamma-encoded RGB.
            const float3 gamma_mixed_color = pow(get_raw_interpolated_color(
                color0, color1, color2, color3, weights), intermediate_gamma);
            const float3 linear_mixed_color = get_raw_interpolated_color(
                pow(color0, float3(intermediate_gamma)),
                pow(color1, float3(intermediate_gamma)),
                pow(color2, float3(intermediate_gamma)),
                pow(color3, float3(intermediate_gamma)),
                weights);
            return lerp(gamma_mixed_color, linear_mixed_color,
                beam_horiz_linear_rgb_weight);
        #else
            //  Inputs: color0-3 are colors in linear RGB.
            const float3 linear_mixed_color = get_raw_interpolated_color(
                color0, color1, color2, color3, weights);
            const float3 gamma_mixed_color = get_raw_interpolated_color(
                    pow(color0, float3(1.0/intermediate_gamma)),
                    pow(color1, float3(1.0/intermediate_gamma)),
                    pow(color2, float3(1.0/intermediate_gamma)),
                    pow(color3, float3(1.0/intermediate_gamma)),
                    weights);
			// wtf fixme
//			const float beam_horiz_linear_rgb_weight1 = 1.0;
            return lerp(gamma_mixed_color, linear_mixed_color,
                beam_horiz_linear_rgb_weight);
        #endif  //  GAMMA_ENCODE_EVERY_FBO
    #endif  //  SCANLINES_BRANCH_FOR_LINEAR_RGB_WEIGHT
}

float3 get_scanline_color(const sampler2D tex, const float2 scanline_uv,
    const float2 uv_step_x, const float4 weights)
{
    //  Requires:   1.) scanline_uv must be vertically snapped to the caller's
    //                  desired line or scanline and horizontally snapped to the
    //                  texel just left of the output pixel (color1)
    //              2.) uv_step_x must contain the horizontal uv distance
    //                  between texels.
    //              3.) weights must contain interpolation filter weights for
    //                  color0, color1, color2, and color3, where color1 is just
    //                  left of the output pixel.
    //  Returns:    Return a horizontally interpolated texture lookup using 2-4
    //              nearby texels, according to weights and the conventions of
    //              get_interpolated_linear_color().
    //  We can ignore the outside texture lookups for Quilez resampling.
    const float3 color1 = COMPAT_TEXTURE(tex, scanline_uv).rgb;
    const float3 color2 = COMPAT_TEXTURE(tex, scanline_uv + uv_step_x).rgb;
    float3 color0 = float3(0.0);
    float3 color3 = float3(0.0);
    if(beam_horiz_filter > 0.5)
    {
        color0 = COMPAT_TEXTURE(tex, scanline_uv - uv_step_x).rgb;
        color3 = COMPAT_TEXTURE(tex, scanline_uv + 2.0 * uv_step_x).rgb;
    }
    //  Sample the texture as-is, whether it's linear or gamma-encoded:
    //  get_interpolated_linear_color() will handle the difference.
    return get_interpolated_linear_color(color0, color1, color2, color3, weights);
}

float3 sample_single_scanline_horizontal(const sampler2D tex,
    const float2 tex_uv, const float2 tex_size,
    const float2 texture_size_inv)
{
    //  TODO: Add function requirements.
    //  Snap to the previous texel and get sample dists from 2/4 nearby texels:
    const float2 curr_texel = tex_uv * tex_size;
    //  Use under_half to fix a rounding bug right around exact texel locations.
    const float2 prev_texel =
        floor(curr_texel - float2(under_half)) + float2(0.5);
    const float2 prev_texel_hor = float2(prev_texel.x, curr_texel.y);
    const float2 prev_texel_hor_uv = prev_texel_hor * texture_size_inv;
    const float prev_dist = curr_texel.x - prev_texel_hor.x;
    const float4 sample_dists = float4(1.0 + prev_dist, prev_dist,
        1.0 - prev_dist, 2.0 - prev_dist);
    //  Get Quilez, Lanczos2, or Gaussian resize weights for 2/4 nearby texels:
    float4 weights;
    if(beam_horiz_filter < 0.5)
    {
        //  Quilez:
        const float x = sample_dists.y;
        const float w2 = x*x*x*(x*(x*6.0 - 15.0) + 10.0);
        weights = float4(0.0, 1.0 - w2, w2, 0.0);
    }
    else if(beam_horiz_filter < 1.5)
    {
        //  Gaussian:
        float inner_denom_inv = 1.0/(2.0*beam_horiz_sigma*beam_horiz_sigma);
        weights = exp(-(sample_dists*sample_dists)*inner_denom_inv);
    }
    else
    {
        //  Lanczos2:
        const float4 pi_dists = FIX_ZERO(sample_dists * pi);
        weights = 2.0 * sin(pi_dists) * sin(pi_dists * 0.5) /
            (pi_dists * pi_dists);
    }
    //  Ensure the weight sum == 1.0:
    const float4 final_weights = weights/dot(weights, float4(1.0));
    //  Get the interpolated horizontal scanline color:
    const float2 uv_step_x = float2(texture_size_inv.x, 0.0);
    return get_scanline_color(
        tex, prev_texel_hor_uv, uv_step_x, final_weights);
}

float3 sample_rgb_scanline_horizontal(const sampler2D tex,
    const float2 tex_uv, const float2 tex_size,
    const float2 texture_size_inv)
{
    //  TODO: Add function requirements.
    //  Rely on a helper to make convergence easier.
    if(beam_misconvergence)
    {
        const float3 convergence_offsets_rgb =
            get_convergence_offsets_x_vector();
        const float3 offset_u_rgb =
            convergence_offsets_rgb * texture_size_inv.xxx;
        const float2 scanline_uv_r = tex_uv - float2(offset_u_rgb.r, 0.0);
        const float2 scanline_uv_g = tex_uv - float2(offset_u_rgb.g, 0.0);
        const float2 scanline_uv_b = tex_uv - float2(offset_u_rgb.b, 0.0);
        const float3 sample_r = sample_single_scanline_horizontal(
            tex, scanline_uv_r, tex_size, texture_size_inv);
        const float3 sample_g = sample_single_scanline_horizontal(
            tex, scanline_uv_g, tex_size, texture_size_inv);
        const float3 sample_b = sample_single_scanline_horizontal(
            tex, scanline_uv_b, tex_size, texture_size_inv);
        return float3(sample_r.r, sample_g.g, sample_b.b);
    }
    else
    {
        return sample_single_scanline_horizontal(tex, tex_uv, tex_size,
            texture_size_inv);
    }
}

float2 get_last_scanline_uv(const float2 tex_uv, const float2 tex_size,
    const float2 texture_size_inv, const float2 il_step_multiple,
    const float frame_count, out float dist)
{
    //  Compute texture coords for the last/upper scanline, accounting for
    //  interlacing: With interlacing, only consider even/odd scanlines every
    //  other frame.  Top-field first (TFF) order puts even scanlines on even
    //  frames, and BFF order puts them on odd frames.  Texels are centered at:
    //      frac(tex_uv * tex_size) == x.5
    //  Caution: If these coordinates ever seem incorrect, first make sure it's
    //  not because anisotropic filtering is blurring across field boundaries.
    //  Note: TFF/BFF won't matter for sources that double-weave or similar.
	// wtf fixme
//	const float interlace_bff1 = 1.0;
    const float field_offset = floor(il_step_multiple.y * 0.75) *
        fmod(frame_count + float(interlace_bff), 2.0);
    const float2 curr_texel = tex_uv * tex_size;
    //  Use under_half to fix a rounding bug right around exact texel locations.
    const float2 prev_texel_num = floor(curr_texel - float2(under_half));
    const float wrong_field = fmod(
        prev_texel_num.y + field_offset, il_step_multiple.y);
    const float2 scanline_texel_num = prev_texel_num - float2(0.0, wrong_field);
    //  Snap to the center of the previous scanline in the current field:
    const float2 scanline_texel = scanline_texel_num + float2(0.5);
    const float2 scanline_uv = scanline_texel * texture_size_inv;
    //  Save the sample's distance from the scanline, in units of scanlines:
    dist = (curr_texel.y - scanline_texel.y)/il_step_multiple.y;
    return scanline_uv;
}

inline bool is_interlaced(float num_lines)
{
    //  Detect interlacing based on the number of lines in the source.
    if(interlace_detect)
    {
        //  NTSC: 525 lines, 262.5/field; 486 active (2 half-lines), 243/field
        //  NTSC Emulators: Typically 224 or 240 lines
        //  PAL: 625 lines, 312.5/field; 576 active (typical), 288/field
        //  PAL Emulators: ?
        //  ATSC: 720p, 1080i, 1080p
        //  Where do we place our cutoffs?  Assumptions:
        //  1.) We only need to care about active lines.
        //  2.) Anything > 288 and <= 576 lines is probably interlaced.
        //  3.) Anything > 576 lines is probably not interlaced...
        //  4.) ...except 1080 lines, which is a crapshoot (user decision).
        //  5.) Just in case the main program uses calculated video sizes,
        //      we should nudge the float thresholds a bit.
        const bool sd_interlace = ((num_lines > 288.5) && (num_lines < 576.5));
        const bool hd_interlace = bool(interlace_1080i) ?
            ((num_lines > 1079.5) && (num_lines < 1080.5)) :
            false;
        return (sd_interlace || hd_interlace);
    }
    else
    {
        return false;
    }
}

#endif  //  SCANLINE_FUNCTIONS_H

/////////////////////////////  END SCANLINE-FUNCTIONS  ////////////////////////////

///////////////////////////////  END VERTEX INCLUDES  /////////////////////////////

#undef COMPAT_PRECISION
#undef COMPAT_TEXTURE

float bloom_approx_scale_x = targetSize.x / sourceSize[0].y;
const float max_viewport_size_x = 1080.0*1024.0*(4.0/3.0);

void main() {
   gl_Position = position;
   vTexCoord = texCoord;
	const float2 video_uv = vTexCoord * texture_size/video_size;
    tex_uv = video_uv * ORIG_LINEARIZEDvideo_size /
        ORIG_LINEARIZEDtexture_size;
    //  The last pass (vertical scanlines) had a viewport y scale, so we can
    //  use it to calculate a better runtime sigma:
    estimated_viewport_size_x =
        video_size.y * geom_aspect_ratio_x/geom_aspect_ratio_y;

    //  Get the uv sample distance between output pixels.  We're using a resize
    //  blur, so arbitrary upsizing will be acceptable if filter_linearN =
    //  "true," and arbitrary downsizing will be acceptable if mipmap_inputN =
    //  "true" too.  The blur will be much more accurate if a true 4x4 Gaussian
    //  resize is used instead of tex2Dblur3x3_resize (which samples between
    //  texels even for upsizing).
    const float2 dxdy_min_scale = ORIG_LINEARIZEDvideo_size/output_size;
    const float2 texture_size_inv = float2(1.0, 1.0)/ORIG_LINEARIZEDtexture_size;
    if(bloom_approx_filter > 1.5)   //  4x4 true Gaussian resize
    {
        //  For upsizing, we'll snap to texels and sample the nearest 4.
        const float2 dxdy_scale = max(dxdy_min_scale, float2(1.0, 1.0));
        blur_dxdy = dxdy_scale * texture_size_inv;
    }
    else
    {
        const float2 dxdy_scale = dxdy_min_scale;
        blur_dxdy = dxdy_scale * texture_size_inv;
    }
    //  tex2Dresize_gaussian4x4 needs to know a bit more than the other filters:
    tex_uv_to_pixel_scale = output_size *
        ORIG_LINEARIZEDtexture_size / ORIG_LINEARIZEDvideo_size;
    //texture_size_inv = texture_size_inv;

    //  Detecting interlacing again here lets us apply convergence offsets in
    //  this pass.  il_step_multiple contains the (texel, scanline) step
    //  multiple: 1 for progressive, 2 for interlaced.
    const float2 orig_video_size = ORIG_LINEARIZEDvideo_size;
    const float y_step = 1.0 + float(is_interlaced(orig_video_size.y));
    const float2 il_step_multiple = float2(1.0, y_step);
    //  Get the uv distance between (texels, same-field scanlines):
    uv_scanline_step = il_step_multiple / ORIG_LINEARIZEDtexture_size;
}