#version 150

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

#if __VERSION__ >= 130
#define COMPAT_TEXTURE texture
#else
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 vTexCoord;
   vec2 blur_dxdy;
};

uniform vec4 targetSize;
uniform vec4 sourceSize[];

/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////

//  PASS SETTINGS:
//  gamma-management.h needs to know what kind of pipeline we're using and
//  what pass this is in that pipeline.  This will become obsolete if/when we
//  can #define things like this in the .cgp preset file.
//#define GAMMA_ENCODE_EVERY_FBO
//#define FIRST_PASS
//#define LAST_PASS
//#define SIMULATE_CRT_ON_LCD
//#define SIMULATE_GBA_ON_LCD
//#define SIMULATE_LCD_ON_CRT
//#define SIMULATE_GBA_ON_CRT

#ifndef GAMMA_MANAGEMENT_H
#define GAMMA_MANAGEMENT_H

///////////////////////////////  BASE CONSTANTS  ///////////////////////////////

//  Set standard gamma constants, but allow users to override them:
#ifndef OVERRIDE_STANDARD_GAMMA
    //  Standard encoding gammas:
    float ntsc_gamma = 2.2;    //  Best to use NTSC for PAL too?
    float pal_gamma = 2.8;     //  Never actually 2.8 in practice
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
    float crt_reference_gamma_high = 2.5;  //  In (2.35, 2.55)
    float crt_reference_gamma_low = 2.35;  //  In (2.35, 2.55)
    float lcd_reference_gamma = 2.5;       //  To match CRT
    float crt_office_gamma = 2.2;  //  Circuitry-adjusted for NTSC
    float lcd_office_gamma = 2.2;  //  Approximates sRGB
#endif  //  OVERRIDE_STANDARD_GAMMA

//  Assuming alpha == 1.0 might make it easier for users to avoid some bugs,
//  but only if they're aware of it.
#ifndef OVERRIDE_ALPHA_ASSUMPTIONS
    bool assume_opaque_alpha = false;
#endif


///////////////////////  DERIVED CONSTANTS AS FUNCTIONS  ///////////////////////

//  gamma-management.h should be compatible with overriding gamma values with
//  runtime user parameters, but we can only define other global constants in
//  terms of static constants, not uniform user parameters.  To get around this
//  limitation, we need to define derived constants using functions.

//  Set device gamma constants, but allow users to override them:
#ifdef OVERRIDE_DEVICE_GAMMA
    //  The user promises to globally define the appropriate constants:
    float get_crt_gamma()    {   return crt_gamma;   }
    float get_gba_gamma()    {   return gba_gamma;   }
    float get_lcd_gamma()    {   return lcd_gamma;   }
#else
    float get_crt_gamma()    {   return crt_reference_gamma_high;    }
    float get_gba_gamma()    {   return 3.5; }   //  Game Boy Advance; in (3.0, 4.0)
    float get_lcd_gamma()    {   return lcd_office_gamma;            }
#endif  //  OVERRIDE_DEVICE_GAMMA

//  Set decoding/encoding gammas for the first/lass passes, but allow overrides:
#ifdef OVERRIDE_FINAL_GAMMA
    //  The user promises to globally define the appropriate constants:
    float get_intermediate_gamma()   {   return intermediate_gamma;  }
    float get_input_gamma()          {   return input_gamma;         }
    float get_output_gamma()         {   return output_gamma;        }
#else
    //  If we gamma-correct every pass, always use ntsc_gamma between passes to
    //  ensure middle passes don't need to care if anything is being simulated:
    float get_intermediate_gamma()   {   return ntsc_gamma;          }
    #ifdef SIMULATE_CRT_ON_LCD
        float get_input_gamma()      {   return get_crt_gamma();     }
        float get_output_gamma()     {   return get_lcd_gamma();     }
    #else
    #ifdef SIMULATE_GBA_ON_LCD
        float get_input_gamma()      {   return get_gba_gamma();     }
        float get_output_gamma()     {   return get_lcd_gamma();     }
    #else
    #ifdef SIMULATE_LCD_ON_CRT
        float get_input_gamma()      {   return get_lcd_gamma();     }
        float get_output_gamma()     {   return get_crt_gamma();     }
    #else
    #ifdef SIMULATE_GBA_ON_CRT
        float get_input_gamma()      {   return get_gba_gamma();     }
        float get_output_gamma()     {   return get_crt_gamma();     }
    #else   //  Don't simulate anything:
        float get_input_gamma()      {   return ntsc_gamma;          }
        float get_output_gamma()     {   return ntsc_gamma;          }
    #endif  //  SIMULATE_GBA_ON_CRT
    #endif  //  SIMULATE_LCD_ON_CRT
    #endif  //  SIMULATE_GBA_ON_LCD
    #endif  //  SIMULATE_CRT_ON_LCD
#endif  //  OVERRIDE_FINAL_GAMMA

#ifndef GAMMA_ENCODE_EVERY_FBO
    #ifdef FIRST_PASS
        bool linearize_input = true;
        float get_pass_input_gamma()     {   return get_input_gamma();   }
    #else
        bool linearize_input = false;
        float get_pass_input_gamma()     {   return 1.0;                 }
    #endif
    #ifdef LAST_PASS
        bool gamma_encode_output = true;
        float get_pass_output_gamma()    {   return get_output_gamma();  }
    #else
        bool gamma_encode_output = false;
        float get_pass_output_gamma()    {   return 1.0;                 }
    #endif
#else
    bool linearize_input = true;
    bool gamma_encode_output = true;
    #ifdef FIRST_PASS
        float get_pass_input_gamma()     {   return get_input_gamma();   }
    #else
        float get_pass_input_gamma()     {   return get_intermediate_gamma();    }
    #endif
    #ifdef LAST_PASS
        float get_pass_output_gamma()    {   return get_output_gamma();  }
    #else
        float get_pass_output_gamma()    {   return get_intermediate_gamma();    }
    #endif
#endif

vec4 decode_input(vec4 color)
{
    if(linearize_input = true)
    {
        if(assume_opaque_alpha = true)
        {
            return vec4(pow(color.rgb, vec3(get_pass_input_gamma())), 1.0);
        }
        else
        {
            return vec4(pow(color.rgb, vec3(get_pass_input_gamma())), color.a);
        }
    }
    else
    {
        return color;
    }
}

vec4 encode_output(vec4 color)
{
    if(gamma_encode_output = true)
    {
        if(assume_opaque_alpha = true)
        {
            return vec4(pow(color.rgb, vec3(1.0/get_pass_output_gamma())), 1.0);
        }
        else
        {
            return vec4(pow(color.rgb, vec3(1.0/get_pass_output_gamma())), color.a);
        }
    }
    else
    {
        return color;
    }
}

#define tex2D_linearize(C, D) decode_input(vec4(COMPAT_TEXTURE(C, D)))
//vec4 tex2D_linearize(sampler2D tex, vec2 tex_coords)
//{   return decode_input(vec4(COMPAT_TEXTURE(tex, tex_coords)));   }

//#define tex2D_linearize(C, D, E) decode_input(vec4(COMPAT_TEXTURE(C, D, E)))
//vec4 tex2D_linearize(sampler2D tex, vec2 tex_coords, int texel_off)
//{   return decode_input(vec4(COMPAT_TEXTURE(tex, tex_coords, texel_off)));    }

#endif  //  GAMMA_MANAGEMENT_H

#ifndef BLUR_FUNCTIONS_H
#define BLUR_FUNCTIONS_H

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

//  This file provides reusable one-pass and separable (two-pass) blurs.
//  Requires:   All blurs share these requirements (dxdy requirement is split):
//              1.) All requirements of gamma-management.h must be satisfied!
//              2.) filter_linearN must == "true" in your .cgp preset unless
//                  you're using tex2DblurNresize at 1x scale.
//              3.) mipmap_inputN must == "true" in your .cgp preset if
//                  IN.output_size < IN.video_size.
//              4.) IN.output_size == IN.video_size / pow(2, M), where M is some
//                  positive integer.  tex2Dblur*resize can resize arbitrarily
//                  (and the blur will be done after resizing), but arbitrary
//                  resizes "fail" with other blurs due to the way they mix
//                  static weights with bilinear sample exploitation.
//              5.) In general, dxdy should contain the uv pixel spacing:
//                      dxdy = (IN.video_size/IN.output_size)/IN.texture_size
//              6.) For separable blurs (tex2DblurNresize and tex2DblurNfast),
//                  zero out the dxdy component in the unblurred dimension:
//                      dxdy = vec2(dxdy.x, 0.0) or vec2(0.0, dxdy.y)
//              Many blurs share these requirements:
//              1.) One-pass blurs require scale_xN == scale_yN or scales > 1.0,
//                  or they will blur more in the lower-scaled dimension.
//              2.) One-pass shared sample blurs require ddx(), ddy(), and
//                  tex2Dlod() to be supported by the current Cg profile, and
//                  the drivers must support high-quality derivatives.
//              3.) One-pass shared sample blurs require:
//                      tex_uv.w == log2(IN.video_size/IN.output_size).y;
//              Non-wrapper blurs share this requirement:
//              1.) sigma is the intended standard deviation of the blur
//              Wrapper blurs share this requirement, which is automatically
//              met (unless OVERRIDE_BLUR_STD_DEVS is #defined; see below):
//              1.) blurN_std_dev must be global static float values
//                  specifying standard deviations for Nx blurs in units
//                  of destination pixels
//  Optional:   1.) The including file (or an earlier included file) may
//                  optionally #define USE_BINOMIAL_BLUR_STD_DEVS to replace
//                  default standard deviations with those matching a binomial
//                  distribution.  (See below for details/properties.)
//              2.) The including file (or an earlier included file) may
//                  optionally #define OVERRIDE_BLUR_STD_DEVS and override:
//                      static float blur3_std_dev
//                      static float blur4_std_dev
//                      static float blur5_std_dev
//                      static float blur6_std_dev
//                      static float blur7_std_dev
//                      static float blur8_std_dev
//                      static float blur9_std_dev
//                      static float blur10_std_dev
//                      static float blur11_std_dev
//                      static float blur12_std_dev
//                      static float blur17_std_dev
//                      static float blur25_std_dev
//                      static float blur31_std_dev
//                      static float blur43_std_dev
//              3.) The including file (or an earlier included file) may
//                  optionally #define OVERRIDE_ERROR_BLURRING and override:
//                      static float error_blurring
//                  This tuning value helps mitigate weighting errors from one-
//                  pass shared-sample blurs sharing bilinear samples between
//                  fragments.  Values closer to 0.0 have "correct" blurriness
//                  but allow more artifacts, and values closer to 1.0 blur away
//                  artifacts by sampling closer to halfway between texels.
//              UPDATE 6/21/14: The above static constants may now be overridden
//              by non-static uniform constants.  This permits exposing blur
//              standard deviations as runtime GUI shader parameters.  However,
//              using them keeps weights from being statically computed, and the
//              speed hit depends on the blur: On my machine, uniforms kill over
//              53% of the framerate with tex2Dblur12x12shared, but they only
//              drop the framerate by about 18% with tex2Dblur11fast.
//  Quality and Performance Comparisons:
//  For the purposes of the following discussion, "no sRGB" means
//  GAMMA_ENCODE_EVERY_FBO is #defined, and "sRGB" means it isn't.
//  1.) tex2DblurNfast is always faster than tex2DblurNresize.
//  2.) tex2DblurNresize functions are the only ones that can arbitrarily resize
//      well, because they're the only ones that don't exploit bilinear samples.
//      This also means they're the only functions which can be truly gamma-
//      correct without linear (or sRGB FBO) input, but only at 1x scale.
//  3.) One-pass shared sample blurs only have a speed advantage without sRGB.
//      They also have some inaccuracies due to their shared-[bilinear-]sample
//      design, which grow increasingly bothersome for smaller blurs and higher-
//      frequency source images (relative to their resolution).  I had high
//      hopes for them, but their most realistic use case is limited to quickly
//      reblurring an already blurred input at full resolution.  Otherwise:
//      a.) If you're blurring a low-resolution source, you want a better blur.
//      b.) If you're blurring a lower mipmap, you want a better blur.
//      c.) If you're blurring a high-resolution, high-frequency source, you
//          want a better blur.
//  4.) The one-pass blurs without shared samples grow slower for larger blurs,
//      but they're competitive with separable blurs at 5x5 and smaller, and
//      even tex2Dblur7x7 isn't bad if you're wanting to conserve passes.
//  Here are some framerates from a GeForce 8800GTS.  The first pass resizes to
//  viewport size (4x in this test) and linearizes for sRGB codepaths, and the
//  remaining passes perform 6 full blurs.  Mipmapped tests are performed at the
//  same scale, so they just measure the cost of mipmapping each FBO (only every
//  other FBO is mipmapped for separable blurs, to mimic realistic usage).
//  Mipmap      Neither     sRGB+Mipmap sRGB        Function
//  76.0        92.3        131.3       193.7       tex2Dblur3fast
//  63.2        74.4        122.4       175.5       tex2Dblur3resize
//  93.7        121.2       159.3       263.2       tex2Dblur3x3
//  59.7        68.7        115.4       162.1       tex2Dblur3x3resize
//  63.2        74.4        122.4       175.5       tex2Dblur5fast
//  49.3        54.8        100.0       132.7       tex2Dblur5resize
//  59.7        68.7        115.4       162.1       tex2Dblur5x5
//  64.9        77.2        99.1        137.2       tex2Dblur6x6shared
//  55.8        63.7        110.4       151.8       tex2Dblur7fast
//  39.8        43.9        83.9        105.8       tex2Dblur7resize
//  40.0        44.2        83.2        104.9       tex2Dblur7x7
//  56.4        65.5        71.9        87.9        tex2Dblur8x8shared
//  49.3        55.1        99.9        132.5       tex2Dblur9fast
//  33.3        36.2        72.4        88.0        tex2Dblur9resize
//  27.8        29.7        61.3        72.2        tex2Dblur9x9
//  37.2        41.1        52.6        60.2        tex2Dblur10x10shared
//  44.4        49.5        91.3        117.8       tex2Dblur11fast
//  28.8        30.8        63.6        75.4        tex2Dblur11resize
//  33.6        36.5        40.9        45.5        tex2Dblur12x12shared
//  TODO: Fill in benchmarks for new untested blurs.
//                                                  tex2Dblur17fast
//                                                  tex2Dblur25fast
//                                                  tex2Dblur31fast
//                                                  tex2Dblur43fast
//                                                  tex2Dblur3x3resize

/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////

//  Set static standard deviations, but allow users to override them with their
//  own constants (even non-static uniforms if they're okay with the speed hit):
#ifndef OVERRIDE_BLUR_STD_DEVS
    //  blurN_std_dev values are specified in terms of dxdy strides.
    #ifdef USE_BINOMIAL_BLUR_STD_DEVS
        //  By request, we can define standard deviations corresponding to a
        //  binomial distribution with p = 0.5 (related to Pascal's triangle).
        //  This distribution works such that blurring multiple times should
        //  have the same result as a single larger blur.  These values are
        //  larger than default for blurs up to 6x and smaller thereafter.
        float blur3_std_dev = 0.84931640625;
        float blur4_std_dev = 0.84931640625;
        float blur5_std_dev = 1.0595703125;
        float blur6_std_dev = 1.06591796875;
        float blur7_std_dev = 1.17041015625;
        float blur8_std_dev = 1.1720703125;
        float blur9_std_dev = 1.2259765625;
        float blur10_std_dev = 1.21982421875;
        float blur11_std_dev = 1.25361328125;
        float blur12_std_dev = 1.2423828125;
        float blur17_std_dev = 1.27783203125;
        float blur25_std_dev = 1.2810546875;
        float blur31_std_dev = 1.28125;
        float blur43_std_dev = 1.28125;
    #else
        //  The defaults are the largest values that keep the largest unused
        //  blur term on each side <= 1.0/256.0.  (We could get away with more
        //  or be more conservative, but this compromise is pretty reasonable.)
        float blur3_std_dev = 0.62666015625;
        float blur4_std_dev = 0.66171875;
        float blur5_std_dev = 0.9845703125;
        float blur6_std_dev = 1.02626953125;
        float blur7_std_dev = 1.36103515625;
        float blur8_std_dev = 1.4080078125;
        float blur9_std_dev = 1.7533203125;
        float blur10_std_dev = 1.80478515625;
        float blur11_std_dev = 2.15986328125;
        float blur12_std_dev = 2.215234375;
        float blur17_std_dev = 3.45535583496;
        float blur25_std_dev = 5.3409576416;
        float blur31_std_dev = 6.86488037109;
        float blur43_std_dev = 10.1852050781;
    #endif  //  USE_BINOMIAL_BLUR_STD_DEVS
#endif  //  OVERRIDE_BLUR_STD_DEVS

#ifndef OVERRIDE_ERROR_BLURRING
    //  error_blurring should be in [0.0, 1.0].  Higher values reduce ringing
    //  in shared-sample blurs but increase blurring and feature shifting.
    float error_blurring = 0.5;
#endif

//  Make a length squared helper macro (for usage with static constants):
#define LENGTH_SQ(vec) (dot(vec, vec))

//////////////////////////////////  INCLUDES  //////////////////////////////////

//  gamma-management.h relies on pass-specific settings to guide its behavior:
//  FIRST_PASS, LAST_PASS, GAMMA_ENCODE_EVERY_FBO, etc.  See it for details.
//#include "gamma-management.h"
//#include "quad-pixel-communication.h"
//#include "special-functions.h"

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
//  different input types (vec4/vec3/vec2/float).  This is unfortunate,
//  but Cg doesn't allow function templates.  Macros would be far less verbose,
//  but they would make the code harder to document and read.  I don't expect
//  these functions will require a whole lot of maintenance changes unless
//  someone ever has need for more robust incomplete gamma functions, so code
//  duplication seems to be the lesser evil in this case.


///////////////////////////  GAUSSIAN ERROR FUNCTION  //////////////////////////

vec4 erf6(vec4 x)
{
    //  Requires:   x is the standard parameter to erf().
    //  Returns:    Return an Abramowitz/Stegun approximation of erf(), where:
    //                  erf(x) = 2/sqrt(pi) * integral(e**(-x**2))
    //              This approximation has a max absolute error of 2.5*10**-5
    //              with solid numerical robustness and efficiency.  See:
	//                  https://en.wikipedia.org/wiki/Error_function#Approximation_with_elementary_functions
	vec4 one = vec4(1.0);
	vec4 sign_x = sign(x);
	vec4 t = one/(one + 0.47047*abs(x));
	vec4 result = one - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

vec3 erf6(vec3 x)
{
    //  vec3 version:
	vec3 one = vec3(1.0);
	vec3 sign_x = sign(x);
	vec3 t = one/(one + 0.47047*abs(x));
	vec3 result = one - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

vec2 erf6(vec2 x)
{
    //  vec2 version:
	vec2 one = vec2(1.0);
	vec2 sign_x = sign(x);
	vec2 t = one/(one + 0.47047*abs(x));
	vec2 result = one - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

float erf6(float x)
{
    //  Float version:
	float sign_x = sign(x);
	float t = 1.0/(1.0 + 0.47047*abs(x));
	float result = 1.0 - t*(0.3480242 + t*(-0.0958798 + t*0.7478556))*
		exp(-(x*x));
	return result * sign_x;
}

vec4 erft(vec4 x)
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

vec3 erft(vec3 x)
{
    //  vec3 version:
	return tanh(1.202760580 * x);
}

vec2 erft(vec2 x)
{
    //  vec2 version:
	return tanh(1.202760580 * x);
}

float erft(float x)
{
    //  Float version:
	return tanh(1.202760580 * x);
}

vec4 erf(vec4 x)
{
    //  Requires:   x is the standard parameter to erf().
    //  Returns:    Some approximation of erf(x), depending on user settings.
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

vec3 erf(vec3 x)
{
    //  vec3 version:
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

vec2 erf(vec2 x)
{
    //  vec2 version:
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

float erf(float x)
{
    //  Float version:
	#ifdef ERF_FAST_APPROXIMATION
		return erft(x);
	#else
		return erf6(x);
	#endif
}

///////////////////////////  COMPLETE GAMMA FUNCTION  //////////////////////////

vec4 gamma_impl(vec4 s, vec4 s_inv)
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
	vec4 g = vec4(1.12906830989);
	vec4 c0 = vec4(0.8109119309638332633713423362694399653724431);
	vec4 c1 = vec4(0.4808354605142681877121661197951496120000040);
	vec4 e = vec4(2.71828182845904523536028747135266249775724709);
	vec4 sph = s + vec4(0.5);
	vec4 lanczos_sum = c0 + c1/(s + vec4(1.0));
	vec4 base = (sph + g)/e;  //  or (s + g + vec4(0.5))/e
	//  gamma(s + 1) = base**sph * lanczos_sum; divide by s for gamma(s).
	//  This has less error for small s's than (s -= 1.0) at the beginning.
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

vec3 gamma_impl(vec3 s, vec3 s_inv)
{
    //  vec3 version:
	vec3 g = vec3(1.12906830989);
	vec3 c0 = vec3(0.8109119309638332633713423362694399653724431);
	vec3 c1 = vec3(0.4808354605142681877121661197951496120000040);
	vec3 e = vec3(2.71828182845904523536028747135266249775724709);
	vec3 sph = s + vec3(0.5);
	vec3 lanczos_sum = c0 + c1/(s + vec3(1.0));
	vec3 base = (sph + g)/e;
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

vec2 gamma_impl(vec2 s, vec2 s_inv)
{
    //  vec2 version:
	vec2 g = vec2(1.12906830989);
	vec2 c0 = vec2(0.8109119309638332633713423362694399653724431);
	vec2 c1 = vec2(0.4808354605142681877121661197951496120000040);
	vec2 e = vec2(2.71828182845904523536028747135266249775724709);
	vec2 sph = s + vec2(0.5);
	vec2 lanczos_sum = c0 + c1/(s + vec2(1.0));
	vec2 base = (sph + g)/e;
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

float gamma_impl(float s, float s_inv)
{
    //  Float version:
	float g = 1.12906830989;
	float c0 = 0.8109119309638332633713423362694399653724431;
	float c1 = 0.4808354605142681877121661197951496120000040;
	float e = 2.71828182845904523536028747135266249775724709;
	float sph = s + 0.5;
	float lanczos_sum = c0 + c1/(s + 1.0);
	float base = (sph + g)/e;
	return (pow(base, sph) * lanczos_sum) * s_inv;
}

vec4 gamma(vec4 s)
{
    //  Requires:   s is the standard parameter to the gamma function, and it
    //              should lie in the [0, 36] range.
    //  Returns:    Return approximate gamma function output with a maximum
    //              relative error of 0.000463.  See gamma_impl for details.
	return gamma_impl(s, vec4(1.0)/s);
}

vec3 gamma(vec3 s)
{
    //  vec3 version:
	return gamma_impl(s, vec3(1.0)/s);
}

vec2 gamma(vec2 s)
{
    //  vec2 version:
	return gamma_impl(s, vec2(1.0)/s);
}

float gamma(float s)
{
    //  Float version:
	return gamma_impl(s, 1.0/s);
}

////////////////  INCOMPLETE GAMMA FUNCTIONS (RESTRICTED INPUT)  ///////////////

//  Lower incomplete gamma function for small s and z (implementation):
vec4 ligamma_small_z_impl(vec4 s, vec4 z, vec4 s_inv)
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
	vec4 scale = pow(z, s);
	vec4 sum = s_inv;  //  Summation iteration 0 result
	//  Summation iterations 1, 2, and 3:
	vec4 z_sq = z*z;
	vec4 denom1 = s + vec4(1.0);
	vec4 denom2 = 2.0*s + vec4(4.0);
	vec4 denom3 = 6.0*s + vec4(18.0);
	//vec4 denom4 = 24.0*s + vec4(96.0);
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	//sum += z_sq * z_sq / denom4;
	//  Scale and return:
	return scale * sum;
}

vec3 ligamma_small_z_impl(vec3 s, vec3 z, vec3 s_inv)
{
    //  vec3 version:
	vec3 scale = pow(z, s);
	vec3 sum = s_inv;
	vec3 z_sq = z*z;
	vec3 denom1 = s + vec3(1.0);
	vec3 denom2 = 2.0*s + vec3(4.0);
	vec3 denom3 = 6.0*s + vec3(18.0);
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	return scale * sum;
}

vec2 ligamma_small_z_impl(vec2 s, vec2 z, vec2 s_inv)
{
    //  vec2 version:
	vec2 scale = pow(z, s);
	vec2 sum = s_inv;
	vec2 z_sq = z*z;
	vec2 denom1 = s + vec2(1.0);
	vec2 denom2 = 2.0*s + vec2(4.0);
	vec2 denom3 = 6.0*s + vec2(18.0);
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	return scale * sum;
}

float ligamma_small_z_impl(float s, float z, float s_inv)
{
    //  Float version:
	float scale = pow(z, s);
	float sum = s_inv;
	float z_sq = z*z;
	float denom1 = s + 1.0;
	float denom2 = 2.0*s + 4.0;
	float denom3 = 6.0*s + 18.0;
	sum -= z/denom1;
	sum += z_sq/denom2;
	sum -= z * z_sq/denom3;
	return scale * sum;
}

//  Upper incomplete gamma function for small s and large z (implementation):
vec4 uigamma_large_z_impl(vec4 s, vec4 z)
{
    //  Requires:   1.) s < ~0.5
    //              2.) z > ~0.775075
    //  Returns:    Gauss's continued fraction representation for the upper
    //              incomplete gamma function (4 terms).
	//  The "rolled up" continued fraction looks like this.  The denominator
    //  is truncated, and it's calculated "from the bottom up:"
	//      denom = vec4('inf');
	//      vec4 one = vec4(1.0);
	//      for(int i = 4; i > 0; --i)
	//      {
	//          denom = ((i * 2.0) - one) + z - s + (i * (s - i))/denom;
	//      }
	//  Unrolled and constant-unfolded for madds and parallelism:
	vec4 numerator = pow(z, s) * exp(-z);
	vec4 denom = vec4(7.0) + z - s;
	denom = vec4(5.0) + z - s + (3.0*s - vec4(9.0))/denom;
	denom = vec4(3.0) + z - s + (2.0*s - vec4(4.0))/denom;
	denom = vec4(1.0) + z - s + (s - vec4(1.0))/denom;
	return numerator / denom;
}

vec3 uigamma_large_z_impl(vec3 s, vec3 z)
{
    //  vec3 version:
	vec3 numerator = pow(z, s) * exp(-z);
	vec3 denom = vec3(7.0) + z - s;
	denom = vec3(5.0) + z - s + (3.0*s - vec3(9.0))/denom;
	denom = vec3(3.0) + z - s + (2.0*s - vec3(4.0))/denom;
	denom = vec3(1.0) + z - s + (s - vec3(1.0))/denom;
	return numerator / denom;
}

vec2 uigamma_large_z_impl(vec2 s, vec2 z)
{
    //  vec2 version:
	vec2 numerator = pow(z, s) * exp(-z);
	vec2 denom = vec2(7.0) + z - s;
	denom = vec2(5.0) + z - s + (3.0*s - vec2(9.0))/denom;
	denom = vec2(3.0) + z - s + (2.0*s - vec2(4.0))/denom;
	denom = vec2(1.0) + z - s + (s - vec2(1.0))/denom;
	return numerator / denom;
}

float uigamma_large_z_impl(float s, float z)
{
    //  Float version:
	float numerator = pow(z, s) * exp(-z);
	float denom = 7.0 + z - s;
	denom = 5.0 + z - s + (3.0*s - 9.0)/denom;
	denom = 3.0 + z - s + (2.0*s - 4.0)/denom;
	denom = 1.0 + z - s + (s - 1.0)/denom;
	return numerator / denom;
}

//  Normalized lower incomplete gamma function for small s (implementation):
vec4 normalized_ligamma_impl(vec4 s, vec4 z,
    vec4 s_inv, vec4 gamma_s_inv)
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
	vec4 thresh = vec4(0.775075);
	bvec4 z_is_large = greaterThan(z , thresh);
	vec4 z_size_check = vec4(z_is_large.x ? 1.0 : 0.0, z_is_large.y ? 1.0 : 0.0, z_is_large.z ? 1.0 : 0.0, z_is_large.w ? 1.0 : 0.0);
	vec4 large_z = vec4(1.0) - uigamma_large_z_impl(s, z) * gamma_s_inv;
	vec4 small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	//  Combine the results from both branches:
	return large_z * vec4(z_size_check) + small_z * vec4(z_size_check);
}

vec3 normalized_ligamma_impl(vec3 s, vec3 z,
    vec3 s_inv, vec3 gamma_s_inv)
{
    //  vec3 version:
	vec3 thresh = vec3(0.775075);
	bvec3 z_is_large = greaterThan(z , thresh);
	vec3 z_size_check = vec3(z_is_large.x ? 1.0 : 0.0, z_is_large.y ? 1.0 : 0.0, z_is_large.z ? 1.0 : 0.0);
	vec3 large_z = vec3(1.0) - uigamma_large_z_impl(s, z) * gamma_s_inv;
	vec3 small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	return large_z * vec3(z_size_check) + small_z * vec3(z_size_check);
}

vec2 normalized_ligamma_impl(vec2 s, vec2 z,
    vec2 s_inv, vec2 gamma_s_inv)
{
    //  vec2 version:
	vec2 thresh = vec2(0.775075);
	bvec2 z_is_large = greaterThan(z , thresh);
	vec2 z_size_check = vec2(z_is_large.x ? 1.0 : 0.0, z_is_large.y ? 1.0 : 0.0);
	vec2 large_z = vec2(1.0) - uigamma_large_z_impl(s, z) * gamma_s_inv;
	vec2 small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	return large_z * vec2(z_size_check) + small_z * vec2(z_size_check);
}

float normalized_ligamma_impl(float s, float z,
    float s_inv, float gamma_s_inv)
{
    //  Float version:
	float thresh = 0.775075;
	float z_size_check = 0.0;
	if (z > thresh) z_size_check = 1.0;
	float large_z = 1.0 - uigamma_large_z_impl(s, z) * gamma_s_inv;
	float small_z = ligamma_small_z_impl(s, z, s_inv) * gamma_s_inv;
	return large_z * float(z_size_check) + small_z * float(z_size_check);
}

//  Normalized lower incomplete gamma function for small s:
vec4 normalized_ligamma(vec4 s, vec4 z)
{
    //  Requires:   s < ~0.5
    //  Returns:    Approximate the normalized lower incomplete gamma function
    //              for s < 0.5.  See normalized_ligamma_impl() for details.
	vec4 s_inv = vec4(1.0)/s;
	vec4 gamma_s_inv = vec4(1.0)/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

vec3 normalized_ligamma(vec3 s, vec3 z)
{
    //  vec3 version:
	vec3 s_inv = vec3(1.0)/s;
	vec3 gamma_s_inv = vec3(1.0)/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

vec2 normalized_ligamma(vec2 s, vec2 z)
{
    //  vec2 version:
	vec2 s_inv = vec2(1.0)/s;
	vec2 gamma_s_inv = vec2(1.0)/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

float normalized_ligamma(float s, float z)
{
    //  Float version:
	float s_inv = 1.0/s;
	float gamma_s_inv = 1.0/gamma_impl(s, s_inv);
	return normalized_ligamma_impl(s, z, s_inv, gamma_s_inv);
}

#endif  //  SPECIAL_FUNCTIONS_H

///////////////////////////////////  HELPERS  //////////////////////////////////

vec4 uv2_to_uv4(vec2 tex_uv)
{
    //  Make a vec2 uv offset safe for adding to vec4 tex2Dlod coords:
    return vec4(tex_uv, 0.0, 0.0);
}

//  Make a length squared helper macro (for usage with static constants):
#define LENGTH_SQ(vec) (dot(vec, vec))

float get_fast_gaussian_weight_sum_inv(float sigma)
{
    //  We can use the Gaussian integral to calculate the asymptotic weight for
    //  the center pixel.  Since the unnormalized center pixel weight is 1.0,
    //  the normalized weight is the same as the weight sum inverse.  Given a
    //  large enough blur (9+), the asymptotic weight sum is close and faster:
    //      center_weight = 0.5 *
    //          (erf(0.5/(sigma*sqrt(2.0))) - erf(-0.5/(sigma*sqrt(2.0))))
    //      erf(-x) == -erf(x), so we get 0.5 * (2.0 * erf(blah blah)):
    //  However, we can get even faster results with curve-fitting.  These are
    //  also closer than the asymptotic results, because they were constructed
    //  from 64 blurs sizes from [3, 131) and 255 equally-spaced sigmas from
    //  (0, blurN_std_dev), so the results for smaller sigmas are biased toward
    //  smaller blurs.  The max error is 0.0031793913.
    //  Relative FPS: 134.3 with erf, 135.8 with curve-fitting.
    //static float temp = 0.5/sqrt(2.0);
    //return erf(temp/sigma);
    return min(exp(exp(0.348348412457428/
        (sigma - 0.0860587260734721))), 0.399334576340352/sigma);
}

////////////////////  ARBITRARILY RESIZABLE SEPARABLE BLURS  ///////////////////

vec3 tex2Dblur11resize(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Global requirements must be met (see file description).
    //  Returns:    A 1D 11x Gaussian blurred texture lookup using a 11-tap blur.
    //              It may be mipmapped depending on settings and dxdy.
    //  Calculate Gaussian blur kernel weights and a normalization factor for
    //  distances of 0-4, ignoring constant factors (since we're normalizing).
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float w5 = exp(-25.0 * denom_inv);
    float weight_sum_inv = 1.0 /
        (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5));
    //  Statically normalize weights, sum weighted samples, and return.  Blurs are
    //  currently optimized for dynamic weights.
    vec3 sum = vec3(0.0);
    sum += w5 * tex2D_linearize(tex, tex_uv - 5.0 * dxdy).rgb;
    sum += w4 * tex2D_linearize(tex, tex_uv - 4.0 * dxdy).rgb;
    sum += w3 * tex2D_linearize(tex, tex_uv - 3.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv - 2.0 * dxdy).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv - 1.0 * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv + 1.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv + 2.0 * dxdy).rgb;
    sum += w3 * tex2D_linearize(tex, tex_uv + 3.0 * dxdy).rgb;
    sum += w4 * tex2D_linearize(tex, tex_uv + 4.0 * dxdy).rgb;
    sum += w5 * tex2D_linearize(tex, tex_uv + 5.0 * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur9resize(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Global requirements must be met (see file description).
    //  Returns:    A 1D 9x Gaussian blurred texture lookup using a 9-tap blur.
    //              It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * (w1 + w2 + w3 + w4));
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w4 * tex2D_linearize(tex, tex_uv - 4.0 * dxdy).rgb;
    sum += w3 * tex2D_linearize(tex, tex_uv - 3.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv - 2.0 * dxdy).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv - 1.0 * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv + 1.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv + 2.0 * dxdy).rgb;
    sum += w3 * tex2D_linearize(tex, tex_uv + 3.0 * dxdy).rgb;
    sum += w4 * tex2D_linearize(tex, tex_uv + 4.0 * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur7resize(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Global requirements must be met (see file description).
    //  Returns:    A 1D 7x Gaussian blurred texture lookup using a 7-tap blur.
    //              It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * (w1 + w2 + w3));
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w3 * tex2D_linearize(tex, tex_uv - 3.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv - 2.0 * dxdy).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv - 1.0 * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv + 1.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv + 2.0 * dxdy).rgb;
    sum += w3 * tex2D_linearize(tex, tex_uv + 3.0 * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur5resize(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Global requirements must be met (see file description).
    //  Returns:    A 1D 5x Gaussian blurred texture lookup using a 5-tap blur.
    //              It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * (w1 + w2));
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w2 * tex2D_linearize(tex, tex_uv - 2.0 * dxdy).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv - 1.0 * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv + 1.0 * dxdy).rgb;
    sum += w2 * tex2D_linearize(tex, tex_uv + 2.0 * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur3resize(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Global requirements must be met (see file description).
    //  Returns:    A 1D 3x Gaussian blurred texture lookup using a 3-tap blur.
    //              It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * w1);
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w1 * tex2D_linearize(tex, tex_uv - 1.0 * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1 * tex2D_linearize(tex, tex_uv + 1.0 * dxdy).rgb;
    return sum * weight_sum_inv;
}

///////////////////////////  FAST SEPARABLE BLURS  ///////////////////////////

vec3 tex2Dblur11fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   1.) Global requirements must be met (see file description).
    //              2.) filter_linearN must = "true" in your .cgp file.
    //              3.) For gamma-correct bilinear filtering, global
    //                  gamma_aware_bilinear == true (from gamma-management.h)
    //  Returns:    A 1D 11x Gaussian blurred texture lookup using 6 linear
    //              taps.  It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float w5 = exp(-25.0 * denom_inv);
    float weight_sum_inv = 1.0 /
        (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5));
    //  Calculate combined weights and linear sample ratios between texel pairs.
    //  The center texel (with weight w0) is used twice, so halve its weight.
    float w01 = w0 * 0.5 + w1;
    float w23 = w2 + w3;
    float w45 = w4 + w5;
    float w01_ratio = w1/w01;
    float w23_ratio = w3/w23;
    float w45_ratio = w5/w45;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w45 * tex2D_linearize(tex, tex_uv - (4.0 + w45_ratio) * dxdy).rgb;
    sum += w23 * tex2D_linearize(tex, tex_uv - (2.0 + w23_ratio) * dxdy).rgb;
    sum += w01 * tex2D_linearize(tex, tex_uv - w01_ratio * dxdy).rgb;
    sum += w01 * tex2D_linearize(tex, tex_uv + w01_ratio * dxdy).rgb;
    sum += w23 * tex2D_linearize(tex, tex_uv + (2.0 + w23_ratio) * dxdy).rgb;
    sum += w45 * tex2D_linearize(tex, tex_uv + (4.0 + w45_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur17fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 17x Gaussian blurred texture lookup using 1 nearest
    //              neighbor and 8 linear taps.  It may be mipmapped depending
    //              on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float w5 = exp(-25.0 * denom_inv);
    float w6 = exp(-36.0 * denom_inv);
    float w7 = exp(-49.0 * denom_inv);
    float w8 = exp(-64.0 * denom_inv);
    //float weight_sum_inv = 1.0 / (w0 + 2.0 * (
    //    w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8));
    float weight_sum_inv = get_fast_gaussian_weight_sum_inv(sigma);
    //  Calculate combined weights and linear sample ratios between texel pairs.
    float w1_2 = w1 + w2;
    float w3_4 = w3 + w4;
    float w5_6 = w5 + w6;
    float w7_8 = w7 + w8;
    float w1_2_ratio = w2/w1_2;
    float w3_4_ratio = w4/w3_4;
    float w5_6_ratio = w6/w5_6;
    float w7_8_ratio = w8/w7_8;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w7_8 * tex2D_linearize(tex, tex_uv - (7.0 + w7_8_ratio) * dxdy).rgb;
    sum += w5_6 * tex2D_linearize(tex, tex_uv - (5.0 + w5_6_ratio) * dxdy).rgb;
    sum += w3_4 * tex2D_linearize(tex, tex_uv - (3.0 + w3_4_ratio) * dxdy).rgb;
    sum += w1_2 * tex2D_linearize(tex, tex_uv - (1.0 + w1_2_ratio) * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1_2 * tex2D_linearize(tex, tex_uv + (1.0 + w1_2_ratio) * dxdy).rgb;
    sum += w3_4 * tex2D_linearize(tex, tex_uv + (3.0 + w3_4_ratio) * dxdy).rgb;
    sum += w5_6 * tex2D_linearize(tex, tex_uv + (5.0 + w5_6_ratio) * dxdy).rgb;
    sum += w7_8 * tex2D_linearize(tex, tex_uv + (7.0 + w7_8_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur25fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 25x Gaussian blurred texture lookup using 1 nearest
    //              neighbor and 12 linear taps.  It may be mipmapped depending
    //              on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float w5 = exp(-25.0 * denom_inv);
    float w6 = exp(-36.0 * denom_inv);
    float w7 = exp(-49.0 * denom_inv);
    float w8 = exp(-64.0 * denom_inv);
    float w9 = exp(-81.0 * denom_inv);
    float w10 = exp(-100.0 * denom_inv);
    float w11 = exp(-121.0 * denom_inv);
    float w12 = exp(-144.0 * denom_inv);
    //float weight_sum_inv = 1.0 / (w0 + 2.0 * (
    //    w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + w10 + w11 + w12));
    float weight_sum_inv = get_fast_gaussian_weight_sum_inv(sigma);
    //  Calculate combined weights and linear sample ratios between texel pairs.
    float w1_2 = w1 + w2;
    float w3_4 = w3 + w4;
    float w5_6 = w5 + w6;
    float w7_8 = w7 + w8;
    float w9_10 = w9 + w10;
    float w11_12 = w11 + w12;
    float w1_2_ratio = w2/w1_2;
    float w3_4_ratio = w4/w3_4;
    float w5_6_ratio = w6/w5_6;
    float w7_8_ratio = w8/w7_8;
    float w9_10_ratio = w10/w9_10;
    float w11_12_ratio = w12/w11_12;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w11_12 * tex2D_linearize(tex, tex_uv - (11.0 + w11_12_ratio) * dxdy).rgb;
    sum += w9_10 * tex2D_linearize(tex, tex_uv - (9.0 + w9_10_ratio) * dxdy).rgb;
    sum += w7_8 * tex2D_linearize(tex, tex_uv - (7.0 + w7_8_ratio) * dxdy).rgb;
    sum += w5_6 * tex2D_linearize(tex, tex_uv - (5.0 + w5_6_ratio) * dxdy).rgb;
    sum += w3_4 * tex2D_linearize(tex, tex_uv - (3.0 + w3_4_ratio) * dxdy).rgb;
    sum += w1_2 * tex2D_linearize(tex, tex_uv - (1.0 + w1_2_ratio) * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w1_2 * tex2D_linearize(tex, tex_uv + (1.0 + w1_2_ratio) * dxdy).rgb;
    sum += w3_4 * tex2D_linearize(tex, tex_uv + (3.0 + w3_4_ratio) * dxdy).rgb;
    sum += w5_6 * tex2D_linearize(tex, tex_uv + (5.0 + w5_6_ratio) * dxdy).rgb;
    sum += w7_8 * tex2D_linearize(tex, tex_uv + (7.0 + w7_8_ratio) * dxdy).rgb;
    sum += w9_10 * tex2D_linearize(tex, tex_uv + (9.0 + w9_10_ratio) * dxdy).rgb;
    sum += w11_12 * tex2D_linearize(tex, tex_uv + (11.0 + w11_12_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur31fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 31x Gaussian blurred texture lookup using 16 linear
    //              taps.  It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float w5 = exp(-25.0 * denom_inv);
    float w6 = exp(-36.0 * denom_inv);
    float w7 = exp(-49.0 * denom_inv);
    float w8 = exp(-64.0 * denom_inv);
    float w9 = exp(-81.0 * denom_inv);
    float w10 = exp(-100.0 * denom_inv);
    float w11 = exp(-121.0 * denom_inv);
    float w12 = exp(-144.0 * denom_inv);
    float w13 = exp(-169.0 * denom_inv);
    float w14 = exp(-196.0 * denom_inv);
    float w15 = exp(-225.0 * denom_inv);
    //float weight_sum_inv = 1.0 /
    //    (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 +
    //        w9 + w10 + w11 + w12 + w13 + w14 + w15));
    float weight_sum_inv = get_fast_gaussian_weight_sum_inv(sigma);
    //  Calculate combined weights and linear sample ratios between texel pairs.
    //  The center texel (with weight w0) is used twice, so halve its weight.
    float w0_1 = w0 * 0.5 + w1;
    float w2_3 = w2 + w3;
    float w4_5 = w4 + w5;
    float w6_7 = w6 + w7;
    float w8_9 = w8 + w9;
    float w10_11 = w10 + w11;
    float w12_13 = w12 + w13;
    float w14_15 = w14 + w15;
    float w0_1_ratio = w1/w0_1;
    float w2_3_ratio = w3/w2_3;
    float w4_5_ratio = w5/w4_5;
    float w6_7_ratio = w7/w6_7;
    float w8_9_ratio = w9/w8_9;
    float w10_11_ratio = w11/w10_11;
    float w12_13_ratio = w13/w12_13;
    float w14_15_ratio = w15/w14_15;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w14_15 * tex2D_linearize(tex, tex_uv - (14.0 + w14_15_ratio) * dxdy).rgb;
    sum += w12_13 * tex2D_linearize(tex, tex_uv - (12.0 + w12_13_ratio) * dxdy).rgb;
    sum += w10_11 * tex2D_linearize(tex, tex_uv - (10.0 + w10_11_ratio) * dxdy).rgb;
    sum += w8_9 * tex2D_linearize(tex, tex_uv - (8.0 + w8_9_ratio) * dxdy).rgb;
    sum += w6_7 * tex2D_linearize(tex, tex_uv - (6.0 + w6_7_ratio) * dxdy).rgb;
    sum += w4_5 * tex2D_linearize(tex, tex_uv - (4.0 + w4_5_ratio) * dxdy).rgb;
    sum += w2_3 * tex2D_linearize(tex, tex_uv - (2.0 + w2_3_ratio) * dxdy).rgb;
    sum += w0_1 * tex2D_linearize(tex, tex_uv - w0_1_ratio * dxdy).rgb;
    sum += w0_1 * tex2D_linearize(tex, tex_uv + w0_1_ratio * dxdy).rgb;
    sum += w2_3 * tex2D_linearize(tex, tex_uv + (2.0 + w2_3_ratio) * dxdy).rgb;
    sum += w4_5 * tex2D_linearize(tex, tex_uv + (4.0 + w4_5_ratio) * dxdy).rgb;
    sum += w6_7 * tex2D_linearize(tex, tex_uv + (6.0 + w6_7_ratio) * dxdy).rgb;
    sum += w8_9 * tex2D_linearize(tex, tex_uv + (8.0 + w8_9_ratio) * dxdy).rgb;
    sum += w10_11 * tex2D_linearize(tex, tex_uv + (10.0 + w10_11_ratio) * dxdy).rgb;
    sum += w12_13 * tex2D_linearize(tex, tex_uv + (12.0 + w12_13_ratio) * dxdy).rgb;
    sum += w14_15 * tex2D_linearize(tex, tex_uv + (14.0 + w14_15_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur43fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 43x Gaussian blurred texture lookup using 22 linear
    //              taps.  It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float w5 = exp(-25.0 * denom_inv);
    float w6 = exp(-36.0 * denom_inv);
    float w7 = exp(-49.0 * denom_inv);
    float w8 = exp(-64.0 * denom_inv);
    float w9 = exp(-81.0 * denom_inv);
    float w10 = exp(-100.0 * denom_inv);
    float w11 = exp(-121.0 * denom_inv);
    float w12 = exp(-144.0 * denom_inv);
    float w13 = exp(-169.0 * denom_inv);
    float w14 = exp(-196.0 * denom_inv);
    float w15 = exp(-225.0 * denom_inv);
    float w16 = exp(-256.0 * denom_inv);
    float w17 = exp(-289.0 * denom_inv);
    float w18 = exp(-324.0 * denom_inv);
    float w19 = exp(-361.0 * denom_inv);
    float w20 = exp(-400.0 * denom_inv);
    float w21 = exp(-441.0 * denom_inv);
    //float weight_sum_inv = 1.0 /
    //    (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + w10 + w11 +
    //        w12 + w13 + w14 + w15 + w16 + w17 + w18 + w19 + w20 + w21));
    float weight_sum_inv = get_fast_gaussian_weight_sum_inv(sigma);
    //  Calculate combined weights and linear sample ratios between texel pairs.
    //  The center texel (with weight w0) is used twice, so halve its weight.
    float w0_1 = w0 * 0.5 + w1;
    float w2_3 = w2 + w3;
    float w4_5 = w4 + w5;
    float w6_7 = w6 + w7;
    float w8_9 = w8 + w9;
    float w10_11 = w10 + w11;
    float w12_13 = w12 + w13;
    float w14_15 = w14 + w15;
    float w16_17 = w16 + w17;
    float w18_19 = w18 + w19;
    float w20_21 = w20 + w21;
    float w0_1_ratio = w1/w0_1;
    float w2_3_ratio = w3/w2_3;
    float w4_5_ratio = w5/w4_5;
    float w6_7_ratio = w7/w6_7;
    float w8_9_ratio = w9/w8_9;
    float w10_11_ratio = w11/w10_11;
    float w12_13_ratio = w13/w12_13;
    float w14_15_ratio = w15/w14_15;
    float w16_17_ratio = w17/w16_17;
    float w18_19_ratio = w19/w18_19;
    float w20_21_ratio = w21/w20_21;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w20_21 * tex2D_linearize(tex, tex_uv - (20.0 + w20_21_ratio) * dxdy).rgb;
    sum += w18_19 * tex2D_linearize(tex, tex_uv - (18.0 + w18_19_ratio) * dxdy).rgb;
    sum += w16_17 * tex2D_linearize(tex, tex_uv - (16.0 + w16_17_ratio) * dxdy).rgb;
    sum += w14_15 * tex2D_linearize(tex, tex_uv - (14.0 + w14_15_ratio) * dxdy).rgb;
    sum += w12_13 * tex2D_linearize(tex, tex_uv - (12.0 + w12_13_ratio) * dxdy).rgb;
    sum += w10_11 * tex2D_linearize(tex, tex_uv - (10.0 + w10_11_ratio) * dxdy).rgb;
    sum += w8_9 * tex2D_linearize(tex, tex_uv - (8.0 + w8_9_ratio) * dxdy).rgb;
    sum += w6_7 * tex2D_linearize(tex, tex_uv - (6.0 + w6_7_ratio) * dxdy).rgb;
    sum += w4_5 * tex2D_linearize(tex, tex_uv - (4.0 + w4_5_ratio) * dxdy).rgb;
    sum += w2_3 * tex2D_linearize(tex, tex_uv - (2.0 + w2_3_ratio) * dxdy).rgb;
    sum += w0_1 * tex2D_linearize(tex, tex_uv - w0_1_ratio * dxdy).rgb;
    sum += w0_1 * tex2D_linearize(tex, tex_uv + w0_1_ratio * dxdy).rgb;
    sum += w2_3 * tex2D_linearize(tex, tex_uv + (2.0 + w2_3_ratio) * dxdy).rgb;
    sum += w4_5 * tex2D_linearize(tex, tex_uv + (4.0 + w4_5_ratio) * dxdy).rgb;
    sum += w6_7 * tex2D_linearize(tex, tex_uv + (6.0 + w6_7_ratio) * dxdy).rgb;
    sum += w8_9 * tex2D_linearize(tex, tex_uv + (8.0 + w8_9_ratio) * dxdy).rgb;
    sum += w10_11 * tex2D_linearize(tex, tex_uv + (10.0 + w10_11_ratio) * dxdy).rgb;
    sum += w12_13 * tex2D_linearize(tex, tex_uv + (12.0 + w12_13_ratio) * dxdy).rgb;
    sum += w14_15 * tex2D_linearize(tex, tex_uv + (14.0 + w14_15_ratio) * dxdy).rgb;
    sum += w16_17 * tex2D_linearize(tex, tex_uv + (16.0 + w16_17_ratio) * dxdy).rgb;
    sum += w18_19 * tex2D_linearize(tex, tex_uv + (18.0 + w18_19_ratio) * dxdy).rgb;
    sum += w20_21 * tex2D_linearize(tex, tex_uv + (20.0 + w20_21_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur3fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 3x Gaussian blurred texture lookup using 2 linear
    //              taps.  It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * w1);
    //  Calculate combined weights and linear sample ratios between texel pairs.
    //  The center texel (with weight w0) is used twice, so halve its weight.
    float w01 = w0 * 0.5 + w1;
    float w01_ratio = w1/w01;
    //  Weights for all samples are the same, so just average them:
    return 0.5 * (
        tex2D_linearize(tex, tex_uv - w01_ratio * dxdy).rgb +
        tex2D_linearize(tex, tex_uv + w01_ratio * dxdy).rgb);
}

vec3 tex2Dblur5fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 5x Gaussian blurred texture lookup using 1 nearest
    //              neighbor and 2 linear taps.  It may be mipmapped depending
    //              on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * (w1 + w2));
    //  Calculate combined weights and linear sample ratios between texel pairs.
    float w12 = w1 + w2;
    float w12_ratio = w2/w12;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w12 * tex2D_linearize(tex, tex_uv - (1.0 + w12_ratio) * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w12 * tex2D_linearize(tex, tex_uv + (1.0 + w12_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur7fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 7x Gaussian blurred texture lookup using 4 linear
    //              taps.  It may be mipmapped depending on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * (w1 + w2 + w3));
    //  Calculate combined weights and linear sample ratios between texel pairs.
    //  The center texel (with weight w0) is used twice, so halve its weight.
    float w01 = w0 * 0.5 + w1;
    float w23 = w2 + w3;
    float w01_ratio = w1/w01;
    float w23_ratio = w3/w23;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w23 * tex2D_linearize(tex, tex_uv - (2.0 + w23_ratio) * dxdy).rgb;
    sum += w01 * tex2D_linearize(tex, tex_uv - w01_ratio * dxdy).rgb;
    sum += w01 * tex2D_linearize(tex, tex_uv + w01_ratio * dxdy).rgb;
    sum += w23 * tex2D_linearize(tex, tex_uv + (2.0 + w23_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

////////////////////  ARBITRARILY RESIZABLE ONE-PASS BLURS  ////////////////////

vec3 tex2Dblur3x3resize(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Global requirements must be met (see file description).
    //  Returns:    A 3x3 Gaussian blurred mipmapped texture lookup of the
    //              resized input.
    //  Description:
    //  This is the only arbitrarily resizable one-pass blur; tex2Dblur5x5resize
    //  would perform like tex2Dblur9x9, MUCH slower than tex2Dblur5resize.
    float denom_inv = 0.5/(sigma*sigma);
    //  Load each sample.  We need all 3x3 samples.  Quad-pixel communication
    //  won't help either: This should perform like tex2Dblur5x5, but sharing a
    //  4x4 sample field would perform more like tex2Dblur8x8shared (worse).
    vec2 sample4_uv = tex_uv;
    vec2 dx = vec2(dxdy.x, 0.0);
    vec2 dy = vec2(0.0, dxdy.y);
    vec2 sample1_uv = sample4_uv - dy;
    vec2 sample7_uv = sample4_uv + dy;
    vec3 sample0 = tex2D_linearize(tex, sample1_uv - dx).rgb;
    vec3 sample1 = tex2D_linearize(tex, sample1_uv).rgb;
    vec3 sample2 = tex2D_linearize(tex, sample1_uv + dx).rgb;
    vec3 sample3 = tex2D_linearize(tex, sample4_uv - dx).rgb;
    vec3 sample4 = tex2D_linearize(tex, sample4_uv).rgb;
    vec3 sample5 = tex2D_linearize(tex, sample4_uv + dx).rgb;
    vec3 sample6 = tex2D_linearize(tex, sample7_uv - dx).rgb;
    vec3 sample7 = tex2D_linearize(tex, sample7_uv).rgb;
    vec3 sample8 = tex2D_linearize(tex, sample7_uv + dx).rgb;
    //  Statically compute Gaussian sample weights:
    float w4 = 1.0;
    float w1_3_5_7 = exp(-LENGTH_SQ(vec2(1.0, 0.0)) * denom_inv);
    float w0_2_6_8 = exp(-LENGTH_SQ(vec2(1.0, 1.0)) * denom_inv);
    float weight_sum_inv = 1.0/(w4 + 4.0 * (w1_3_5_7 + w0_2_6_8));
    //  Weight and sum the samples:
    vec3 sum = w4 * sample4 +
        w1_3_5_7 * (sample1 + sample3 + sample5 + sample7) +
        w0_2_6_8 * (sample0 + sample2 + sample6 + sample8);
    return sum * weight_sum_inv;
}

//  Resizable one-pass blurs:
vec3 tex2Dblur3x3resize(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur3x3resize(texture, tex_uv, dxdy, blur3_std_dev);
}

vec3 tex2Dblur9fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Requires:   Same as tex2Dblur11()
    //  Returns:    A 1D 9x Gaussian blurred texture lookup using 1 nearest
    //              neighbor and 4 linear taps.  It may be mipmapped depending
    //              on settings and dxdy.
    //  First get the texel weights and normalization factor as above.
    float denom_inv = 0.5/(sigma*sigma);
    float w0 = 1.0;
    float w1 = exp(-1.0 * denom_inv);
    float w2 = exp(-4.0 * denom_inv);
    float w3 = exp(-9.0 * denom_inv);
    float w4 = exp(-16.0 * denom_inv);
    float weight_sum_inv = 1.0 / (w0 + 2.0 * (w1 + w2 + w3 + w4));
    //  Calculate combined weights and linear sample ratios between texel pairs.
    float w12 = w1 + w2;
    float w34 = w3 + w4;
    float w12_ratio = w2/w12;
    float w34_ratio = w4/w34;
    //  Statically normalize weights, sum weighted samples, and return:
    vec3 sum = vec3(0.0);
    sum += w34 * tex2D_linearize(tex, tex_uv - (3.0 + w34_ratio) * dxdy).rgb;
    sum += w12 * tex2D_linearize(tex, tex_uv - (1.0 + w12_ratio) * dxdy).rgb;
    sum += w0 * tex2D_linearize(tex, tex_uv).rgb;
    sum += w12 * tex2D_linearize(tex, tex_uv + (1.0 + w12_ratio) * dxdy).rgb;
    sum += w34 * tex2D_linearize(tex, tex_uv + (3.0 + w34_ratio) * dxdy).rgb;
    return sum * weight_sum_inv;
}

vec3 tex2Dblur9x9(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Perform a 1-pass 9x9 blur with 5x5 bilinear samples.
    //  Requires:   Same as tex2Dblur9()
    //  Returns:    A 9x9 Gaussian blurred mipmapped texture lookup composed of
    //              5x5 carefully selected bilinear samples.
    //  Description:
    //  Perform a 1-pass 9x9 blur with 5x5 bilinear samples.  Adjust the
    //  bilinear sample location to reflect the true Gaussian weights for each
    //  underlying texel.  The following diagram illustrates the relative
    //  locations of bilinear samples.  Each sample with the same number has the
    //  same weight (notice the symmetry).  The letters a, b, c, d distinguish
    //  quadrants, and the letters U, D, L, R, C (up, down, left, right, center)
    //  distinguish 1D directions along the line containing the pixel center:
    //      6a 5a 2U 5b 6b
    //      4a 3a 1U 3b 4b
    //      2L 1L 0C 1R 2R
    //      4c 3c 1D 3d 4d
    //      6c 5c 2D 5d 6d
    //  The following diagram illustrates the underlying equally spaced texels,
    //  named after the sample that accesses them and subnamed by their location
    //  within their 2x2, 2x1, 1x2, or 1x1 texel block:
    //      6a4 6a3 5a4 5a3 2U2 5b3 5b4 6b3 6b4
    //      6a2 6a1 5a2 5a1 2U1 5b1 5b2 6b1 6b2
    //      4a4 4a3 3a4 3a3 1U2 3b3 3b4 4b3 4b4
    //      4a2 4a1 3a2 3a1 1U1 3b1 3b2 4b1 4b2
    //      2L2 2L1 1L2 1L1 0C1 1R1 1R2 2R1 2R2
    //      4c2 4c1 3c2 3c1 1D1 3d1 3d2 4d1 4d2
    //      4c4 4c3 3c4 3c3 1D2 3d3 3d4 4d3 4d4
    //      6c2 6c1 5c2 5c1 2D1 5d1 5d2 6d1 6d2
    //      6c4 6c3 5c4 5c3 2D2 5d3 5d4 6d3 6d4
    //  Note there is only one C texel and only two texels for each U, D, L, or
    //  R sample.  The center sample is effectively a nearest neighbor sample,
    //  and the U/D/L/R samples use 1D linear filtering.  All other texels are
    //  read with bilinear samples somewhere within their 2x2 texel blocks.

    //  COMPUTE TEXTURE COORDS:
    //  Statically compute sampling offsets within each 2x2 texel block, based
    //  on 1D sampling ratios between texels [1, 2] and [3, 4] texels away from
    //  the center, and reuse them independently for both dimensions.  Compute
    //  these offsets based on the relative 1D Gaussian weights of the texels
    //  in question.  (w1off means "Gaussian weight for the texel 1.0 texels
    //  away from the pixel center," etc.).
    float denom_inv = 0.5/(sigma*sigma);
    float w1off = exp(-1.0 * denom_inv);
    float w2off = exp(-4.0 * denom_inv);
    float w3off = exp(-9.0 * denom_inv);
    float w4off = exp(-16.0 * denom_inv);
    float texel1to2ratio = w2off/(w1off + w2off);
    float texel3to4ratio = w4off/(w3off + w4off);
    //  Statically compute texel offsets from the fragment center to each
    //  bilinear sample in the bottom-right quadrant, including x-axis-aligned:
    vec2 sample1R_texel_offset = vec2(1.0, 0.0) + vec2(texel1to2ratio, 0.0);
    vec2 sample2R_texel_offset = vec2(3.0, 0.0) + vec2(texel3to4ratio, 0.0);
    vec2 sample3d_texel_offset = vec2(1.0, 1.0) + vec2(texel1to2ratio, texel1to2ratio);
    vec2 sample4d_texel_offset = vec2(3.0, 1.0) + vec2(texel3to4ratio, texel1to2ratio);
    vec2 sample5d_texel_offset = vec2(1.0, 3.0) + vec2(texel1to2ratio, texel3to4ratio);
    vec2 sample6d_texel_offset = vec2(3.0, 3.0) + vec2(texel3to4ratio, texel3to4ratio);

    //  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
    //  Statically compute Gaussian texel weights for the bottom-right quadrant.
    //  Read underscores as "and."
    float w1R1 = w1off;
    float w1R2 = w2off;
    float w2R1 = w3off;
    float w2R2 = w4off;
    float w3d1 =     exp(-LENGTH_SQ(vec2(1.0, 1.0)) * denom_inv);
    float w3d2_3d3 = exp(-LENGTH_SQ(vec2(2.0, 1.0)) * denom_inv);
    float w3d4 =     exp(-LENGTH_SQ(vec2(2.0, 2.0)) * denom_inv);
    float w4d1_5d1 = exp(-LENGTH_SQ(vec2(3.0, 1.0)) * denom_inv);
    float w4d2_5d3 = exp(-LENGTH_SQ(vec2(4.0, 1.0)) * denom_inv);
    float w4d3_5d2 = exp(-LENGTH_SQ(vec2(3.0, 2.0)) * denom_inv);
    float w4d4_5d4 = exp(-LENGTH_SQ(vec2(4.0, 2.0)) * denom_inv);
    float w6d1 =     exp(-LENGTH_SQ(vec2(3.0, 3.0)) * denom_inv);
    float w6d2_6d3 = exp(-LENGTH_SQ(vec2(4.0, 3.0)) * denom_inv);
    float w6d4 =     exp(-LENGTH_SQ(vec2(4.0, 4.0)) * denom_inv);
    //  Statically add texel weights in each sample to get sample weights:
    float w0 = 1.0;
    float w1 = w1R1 + w1R2;
    float w2 = w2R1 + w2R2;
    float w3 = w3d1 + 2.0 * w3d2_3d3 + w3d4;
    float w4 = w4d1_5d1 + w4d2_5d3 + w4d3_5d2 + w4d4_5d4;
    float w5 = w4;
    float w6 = w6d1 + 2.0 * w6d2_6d3 + w6d4;
    //  Get the weight sum inverse (normalization factor):
    float weight_sum_inv =
        1.0/(w0 + 4.0 * (w1 + w2 + w3 + w4 + w5 + w6));

    //  LOAD TEXTURE SAMPLES:
    //  Load all 25 samples (1 nearest, 8 linear, 16 bilinear) using symmetry:
    vec2 mirror_x = vec2(-1.0, 1.0);
    vec2 mirror_y = vec2(1.0, -1.0);
    vec2 mirror_xy = vec2(-1.0, -1.0);
    vec2 dxdy_mirror_x = dxdy * mirror_x;
    vec2 dxdy_mirror_y = dxdy * mirror_y;
    vec2 dxdy_mirror_xy = dxdy * mirror_xy;
    //  Sampling order doesn't seem to affect performance, so just be clear:
    vec3 sample0C = tex2D_linearize(tex, tex_uv).rgb;
    vec3 sample1R = tex2D_linearize(tex, tex_uv + dxdy * sample1R_texel_offset).rgb;
    vec3 sample1D = tex2D_linearize(tex, tex_uv + dxdy * sample1R_texel_offset.yx).rgb;
    vec3 sample1L = tex2D_linearize(tex, tex_uv - dxdy * sample1R_texel_offset).rgb;
    vec3 sample1U = tex2D_linearize(tex, tex_uv - dxdy * sample1R_texel_offset.yx).rgb;
    vec3 sample2R = tex2D_linearize(tex, tex_uv + dxdy * sample2R_texel_offset).rgb;
    vec3 sample2D = tex2D_linearize(tex, tex_uv + dxdy * sample2R_texel_offset.yx).rgb;
    vec3 sample2L = tex2D_linearize(tex, tex_uv - dxdy * sample2R_texel_offset).rgb;
    vec3 sample2U = tex2D_linearize(tex, tex_uv - dxdy * sample2R_texel_offset.yx).rgb;
    vec3 sample3d = tex2D_linearize(tex, tex_uv + dxdy * sample3d_texel_offset).rgb;
    vec3 sample3c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample3d_texel_offset).rgb;
    vec3 sample3b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample3d_texel_offset).rgb;
    vec3 sample3a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample3d_texel_offset).rgb;
    vec3 sample4d = tex2D_linearize(tex, tex_uv + dxdy * sample4d_texel_offset).rgb;
    vec3 sample4c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample4d_texel_offset).rgb;
    vec3 sample4b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample4d_texel_offset).rgb;
    vec3 sample4a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample4d_texel_offset).rgb;
    vec3 sample5d = tex2D_linearize(tex, tex_uv + dxdy * sample5d_texel_offset).rgb;
    vec3 sample5c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample5d_texel_offset).rgb;
    vec3 sample5b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample5d_texel_offset).rgb;
    vec3 sample5a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample5d_texel_offset).rgb;
    vec3 sample6d = tex2D_linearize(tex, tex_uv + dxdy * sample6d_texel_offset).rgb;
    vec3 sample6c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample6d_texel_offset).rgb;
    vec3 sample6b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample6d_texel_offset).rgb;
    vec3 sample6a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample6d_texel_offset).rgb;

    //  SUM WEIGHTED SAMPLES:
    //  Statically normalize weights (so total = 1.0), and sum weighted samples.
    vec3 sum = w0 * sample0C;
    sum += w1 * (sample1R + sample1D + sample1L + sample1U);
    sum += w2 * (sample2R + sample2D + sample2L + sample2U);
    sum += w3 * (sample3d + sample3c + sample3b + sample3a);
    sum += w4 * (sample4d + sample4c + sample4b + sample4a);
    sum += w5 * (sample5d + sample5c + sample5b + sample5a);
    sum += w6 * (sample6d + sample6c + sample6b + sample6a);
    return sum * weight_sum_inv;
}

vec3 tex2Dblur7x7(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Perform a 1-pass 7x7 blur with 5x5 bilinear samples.
    //  Requires:   Same as tex2Dblur9()
    //  Returns:    A 7x7 Gaussian blurred mipmapped texture lookup composed of
    //              4x4 carefully selected bilinear samples.
    //  Description:
    //  First see the descriptions for tex2Dblur9x9() and tex2Dblur7().  This
    //  blur mixes concepts from both.  The sample layout is as follows:
    //      4a 3a 3b 4b
    //      2a 1a 1b 2b
    //      2c 1c 1d 2d
    //      4c 3c 3d 4d
    //  The texel layout is as follows.  Note that samples 3a/3b, 1a/1b, 1c/1d,
    //  and 3c/3d share a vertical column of texels, and samples 2a/2c, 1a/1c,
    //  1b/1d, and 2b/2d share a horizontal row of texels (all sample1's share
    //  the center texel):
    //      4a4  4a3  3a4  3ab3 3b4  4b3  4b4
    //      4a2  4a1  3a2  3ab1 3b2  4b1  4b2
    //      2a4  2a3  1a4  1ab3 1b4  2b3  2b4
    //      2ac2 2ac1 1ac2 1*   1bd2 2bd1 2bd2
    //      2c4  2c3  1c4  1cd3 1d4  2d3  2d4
    //      4c2  4c1  3c2  3cd1 3d2  4d1  4d2
    //      4c4  4c3  3c4  3cd3 3d4  4d3  4d4

    //  COMPUTE TEXTURE COORDS:
    //  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
    float denom_inv = 0.5/(sigma*sigma);
    float w0off = 1.0;
    float w1off = exp(-1.0 * denom_inv);
    float w2off = exp(-4.0 * denom_inv);
    float w3off = exp(-9.0 * denom_inv);
    float texel0to1ratio = w1off/(w0off * 0.5 + w1off);
    float texel2to3ratio = w3off/(w2off + w3off);
    //  Statically compute texel offsets from the fragment center to each
    //  bilinear sample in the bottom-right quadrant, including axis-aligned:
    vec2 sample1d_texel_offset = vec2(texel0to1ratio, texel0to1ratio);
    vec2 sample2d_texel_offset = vec2(2.0, 0.0) + vec2(texel2to3ratio, texel0to1ratio);
    vec2 sample3d_texel_offset = vec2(0.0, 2.0) + vec2(texel0to1ratio, texel2to3ratio);
    vec2 sample4d_texel_offset = vec2(2.0, 2.0) + vec2(texel2to3ratio, texel2to3ratio);

    //  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
    //  Statically compute Gaussian texel weights for the bottom-right quadrant.
    //  Read underscores as "and."
    float w1abcd = 1.0;
    float w1bd2_1cd3 = exp(-LENGTH_SQ(vec2(1.0, 0.0)) * denom_inv);
    float w2bd1_3cd1 = exp(-LENGTH_SQ(vec2(2.0, 0.0)) * denom_inv);
    float w2bd2_3cd2 = exp(-LENGTH_SQ(vec2(3.0, 0.0)) * denom_inv);
    float w1d4 =       exp(-LENGTH_SQ(vec2(1.0, 1.0)) * denom_inv);
    float w2d3_3d2 =   exp(-LENGTH_SQ(vec2(2.0, 1.0)) * denom_inv);
    float w2d4_3d4 =   exp(-LENGTH_SQ(vec2(3.0, 1.0)) * denom_inv);
    float w4d1 =       exp(-LENGTH_SQ(vec2(2.0, 2.0)) * denom_inv);
    float w4d2_4d3 =   exp(-LENGTH_SQ(vec2(3.0, 2.0)) * denom_inv);
    float w4d4 =       exp(-LENGTH_SQ(vec2(3.0, 3.0)) * denom_inv);
    //  Statically add texel weights in each sample to get sample weights.
    //  Split weights for shared texels between samples sharing them:
    float w1 = w1abcd * 0.25 + w1bd2_1cd3 + w1d4;
    float w2_3 = (w2bd1_3cd1 + w2bd2_3cd2) * 0.5 + w2d3_3d2 + w2d4_3d4;
    float w4 = w4d1 + 2.0 * w4d2_4d3 + w4d4;
    //  Get the weight sum inverse (normalization factor):
    float weight_sum_inv =
        1.0/(4.0 * (w1 + 2.0 * w2_3 + w4));

    //  LOAD TEXTURE SAMPLES:
    //  Load all 16 samples using symmetry:
    vec2 mirror_x = vec2(-1.0, 1.0);
    vec2 mirror_y = vec2(1.0, -1.0);
    vec2 mirror_xy = vec2(-1.0, -1.0);
    vec2 dxdy_mirror_x = dxdy * mirror_x;
    vec2 dxdy_mirror_y = dxdy * mirror_y;
    vec2 dxdy_mirror_xy = dxdy * mirror_xy;
    vec3 sample1a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample1d_texel_offset).rgb;
    vec3 sample2a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample2d_texel_offset).rgb;
    vec3 sample3a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample3d_texel_offset).rgb;
    vec3 sample4a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample4d_texel_offset).rgb;
    vec3 sample1b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample1d_texel_offset).rgb;
    vec3 sample2b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample2d_texel_offset).rgb;
    vec3 sample3b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample3d_texel_offset).rgb;
    vec3 sample4b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample4d_texel_offset).rgb;
    vec3 sample1c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample1d_texel_offset).rgb;
    vec3 sample2c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample2d_texel_offset).rgb;
    vec3 sample3c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample3d_texel_offset).rgb;
    vec3 sample4c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample4d_texel_offset).rgb;
    vec3 sample1d = tex2D_linearize(tex, tex_uv + dxdy * sample1d_texel_offset).rgb;
    vec3 sample2d = tex2D_linearize(tex, tex_uv + dxdy * sample2d_texel_offset).rgb;
    vec3 sample3d = tex2D_linearize(tex, tex_uv + dxdy * sample3d_texel_offset).rgb;
    vec3 sample4d = tex2D_linearize(tex, tex_uv + dxdy * sample4d_texel_offset).rgb;

    //  SUM WEIGHTED SAMPLES:
    //  Statically normalize weights (so total = 1.0), and sum weighted samples.
    vec3 sum = vec3(0.0);
    sum += w1 * (sample1a + sample1b + sample1c + sample1d);
    sum += w2_3 * (sample2a + sample2b + sample2c + sample2d);
    sum += w2_3 * (sample3a + sample3b + sample3c + sample3d);
    sum += w4 * (sample4a + sample4b + sample4c + sample4d);
    return sum * weight_sum_inv;
}

vec3 tex2Dblur5x5(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Perform a 1-pass 5x5 blur with 3x3 bilinear samples.
    //  Requires:   Same as tex2Dblur9()
    //  Returns:    A 5x5 Gaussian blurred mipmapped texture lookup composed of
    //              3x3 carefully selected bilinear samples.
    //  Description:
    //  First see the description for tex2Dblur9x9().  This blur uses the same
    //  concept and sample/texel locations except on a smaller scale.  Samples:
    //      2a 1U 2b
    //      1L 0C 1R
    //      2c 1D 2d
    //  Texels:
    //      2a4 2a3 1U2 2b3 2b4
    //      2a2 2a1 1U1 2b1 2b2
    //      1L2 1L1 0C1 1R1 1R2
    //      2c2 2c1 1D1 2d1 2d2
    //      2c4 2c3 1D2 2d3 2d4

    //  COMPUTE TEXTURE COORDS:
    //  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
    float denom_inv = 0.5/(sigma*sigma);
    float w1off = exp(-1.0 * denom_inv);
    float w2off = exp(-4.0 * denom_inv);
    float texel1to2ratio = w2off/(w1off + w2off);
    //  Statically compute texel offsets from the fragment center to each
    //  bilinear sample in the bottom-right quadrant, including x-axis-aligned:
    vec2 sample1R_texel_offset = vec2(1.0, 0.0) + vec2(texel1to2ratio, 0.0);
    vec2 sample2d_texel_offset = vec2(1.0, 1.0) + vec2(texel1to2ratio, texel1to2ratio);

    //  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
    //  Statically compute Gaussian texel weights for the bottom-right quadrant.
    //  Read underscores as "and."
    float w1R1 = w1off;
    float w1R2 = w2off;
    float w2d1 =   exp(-LENGTH_SQ(vec2(1.0, 1.0)) * denom_inv);
    float w2d2_3 = exp(-LENGTH_SQ(vec2(2.0, 1.0)) * denom_inv);
    float w2d4 =   exp(-LENGTH_SQ(vec2(2.0, 2.0)) * denom_inv);
    //  Statically add texel weights in each sample to get sample weights:
    float w0 = 1.0;
    float w1 = w1R1 + w1R2;
    float w2 = w2d1 + 2.0 * w2d2_3 + w2d4;
    //  Get the weight sum inverse (normalization factor):
    float weight_sum_inv = 1.0/(w0 + 4.0 * (w1 + w2));

    //  LOAD TEXTURE SAMPLES:
    //  Load all 9 samples (1 nearest, 4 linear, 4 bilinear) using symmetry:
    vec2 mirror_x = vec2(-1.0, 1.0);
    vec2 mirror_y = vec2(1.0, -1.0);
    vec2 mirror_xy = vec2(-1.0, -1.0);
    vec2 dxdy_mirror_x = dxdy * mirror_x;
    vec2 dxdy_mirror_y = dxdy * mirror_y;
    vec2 dxdy_mirror_xy = dxdy * mirror_xy;
    vec3 sample0C = tex2D_linearize(tex, tex_uv).rgb;
    vec3 sample1R = tex2D_linearize(tex, tex_uv + dxdy * sample1R_texel_offset).rgb;
    vec3 sample1D = tex2D_linearize(tex, tex_uv + dxdy * sample1R_texel_offset.yx).rgb;
    vec3 sample1L = tex2D_linearize(tex, tex_uv - dxdy * sample1R_texel_offset).rgb;
    vec3 sample1U = tex2D_linearize(tex, tex_uv - dxdy * sample1R_texel_offset.yx).rgb;
    vec3 sample2d = tex2D_linearize(tex, tex_uv + dxdy * sample2d_texel_offset).rgb;
    vec3 sample2c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample2d_texel_offset).rgb;
    vec3 sample2b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample2d_texel_offset).rgb;
    vec3 sample2a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample2d_texel_offset).rgb;

    //  SUM WEIGHTED SAMPLES:
    //  Statically normalize weights (so total = 1.0), and sum weighted samples.
    vec3 sum = w0 * sample0C;
    sum += w1 * (sample1R + sample1D + sample1L + sample1U);
    sum += w2 * (sample2a + sample2b + sample2c + sample2d);
    return sum * weight_sum_inv;
}

vec3 tex2Dblur3x3(sampler2D tex, vec2 tex_uv,
    vec2 dxdy, float sigma)
{
    //  Perform a 1-pass 3x3 blur with 5x5 bilinear samples.
    //  Requires:   Same as tex2Dblur9()
    //  Returns:    A 3x3 Gaussian blurred mipmapped texture lookup composed of
    //              2x2 carefully selected bilinear samples.
    //  Description:
    //  First see the descriptions for tex2Dblur9x9() and tex2Dblur7().  This
    //  blur mixes concepts from both.  The sample layout is as follows:
    //      0a 0b
    //      0c 0d
    //  The texel layout is as follows.  Note that samples 0a/0b and 0c/0d share
    //  a vertical column of texels, and samples 0a/0c and 0b/0d share a
    //  horizontal row of texels (all samples share the center texel):
    //      0a3  0ab2 0b3
    //      0ac1 0*0  0bd1
    //      0c3  0cd2 0d3

    //  COMPUTE TEXTURE COORDS:
    //  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
    float denom_inv = 0.5/(sigma*sigma);
    float w0off = 1.0;
    float w1off = exp(-1.0 * denom_inv);
    float texel0to1ratio = w1off/(w0off * 0.5 + w1off);
    //  Statically compute texel offsets from the fragment center to each
    //  bilinear sample in the bottom-right quadrant, including axis-aligned:
    vec2 sample0d_texel_offset = vec2(texel0to1ratio, texel0to1ratio);

    //  LOAD TEXTURE SAMPLES:
    //  Load all 4 samples using symmetry:
    vec2 mirror_x = vec2(-1.0, 1.0);
    vec2 mirror_y = vec2(1.0, -1.0);
    vec2 mirror_xy = vec2(-1.0, -1.0);
    vec2 dxdy_mirror_x = dxdy * mirror_x;
    vec2 dxdy_mirror_y = dxdy * mirror_y;
    vec2 dxdy_mirror_xy = dxdy * mirror_xy;
    vec3 sample0a = tex2D_linearize(tex, tex_uv + dxdy_mirror_xy * sample0d_texel_offset).rgb;
    vec3 sample0b = tex2D_linearize(tex, tex_uv + dxdy_mirror_y * sample0d_texel_offset).rgb;
    vec3 sample0c = tex2D_linearize(tex, tex_uv + dxdy_mirror_x * sample0d_texel_offset).rgb;
    vec3 sample0d = tex2D_linearize(tex, tex_uv + dxdy * sample0d_texel_offset).rgb;

    //  SUM WEIGHTED SAMPLES:
    //  Weights for all samples are the same, so just average them:
    return 0.25 * (sample0a + sample0b + sample0c + sample0d);
}

vec3 tex2Dblur9fast(sampler2D tex, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur9fast(tex, tex_uv, dxdy, blur9_std_dev);
}

vec3 tex2Dblur17fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur17fast(texture, tex_uv, dxdy, blur17_std_dev);
}

vec3 tex2Dblur25fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur25fast(texture, tex_uv, dxdy, blur25_std_dev);
}

vec3 tex2Dblur43fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur43fast(texture, tex_uv, dxdy, blur43_std_dev);
}
vec3 tex2Dblur31fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur31fast(texture, tex_uv, dxdy, blur31_std_dev);
}

vec3 tex2Dblur3fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur3fast(texture, tex_uv, dxdy, blur3_std_dev);
}

vec3 tex2Dblur3x3(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur3x3(texture, tex_uv, dxdy, blur3_std_dev);
}

vec3 tex2Dblur5fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur5fast(texture, tex_uv, dxdy, blur5_std_dev);
}

vec3 tex2Dblur5resize(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur5resize(texture, tex_uv, dxdy, blur5_std_dev);
}
vec3 tex2Dblur3resize(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur3resize(texture, tex_uv, dxdy, blur3_std_dev);
}

vec3 tex2Dblur5x5(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur5x5(texture, tex_uv, dxdy, blur5_std_dev);
}

vec3 tex2Dblur7resize(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur7resize(texture, tex_uv, dxdy, blur7_std_dev);
}

vec3 tex2Dblur7fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur7fast(texture, tex_uv, dxdy, blur7_std_dev);
}

vec3 tex2Dblur7x7(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur7x7(texture, tex_uv, dxdy, blur7_std_dev);
}

vec3 tex2Dblur9resize(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur9resize(texture, tex_uv, dxdy, blur9_std_dev);
}

vec3 tex2Dblur9x9(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur9x9(texture, tex_uv, dxdy, blur9_std_dev);
}

vec3 tex2Dblur11resize(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur11resize(texture, tex_uv, dxdy, blur11_std_dev);
}

vec3 tex2Dblur11fast(sampler2D texture, vec2 tex_uv,
    vec2 dxdy)
{
    return tex2Dblur11fast(texture, tex_uv, dxdy, blur11_std_dev);
}

#endif  //  BLUR_FUNCTIONS_H

#define InputSize sourceSize[0].xy
#define TextureSize sourceSize[0].xy
#define OutputSize targetSize.xy

void main() {
   gl_Position = position;
   vTexCoord = texCoord;
	//  Get the uv sample distance between output pixels.  Blurs are not generic
    //  Gaussian resizers, and correct blurs require:
    //  1.) OutputSize == InputSize * 2^m, where m is an integer <= 0.
    //  2.) mipmap_inputN = "true" for this pass in the preset if m != 0
    //  3.) filter_linearN = "true" except for 1x scale nearest neighbor blurs
    //  Gaussian resizers would upsize using the distance between input texels
    //  (not output pixels), but we avoid this and consistently blur at the
    //  destination size.  Otherwise, combining statically calculated weights
    //  with bilinear sample exploitation would result in terrible artifacts.
    vec2 dxdy_scale = InputSize/OutputSize;
	vec2 dxdy = dxdy_scale/TextureSize;
    //  This blur is vertical-only, so zero out the horizontal offset:
	blur_dxdy = vec2(dxdy.x, 0.0);
}