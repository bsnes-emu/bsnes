/* Video */

#ifdef _WIN32
  #define _WIN32_WINNT 0x0501
  #include <windows.h>
#endif

#ifdef VIDEO_DIRECT3D
  #include <ruby/video/direct3d.cpp>
#endif

#ifdef VIDEO_DIRECTDRAW
  #include <ruby/video/directdraw.cpp>
#endif

#ifdef VIDEO_GDI
  #include <ruby/video/gdi.cpp>
#endif

#ifdef VIDEO_GLX
  #include <ruby/video/glx.cpp>
#endif

#ifdef VIDEO_SDL
  #include <ruby/video/sdl.cpp>
#endif

#ifdef VIDEO_WGL
  #include <ruby/video/wgl.cpp>
#endif

#ifdef VIDEO_XV
  #include <ruby/video/xv.cpp>
#endif

/* Audio */

#ifdef AUDIO_ALSA
  #include <ruby/audio/alsa.cpp>
#endif

#ifdef AUDIO_AO
  #include <ruby/audio/ao.cpp>
#endif

#ifdef AUDIO_DIRECTSOUND
  #include <ruby/audio/directsound.cpp>
#endif

#ifdef AUDIO_OPENAL
  #include <ruby/audio/openal.cpp>
#endif

#ifdef AUDIO_OSS
  #include <ruby/audio/oss.cpp>
#endif

#ifdef AUDIO_PULSEAUDIO
  #include <ruby/audio/pulseaudio.cpp>
#endif

/* Input */

#ifdef INPUT_DIRECTINPUT
  #include <ruby/input/directinput.cpp>
#endif

#ifdef INPUT_RAWINPUT
  #include <ruby/input/rawinput.cpp>
#endif

#ifdef INPUT_SDL
  #include <ruby/input/sdl.cpp>
#endif

#ifdef INPUT_X
  #include <ruby/input/x.cpp>
#endif
