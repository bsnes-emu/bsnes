#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform sampler2D pixmap[];
uniform int phase;

in Vertex {
   vec2 vTexCoord;
   vec2 src_tex_uv_wrap;
   vec2 resize_magnification_scale;
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

#define mask_grille_texture_small pixmap[0]
#define mask_slot_texture_small pixmap[2]
#define mask_shadow_texture_small pixmap[4]

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

//////////////////////////////////  INCLUDES  //////////////////////////////////

//#include "phosphor-mask-resizing.h"

////////////////////////  BEGIN PHOSPHOR-MASK-RESIZING  ////////////////////////

#ifndef PHOSPHOR_MASK_RESIZING_H
#define PHOSPHOR_MASK_RESIZING_H

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

//#include "../user-settings.h"
//#include "derived-settings-and-constants.h"

/////////////////////////////  CODEPATH SELECTION  /////////////////////////////

//  Choose a looping strategy based on what's allowed:
//  Dynamic loops not allowed: Use a flat static loop.
//  Dynamic loops accomodated: Coarsely branch around static loops.
//  Dynamic loops assumed allowed: Use a flat dynamic loop.
#ifndef DRIVERS_ALLOW_DYNAMIC_BRANCHES
    #ifdef ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
        #define BREAK_LOOPS_INTO_PIECES
    #else
        #define USE_SINGLE_STATIC_LOOP
    #endif
#endif  //  No else needed: Dynamic loops assumed.


//////////////////////////////////  CONSTANTS  /////////////////////////////////

//  The larger the resized tile, the fewer samples we'll need for downsizing.
//  See if we can get a static min tile size > mask_min_allowed_tile_size:
static const float mask_min_allowed_tile_size = ceil(
    mask_min_allowed_triad_size * mask_triads_per_tile);
static const float mask_min_expected_tile_size = 
        mask_min_allowed_tile_size;
//  Limit the number of sinc resize taps by the maximum minification factor:
static const float pi_over_lobes = pi/mask_sinc_lobes;
static const float max_sinc_resize_samples_float = 2.0 * mask_sinc_lobes *
    mask_resize_src_lut_size.x/mask_min_expected_tile_size;
//  Vectorized loops sample in multiples of 4.  Round up to be safe:
static const float max_sinc_resize_samples_m4 = ceil(
    max_sinc_resize_samples_float * 0.25) * 4.0;


/////////////////////////  RESAMPLING FUNCTION HELPERS  ////////////////////////

inline float get_dynamic_loop_size(const float magnification_scale)
{
    //  Requires:   The following global constants must be defined:
    //              1.) mask_sinc_lobes
    //              2.) max_sinc_resize_samples_m4
    //  Returns:    The minimum number of texture samples for a correct downsize
    //              at magnification_scale.
    //  We're downsizing, so the filter is sized across 2*lobes output pixels
    //  (not 2*lobes input texels).  This impacts distance measurements and the
    //  minimum number of input samples needed.
    const float min_samples_float = 2.0 * mask_sinc_lobes / magnification_scale;
    const float min_samples_m4 = ceil(min_samples_float * 0.25) * 4.0;
    #ifdef DRIVERS_ALLOW_DYNAMIC_BRANCHES
        const float max_samples_m4 = max_sinc_resize_samples_m4;
    #else   // ifdef BREAK_LOOPS_INTO_PIECES
        //  Simulating loops with branches imposes a 128-sample limit.
        const float max_samples_m4 = min(128.0, max_sinc_resize_samples_m4);
    #endif
    return min(min_samples_m4, max_samples_m4);
}

float2 get_first_texel_tile_uv_and_dist(const float2 tex_uv, 
    const float2 tex_size, const float dr, 
    const float input_tiles_per_texture_r, const float samples,
    static const bool vertical)
{
    //  Requires:   1.) dr == du == 1.0/texture_size.x or
    //                  dr == dv == 1.0/texture_size.y
    //                  (whichever direction we're resampling in).
    //                  It's a scalar to save register space.
    //              2.) input_tiles_per_texture_r is the number of input tiles
    //                  that can fit in the input texture in the direction we're
    //                  resampling this pass.
    //              3.) vertical indicates whether we're resampling vertically
    //                  this pass (or horizontally).
    //  Returns:    Pack and return the first sample's tile_uv coord in [0, 1]
    //              and its texel distance from the destination pixel, in the
    //              resized dimension only.
    //  We'll start with the topmost or leftmost sample and work down or right,
    //  so get the first sample location and distance.  Modify both dimensions
    //  as if we're doing a one-pass 2D resize; we'll throw away the unneeded
    //  (and incorrect) dimension at the end.
    const float2 curr_texel = tex_uv * tex_size;
    const float2 prev_texel =
        floor(curr_texel - float2(under_half)) + float2(0.5);
    const float2 first_texel = prev_texel - float2(samples/2.0 - 1.0);
    const float2 first_texel_uv_wrap_2D = first_texel * dr;
    const float2 first_texel_dist_2D = curr_texel - first_texel;
    //  Convert from tex_uv to tile_uv coords so we can sub fracs for fmods.
    const float2 first_texel_tile_uv_wrap_2D =
        first_texel_uv_wrap_2D * input_tiles_per_texture_r;
    //  Project wrapped coordinates to the [0, 1] range.  We'll do this with all
    //  samples,but the first texel is special, since it might be negative.
    const float2 coord_negative =
        float2((first_texel_tile_uv_wrap_2D.x < 0.),(first_texel_tile_uv_wrap_2D.y < 0.));
    const float2 first_texel_tile_uv_2D =
        frac(first_texel_tile_uv_wrap_2D) + coord_negative;
    //  Pack the first texel's tile_uv coord and texel distance in 1D:
    const float2 tile_u_and_dist =
        float2(first_texel_tile_uv_2D.x, first_texel_dist_2D.x);
    const float2 tile_v_and_dist =
        float2(first_texel_tile_uv_2D.y, first_texel_dist_2D.y);
    return vertical ? tile_v_and_dist : tile_u_and_dist;
    //return lerp(tile_u_and_dist, tile_v_and_dist, float(vertical));
}

inline float4 tex2Dlod0try(const sampler2D tex, const float2 tex_uv)
{
    //  Mipmapping and anisotropic filtering get confused by sinc-resampling.
    //  One [slow] workaround is to select the lowest mip level:
    #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
        return textureLod(tex, float4(tex_uv, 0.0, 0.0).xy);
    #else
        #ifdef ANISOTROPIC_RESAMPLING_COMPAT_TEX2DBIAS
            return tex2Dbias(tex, float4(tex_uv, 0.0, -16.0));
        #else
            return texture(tex, tex_uv);
        #endif
    #endif
}


//////////////////////////////  LOOP BODY MACROS  //////////////////////////////

//  Using inline functions can exceed the temporary register limit, so we're
//  stuck with #define macros (I'm TRULY sorry).  They're declared here instead
//  of above to be closer to the actual invocation sites.  Steps:
//  1.) Get the exact texel location.
//  2.) Sample the phosphor mask (already assumed encoded in linear RGB).
//  3.) Get the distance from the current pixel and sinc weight:
//          sinc(dist) = sin(pi * dist)/(pi * dist)
//      We can also use the slower/smoother Lanczos instead:
//          L(x) = sinc(dist) * sinc(dist / lobes)
//  4.) Accumulate the weight sum in weights, and accumulate the weighted texels
//      in pixel_color (we'll normalize outside the loop at the end).
//  We vectorize the loop to help reduce the Lanczos window's cost.

    //  The r coord is the coord in the dimension we're resizing along (u or v),
    //  and first_texel_tile_uv_rrrr is a float4 of the first texel's u or v
    //  tile_uv coord in [0, 1].  tex_uv_r will contain the tile_uv u or v coord
    //  for four new texel samples.
    #define CALCULATE_R_COORD_FOR_4_SAMPLES                                    \
        const float4 true_i = float4(i_base + i) + float4(0.0, 1.0, 2.0, 3.0); \
        const float4 tile_uv_r = frac(                                         \
            first_texel_tile_uv_rrrr + true_i * tile_dr);                      \
        const float4 tex_uv_r = tile_uv_r * tile_size_uv_r;

    #ifdef PHOSPHOR_MASK_RESIZE_LANCZOS_WINDOW
        #define CALCULATE_SINC_RESAMPLE_WEIGHTS                                \
            const float4 pi_dist_over_lobes = pi_over_lobes * dist;            \
            const float4 weights = min(sin(pi_dist) * sin(pi_dist_over_lobes) /\
                (pi_dist*pi_dist_over_lobes), float4(1.0));
    #else
        #define CALCULATE_SINC_RESAMPLE_WEIGHTS                                \
            const float4 weights = min(sin(pi_dist)/pi_dist, float4(1.0));
    #endif

    #define UPDATE_COLOR_AND_WEIGHT_SUMS                                       \
        const float4 dist = magnification_scale *                              \
            abs(first_dist_unscaled - true_i);                                 \
        const float4 pi_dist = pi * dist;                                      \
        CALCULATE_SINC_RESAMPLE_WEIGHTS;                                       \
        pixel_color += new_sample0 * weights.xxx;                              \
        pixel_color += new_sample1 * weights.yyy;                              \
        pixel_color += new_sample2 * weights.zzz;                              \
        pixel_color += new_sample3 * weights.www;                              \
        weight_sum += weights;

    #define VERTICAL_SINC_RESAMPLE_LOOP_BODY                                   \
        CALCULATE_R_COORD_FOR_4_SAMPLES;                                       \
        const float3 new_sample0 = tex2Dlod0try(tex,                       \
            float2(tex_uv.x, tex_uv_r.x)).rgb;                                 \
        const float3 new_sample1 = tex2Dlod0try(tex,                       \
            float2(tex_uv.x, tex_uv_r.y)).rgb;                                 \
        const float3 new_sample2 = tex2Dlod0try(tex,                       \
            float2(tex_uv.x, tex_uv_r.z)).rgb;                                 \
        const float3 new_sample3 = tex2Dlod0try(tex,                       \
            float2(tex_uv.x, tex_uv_r.w)).rgb;                                 \
        UPDATE_COLOR_AND_WEIGHT_SUMS;

    #define HORIZONTAL_SINC_RESAMPLE_LOOP_BODY                                 \
        CALCULATE_R_COORD_FOR_4_SAMPLES;                                       \
        const float3 new_sample0 = tex2Dlod0try(tex,                       \
            float2(tex_uv_r.x, tex_uv.y)).rgb;                                 \
        const float3 new_sample1 = tex2Dlod0try(tex,                       \
            float2(tex_uv_r.y, tex_uv.y)).rgb;                                 \
        const float3 new_sample2 = tex2Dlod0try(tex,                       \
            float2(tex_uv_r.z, tex_uv.y)).rgb;                                 \
        const float3 new_sample3 = tex2Dlod0try(tex,                       \
            float2(tex_uv_r.w, tex_uv.y)).rgb;                                 \
        UPDATE_COLOR_AND_WEIGHT_SUMS;


////////////////////////////  RESAMPLING FUNCTIONS  ////////////////////////////

float3 downsample_vertical_sinc_tiled(const sampler2D tex,
    const float2 tex_uv, const float2 tex_size, static const float dr,
    const float magnification_scale, static const float tile_size_uv_r)
{
    //  Requires:   1.) dr == du == 1.0/texture_size.x or
    //                  dr == dv == 1.0/texture_size.y
    //                  (whichever direction we're resampling in).
    //                  It's a scalar to save register space.
    //              2.) tile_size_uv_r is the number of texels an input tile
    //                  takes up in the input texture, in the direction we're
    //                  resampling this pass.
    //              3.) magnification_scale must be <= 1.0.
    //  Returns:    Return a [Lanczos] sinc-resampled pixel of a vertically
    //              downsized input tile embedded in an input texture.  (The
    //              vertical version is special-cased though: It assumes the
    //              tile size equals the [static] texture size, since it's used
    //              on an LUT texture input containing one tile.  For more
    //              generic use, eliminate the "static" in the parameters.)
    //  The "r" in "dr," "tile_size_uv_r," etc. refers to the dimension
    //  we're resizing along, e.g. "dy" in this case.
    #ifdef USE_SINGLE_STATIC_LOOP
        //  A static loop can be faster, but it might blur too much from using
        //  more samples than it should.
        static const int samples = int(max_sinc_resize_samples_m4);
    #else
        const int samples = int(get_dynamic_loop_size(magnification_scale));
    #endif

    //  Get the first sample location (scalar tile uv coord along the resized
    //  dimension) and distance from the output location (in texels):
    static const float input_tiles_per_texture_r = 1.0/tile_size_uv_r;
    //  true = vertical resize:
    const float2 first_texel_tile_r_and_dist = get_first_texel_tile_uv_and_dist(
        tex_uv, tex_size, dr, input_tiles_per_texture_r, samples, true);
    const float4 first_texel_tile_uv_rrrr = first_texel_tile_r_and_dist.xxxx;
    const float4 first_dist_unscaled = first_texel_tile_r_and_dist.yyyy;
    //  Get the tile sample offset:
    static const float tile_dr = dr * input_tiles_per_texture_r;

    //  Sum up each weight and weighted sample color, varying the looping
    //  strategy based on our expected dynamic loop capabilities.  See the
    //  loop body macros above.
    int i_base = 0;
    float4 weight_sum = float4(0.0);
    float3 pixel_color = float3(0.0);
    static const int i_step = 4;
    #ifdef BREAK_LOOPS_INTO_PIECES
        if(samples - i_base >= 64)
        {
            for(int i = 0; i < 64; i += i_step)
            {
                VERTICAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 64;
        }
        if(samples - i_base >= 32)
        {
            for(int i = 0; i < 32; i += i_step)
            {
                VERTICAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 32;
        }
        if(samples - i_base >= 16)
        {
            for(int i = 0; i < 16; i += i_step)
            {
                VERTICAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 16;
        }
        if(samples - i_base >= 8)
        {
            for(int i = 0; i < 8; i += i_step)
            {
                VERTICAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 8;
        }
        if(samples - i_base >= 4)
        {
            for(int i = 0; i < 4; i += i_step)
            {
                VERTICAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 4;
        }
        //  Do another 4-sample block for a total of 128 max samples.
        if(samples - i_base > 0)
        {
            for(int i = 0; i < 4; i += i_step)
            {
                VERTICAL_SINC_RESAMPLE_LOOP_BODY;
            }
        }
    #else
        for(int i = 0; i < samples; i += i_step)
        {
            VERTICAL_SINC_RESAMPLE_LOOP_BODY;
        }
    #endif
    //  Normalize so the weight_sum == 1.0, and return:
    const float2 weight_sum_reduce = weight_sum.xy + weight_sum.zw;
    const float3 scalar_weight_sum = float3(weight_sum_reduce.x + 
        weight_sum_reduce.y);
    return (pixel_color/scalar_weight_sum);
}

float3 downsample_horizontal_sinc_tiled(const sampler2D tex,
    const float2 tex_uv, const float2 tex_size, const float dr,
    const float magnification_scale, const float tile_size_uv_r)
{
    //  Differences from downsample_horizontal_sinc_tiled:
    //  1.) The dr and tile_size_uv_r parameters are not static consts.
    //  2.) The "vertical" parameter to get_first_texel_tile_uv_and_dist is
    //      set to false instead of true.
    //  3.) The horizontal version of the loop body is used.
    //  TODO: If we can get guaranteed compile-time dead code elimination,
    //  we can combine the vertical/horizontal downsampling functions by:
    //  1.) Add an extra static const bool parameter called "vertical."
    //  2.) Supply it with the result of get_first_texel_tile_uv_and_dist().
    //  3.) Use a conditional assignment in the loop body macro.  This is the
    //      tricky part: We DO NOT want to incur the extra conditional
    //      assignment in the inner loop at runtime!
    //  The "r" in "dr," "tile_size_uv_r," etc. refers to the dimension
    //  we're resizing along, e.g. "dx" in this case.
    #ifdef USE_SINGLE_STATIC_LOOP
        //  If we have to load all samples, we might as well use them.
        static const int samples = int(max_sinc_resize_samples_m4);
    #else
        const int samples = int(get_dynamic_loop_size(magnification_scale));
    #endif

    //  Get the first sample location (scalar tile uv coord along resized
    //  dimension) and distance from the output location (in texels):
    const float input_tiles_per_texture_r = 1.0/tile_size_uv_r;
    //  false = horizontal resize:
    const float2 first_texel_tile_r_and_dist = get_first_texel_tile_uv_and_dist(
        tex_uv, tex_size, dr, input_tiles_per_texture_r, samples, false);
    const float4 first_texel_tile_uv_rrrr = first_texel_tile_r_and_dist.xxxx;
    const float4 first_dist_unscaled = first_texel_tile_r_and_dist.yyyy;
    //  Get the tile sample offset:
    const float tile_dr = dr * input_tiles_per_texture_r;

    //  Sum up each weight and weighted sample color, varying the looping
    //  strategy based on our expected dynamic loop capabilities.  See the
    //  loop body macros above.
    int i_base = 0;
    float4 weight_sum = float4(0.0);
    float3 pixel_color = float3(0.0);
    static const int i_step = 4;
    #ifdef BREAK_LOOPS_INTO_PIECES
        if(samples - i_base >= 64)
        {
            for(int i = 0; i < 64; i += i_step)
            {
                HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 64;
        }
        if(samples - i_base >= 32)
        {
            for(int i = 0; i < 32; i += i_step)
            {
                HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 32;
        }
        if(samples - i_base >= 16)
        {
            for(int i = 0; i < 16; i += i_step)
            {
                HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 16;
        }
        if(samples - i_base >= 8)
        {
            for(int i = 0; i < 8; i += i_step)
            {
                HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 8;
        }
        if(samples - i_base >= 4)
        {
            for(int i = 0; i < 4; i += i_step)
            {
                HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
            }
            i_base += 4;
        }
        //  Do another 4-sample block for a total of 128 max samples.
        if(samples - i_base > 0)
        {
            for(int i = 0; i < 4; i += i_step)
            {
                HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
            }
        }
    #else
        for(int i = 0; i < samples; i += i_step)
        {
            HORIZONTAL_SINC_RESAMPLE_LOOP_BODY;
        }
    #endif
    //  Normalize so the weight_sum == 1.0, and return:
    const float2 weight_sum_reduce = weight_sum.xy + weight_sum.zw;
    const float3 scalar_weight_sum = float3(weight_sum_reduce.x +
        weight_sum_reduce.y);
    return (pixel_color/scalar_weight_sum);
}


////////////////////////////  TILE SIZE CALCULATION  ///////////////////////////

float2 get_resized_mask_tile_size(const float2 estimated_viewport_size,
    const float2 estimated_mask_resize_output_size,
    const bool solemnly_swear_same_inputs_for_every_pass)
{
    //  Requires:   The following global constants must be defined according to
    //              certain constraints:
    //              1.) mask_resize_num_triads: Must be high enough that our
    //                  mask sampling method won't have artifacts later
    //                  (long story; see derived-settings-and-constants.h)
    //              2.) mask_resize_src_lut_size: Texel size of our mask LUT
    //              3.) mask_triads_per_tile: Num horizontal triads in our LUT
    //              4.) mask_min_allowed_triad_size: User setting (the more
    //                  restrictive it is, the faster the resize will go)
    //              5.) mask_min_allowed_tile_size_x < mask_resize_src_lut_size.x
    //              6.) mask_triad_size_desired_{runtime, static}
    //              7.) mask_num_triads_desired_{runtime, static}
    //              8.) mask_specify_num_triads must be 0.0/1.0 (false/true)
    //              The function parameters must be defined as follows:
    //              1.) estimated_viewport_size == (final viewport size);
    //                  If mask_specify_num_triads is 1.0/true and the viewport
    //                  estimate is wrong, the number of triads will differ from
    //                  the user's preference by about the same factor.
    //              2.) estimated_mask_resize_output_size: Must equal the
    //                  output size of the MASK_RESIZE pass.
    //                  Exception: The x component may be estimated garbage if
    //                  and only if the caller throws away the x result.
    //              3.) solemnly_swear_same_inputs_for_every_pass: Set to false,
    //                  unless you can guarantee that every call across every
    //                  pass will use the same sizes for the other parameters.
    //              When calling this across multiple passes, always use the
    //              same y viewport size/scale, and always use the same x
    //              viewport size/scale when using the x result.
    //  Returns:    Return the final size of a manually resized mask tile, after
    //              constraining the desired size to avoid artifacts.  Under
    //              unusual circumstances, tiles may become stretched vertically
    //              (see wall of text below).
    //  Stated tile properties must be correct:
    static const float tile_aspect_ratio_inv =
        mask_resize_src_lut_size.y/mask_resize_src_lut_size.x;
    static const float tile_aspect_ratio = 1.0/tile_aspect_ratio_inv;
    static const float2 tile_aspect = float2(1.0, tile_aspect_ratio_inv);
    //  If mask_specify_num_triads is 1.0/true and estimated_viewport_size.x is
    //  wrong, the user preference will be misinterpreted:
    const float desired_tile_size_x = mask_triads_per_tile * lerp(
        mask_triad_size_desired,
        estimated_viewport_size.x / mask_num_triads_desired,
        mask_specify_num_triads);
    if(get_mask_sample_mode() > 0.5)
    {
        //  We don't need constraints unless we're sampling MASK_RESIZE.
        return desired_tile_size_x * tile_aspect;
    }
    //  Make sure we're not upsizing:
    const float temp_tile_size_x =
        min(desired_tile_size_x, mask_resize_src_lut_size.x);
    //  Enforce min_tile_size and max_tile_size in both dimensions:
    const float2 temp_tile_size = temp_tile_size_x * tile_aspect;
    static const float2 min_tile_size =
        mask_min_allowed_tile_size * tile_aspect;
    const float2 max_tile_size =
        estimated_mask_resize_output_size / mask_resize_num_tiles;
    const float2 clamped_tile_size =
        clamp(temp_tile_size, min_tile_size, max_tile_size);
    //  Try to maintain tile_aspect_ratio.  This is the tricky part:
    //  If we're currently resizing in the y dimension, the x components
    //  could be MEANINGLESS.  (If estimated_mask_resize_output_size.x is
    //  bogus, then so is max_tile_size.x and clamped_tile_size.x.)
    //  We can't adjust the y size based on clamped_tile_size.x.  If it
    //  clamps when it shouldn't, it won't clamp again when later passes
    //  call this function with the correct sizes, and the discrepancy will
    //  break the sampling coords in MASKED_SCANLINES.  Instead, we'll limit
    //  the x size based on the y size, but not vice versa, unless the
    //  caller swears the parameters were the same (correct) in every pass.
    //  As a result, triads could appear vertically stretched if:
    //  a.) mask_resize_src_lut_size.x > mask_resize_src_lut_size.y: Wide
    //      LUT's might clamp x more than y (all provided LUT's are square)
    //  b.) true_viewport_size.x < true_viewport_size.y: The user is playing
    //      with a vertically oriented screen (not accounted for anyway)
    //  c.) mask_resize_viewport_scale.x < masked_resize_viewport_scale.y:
    //      Viewport scales are equal by default.
    //  If any of these are the case, you can fix the stretching by setting:
    //      mask_resize_viewport_scale.x = mask_resize_viewport_scale.y *
    //          (1.0 / min_expected_aspect_ratio) *
    //          (mask_resize_src_lut_size.x / mask_resize_src_lut_size.y)
    const float x_tile_size_from_y =
        clamped_tile_size.y * tile_aspect_ratio;
    const float y_tile_size_from_x = lerp(clamped_tile_size.y,
        clamped_tile_size.x * tile_aspect_ratio_inv,
        float(solemnly_swear_same_inputs_for_every_pass));
    const float2 reclamped_tile_size = float2(
        min(clamped_tile_size.x, x_tile_size_from_y),
        min(clamped_tile_size.y, y_tile_size_from_x));
    //  We need integer tile sizes in both directions for tiled sampling to
    //  work correctly.  Use floor (to make sure we don't round up), but be
    //  careful to avoid a rounding bug where floor decreases whole numbers:
    const float2 final_resized_tile_size =
        floor(reclamped_tile_size + float2(FIX_ZERO(0.0)));
    return final_resized_tile_size;
}


/////////////////////////  FINAL MASK SAMPLING HELPERS  ////////////////////////

float4 get_mask_sampling_parameters(const float2 mask_resize_texture_size,
    const float2 mask_resize_video_size, const float2 true_viewport_size,
    out float2 mask_tiles_per_screen)
{
    //  Requires:   1.) Requirements of get_resized_mask_tile_size() must be
    //                  met, particularly regarding global constants.
    //              The function parameters must be defined as follows:
    //              1.) mask_resize_texture_size == MASK_RESIZE.texture_size
    //                  if get_mask_sample_mode() is 0 (otherwise anything)
    //              2.) mask_resize_video_size == MASK_RESIZE.video_size
    //                  if get_mask_sample_mode() is 0 (otherwise anything)
    //              3.) true_viewport_size == output_size for a pass set to
    //                  1.0 viewport scale (i.e. it must be correct)
    //  Returns:    Return a float4 containing:
    //                  xy: tex_uv coords for the start of the mask tile
    //                  zw: tex_uv size of the mask tile from start to end
    //              mask_tiles_per_screen is an out parameter containing the
    //              number of mask tiles that will fit on the screen.
    //  First get the final resized tile size.  The viewport size and mask
    //  resize viewport scale must be correct, but don't solemnly swear they
    //  were correct in both mask resize passes unless you know it's true.
    //  (We can better ensure a correct tile aspect ratio if the parameters are
    //  guaranteed correct in all passes...but if we lie, we'll get inconsistent
    //  sizes across passes, resulting in broken texture coordinates.)
    const float mask_sample_mode = get_mask_sample_mode();
    const float2 mask_resize_tile_size = get_resized_mask_tile_size(
        true_viewport_size, mask_resize_video_size, false);
    if(mask_sample_mode < 0.5)
    {
        //  Sample MASK_RESIZE: The resized tile is a fraction of the texture
        //  size and starts at a nonzero offset to allow for border texels:
        const float2 mask_tile_uv_size = mask_resize_tile_size /
            mask_resize_texture_size;
        const float2 skipped_tiles = mask_start_texels/mask_resize_tile_size;
        const float2 mask_tile_start_uv = skipped_tiles * mask_tile_uv_size;
        //  mask_tiles_per_screen must be based on the *true* viewport size:
        mask_tiles_per_screen = true_viewport_size / mask_resize_tile_size;
        return float4(mask_tile_start_uv, mask_tile_uv_size);
    }
    else
    {
        //  If we're tiling at the original size (1:1 pixel:texel), redefine a
        //  "tile" to be the full texture containing many triads.  Otherwise,
        //  we're hardware-resampling an LUT, and the texture truly contains a
        //  single unresized phosphor mask tile anyway.
        static const float2 mask_tile_uv_size = float2(1.0);
        static const float2 mask_tile_start_uv = float2(0.0);
        if(mask_sample_mode > 1.5)
        {
            //  Repeat the full LUT at a 1:1 pixel:texel ratio without resizing:
            mask_tiles_per_screen = true_viewport_size/mask_texture_large_size;
        }
        else
        {
            //  Hardware-resize the original LUT:
            mask_tiles_per_screen = true_viewport_size / mask_resize_tile_size;
        }
        return float4(mask_tile_start_uv, mask_tile_uv_size);
    }
}
/*
float2 fix_tiling_discontinuities_normalized(const float2 tile_uv,
    float2 duv_dx, float2 duv_dy)
{
    //  Requires:   1.) duv_dx == ddx(tile_uv)
    //              2.) duv_dy == ddy(tile_uv)
    //              3.) tile_uv contains tile-relative uv coords in [0, 1],
    //                  such that (0.5, 0.5) is the center of a tile, etc.
    //                  ("Tile" can mean texture, the video embedded in the
    //                  texture, or some other "tile" embedded in a texture.)
    //  Returns:    Return new tile_uv coords that contain no discontinuities
    //              across a 2x2 pixel quad.
    //  Description:
    //  When uv coords wrap from 1.0 to 0.0, they create a discontinuity in the
    //  derivatives, which we assume happened if the absolute difference between
    //  any fragment in a 2x2 block is > ~half a tile.  If the current block has
    //  a u or v discontinuity and the current fragment is in the first half of
    //  the tile along that axis (i.e. it wrapped from 1.0 to 0.0), add a tile
    //  to that coord to make the 2x2 block continuous.  (It will now have a
    //  coord > 1.0 in the padding area beyond the tile.)  This function takes
    //  derivatives as parameters so the caller can reuse them.
    //  In case we're using high-quality (nVidia-style) derivatives, ensure
    //  diagonically opposite fragments see each other for correctness:
    duv_dx = abs(duv_dx) + abs(ddy(duv_dx));
    duv_dy = abs(duv_dy) + abs(ddx(duv_dy));
    const float2 pixel_in_first_half_tile = float2((tile_uv.x < 0.5),(tile_uv.y < 0.5));
    const float2 jump_exists = float2(((duv_dx + duv_dy).x > 0.5),((duv_dx + duv_dy).y > 0.5));
    return tile_uv + jump_exists * pixel_in_first_half_tile;
}
*/
float2 convert_phosphor_tile_uv_wrap_to_tex_uv(const float2 tile_uv_wrap,
    const float4 mask_tile_start_uv_and_size)
{
    //  Requires:   1.) tile_uv_wrap contains tile-relative uv coords, where the
    //                  tile spans from [0, 1], such that (0.5, 0.5) is at the
    //                  tile center.  The input coords can range from [0, inf],
    //                  and their fractional parts map to a repeated tile.
    //                  ("Tile" can mean texture, the video embedded in the
    //                  texture, or some other "tile" embedded in a texture.)
    //              2.) mask_tile_start_uv_and_size.xy contains tex_uv coords
    //                  for the start of the embedded tile in the full texture.
    //              3.) mask_tile_start_uv_and_size.zw contains the [fractional]
    //                  tex_uv size of the embedded tile in the full texture.
    //  Returns:    Return tex_uv coords (used for texture sampling)
    //              corresponding to tile_uv_wrap.
    if(get_mask_sample_mode() < 0.5)
    {
        //  Manually repeat the resized mask tile to fill the screen:
        //  First get fractional tile_uv coords.  Using frac/fmod on coords
        //  confuses anisotropic filtering; fix it as user options dictate.
        //  derived-settings-and-constants.h disables incompatible options.
        #ifdef ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE
            float2 tile_uv = frac(tile_uv_wrap * 0.5) * 2.0;
        #else
            float2 tile_uv = frac(tile_uv_wrap);
        #endif
        #ifdef ANISOTROPIC_TILING_COMPAT_FIX_DISCONTINUITIES
            const float2 tile_uv_dx = ddx(tile_uv);
            const float2 tile_uv_dy = ddy(tile_uv);
            tile_uv = fix_tiling_discontinuities_normalized(tile_uv,
                tile_uv_dx, tile_uv_dy);
        #endif
        //  The tile is embedded in a padded FBO, and it may start at a
        //  nonzero offset if border texels are used to avoid artifacts:
        const float2 mask_tex_uv = mask_tile_start_uv_and_size.xy +
            tile_uv * mask_tile_start_uv_and_size.zw;
        return mask_tex_uv;
    }
    else
    {
        //  Sample from the input phosphor mask texture with hardware tiling.
        //  If we're tiling at the original size (mode 2), the "tile" is the
        //  whole texture, and it contains a large number of triads mapped with
        //  a 1:1 pixel:texel ratio.  OTHERWISE, the texture contains a single
        //  unresized tile.  tile_uv_wrap already has correct coords for both!
        return tile_uv_wrap;
    }
}


#endif  //  PHOSPHOR_MASK_RESIZING_H

/////////////////////////  END PHOSPHOR-MASK-RESIZING  /////////////////////////

#undef COMPAT_PRECISION
#undef COMPAT_TEXTURE

void main() {
    //  Resize the input phosphor mask tile to the final vertical size it will
    //  appear on screen.  Keep 1x horizontal size if possible (IN.output_size
    //  >= mask_resize_src_lut_size), and otherwise linearly sample horizontally
    //  to fit exactly one tile.  Lanczos-resizing the phosphor mask achieves
    //  much sharper results than mipmapping, and vertically resizing first
    //  minimizes the total number of taps required.  We output a number of
    //  resized tiles >= mask_resize_num_tiles for easier tiled sampling later.
    //const float2 src_tex_uv_wrap = src_tex_uv_wrap;
    #ifdef PHOSPHOR_MASK_MANUALLY_RESIZE
        //  Discard unneeded fragments in case our profile allows real branches.
        const float2 tile_uv_wrap = src_tex_uv_wrap;
        if(get_mask_sample_mode() < 0.5 &&
            tile_uv_wrap.y <= mask_resize_num_tiles)
        {
            static const float src_dy = 1.0/mask_resize_src_lut_size.y;
            const float2 src_tex_uv = frac(src_tex_uv_wrap);
            float3 pixel_color;
            //  If mask_type is static, this branch will be resolved statically.
			#ifdef PHOSPHOR_MASK_RESIZE_MIPMAPPED_LUT
				if(mask_type < 0.5)
				{
					pixel_color = downsample_vertical_sinc_tiled(
						mask_grille_texture_large, src_tex_uv, mask_resize_src_lut_size,
						src_dy, resize_magnification_scale.y, 1.0);
				}
				else if(mask_type < 1.5)
				{
					pixel_color = downsample_vertical_sinc_tiled(
						mask_slot_texture_large, src_tex_uv, mask_resize_src_lut_size,
						src_dy, resize_magnification_scale.y, 1.0);
				}
				else
				{
					pixel_color = downsample_vertical_sinc_tiled(
						mask_shadow_texture_large, src_tex_uv, mask_resize_src_lut_size,
						src_dy, resize_magnification_scale.y, 1.0);
				}
			#else
				if(mask_type < 0.5)
				{
					pixel_color = downsample_vertical_sinc_tiled(
						mask_grille_texture_small, src_tex_uv, mask_resize_src_lut_size,
						src_dy, resize_magnification_scale.y, 1.0);
				}
				else if(mask_type < 1.5)
				{
					pixel_color = downsample_vertical_sinc_tiled(
						mask_slot_texture_small, src_tex_uv, mask_resize_src_lut_size,
						src_dy, resize_magnification_scale.y, 1.0);
				}
				else
				{
					pixel_color = downsample_vertical_sinc_tiled(
						mask_shadow_texture_small, src_tex_uv, mask_resize_src_lut_size,
						src_dy, resize_magnification_scale.y, 1.0);
				}
			#endif
            //  The input LUT was linear RGB, and so is our output:
            FragColor = float4(pixel_color, 1.0);
        }
        else
        {
            discard;
        }
    #else
        discard;
        FragColor = float4(1.0, 1.0, 1.0, 1.0);
	#endif
}