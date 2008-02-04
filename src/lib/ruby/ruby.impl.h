/* Video */

#ifdef VIDEO_DIRECT3D
  #include <ruby/video/direct3d.h>
#endif

#ifdef VIDEO_DIRECTDRAW
  #include <ruby/video/directdraw.h>
#endif

#ifdef VIDEO_GDI
  #include <ruby/video/gdi.h>
#endif

#ifdef VIDEO_GLX
  #include <ruby/video/glx.h>
#endif

#ifdef VIDEO_SDL
  #include <ruby/video/sdl.h>
#endif

#ifdef VIDEO_XV
  #include <ruby/video/xv.h>
#endif

/* Audio */

#ifdef AUDIO_AO
  #include <ruby/audio/ao.h>
#endif

#ifdef AUDIO_DIRECTSOUND
  #include <ruby/audio/directsound.h>
#endif

#ifdef AUDIO_OPENAL
  #include <ruby/audio/openal.h>
#endif

#ifdef AUDIO_OSS
  #include <ruby/audio/oss.h>
#endif

/* Input */

#ifdef INPUT_DIRECTINPUT
  #include <ruby/input/directinput.h>
#endif

#ifdef INPUT_SDL
  #include <ruby/input/sdl.h>
#endif

#ifdef INPUT_X
  #include <ruby/input/x.h>
#endif
