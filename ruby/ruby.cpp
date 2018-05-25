#ifdef _WIN32
  #include <initguid.h>
  #include <cguid.h>
#endif

#include <ruby/ruby.hpp>
using namespace nall;
using namespace ruby;

/* Shared */

#undef deprecated
#undef mkdir
#undef usleep

#if defined(DISPLAY_XORG)
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #include <X11/Xatom.h>
#elif defined(DISPLAY_QUARTZ)
  #define Boolean CocoaBoolean
  #define decimal CocoaDecimal
  #include <Cocoa/Cocoa.h>
  #include <Carbon/Carbon.h>
  #undef Boolean
  #undef decimal
#elif defined(DISPLAY_WINDOWS)
  #include <windows.h>
#endif

/* Video */

#if defined(VIDEO_CGL)
  #include <ruby/video/cgl.cpp>
#endif

#if defined(VIDEO_DIRECT3D)
  #include <ruby/video/direct3d.cpp>
#endif

#if defined(VIDEO_DIRECTDRAW)
  #include <ruby/video/directdraw.cpp>
#endif

#if defined(VIDEO_GDI)
  #include <ruby/video/gdi.cpp>
#endif

#if defined(VIDEO_GLX)
  #include <ruby/video/glx.cpp>
#endif

#if defined(VIDEO_GLX2)
  #include <ruby/video/glx2.cpp>
#endif

#if defined(VIDEO_WGL)
  #include <ruby/video/wgl.cpp>
#endif

#if defined(VIDEO_XSHM)
  #include <ruby/video/xshm.cpp>
#endif

#if defined(VIDEO_XVIDEO)
  #include <ruby/video/xvideo.cpp>
#endif

namespace ruby {

auto Video::create(string driver) -> Video* {
  Video* video = nullptr;
  if(!driver) driver = optimalDriver();

  #if defined(VIDEO_CGL)
  if(driver == "OpenGL") video = new VideoCGL;
  #endif

  #if defined(VIDEO_DIRECT3D)
  if(driver == "Direct3D") video = new VideoDirect3D;
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  if(driver == "DirectDraw") video = new VideoDirectDraw;
  #endif

  #if defined(VIDEO_GDI)
  if(driver == "GDI") video = new VideoGDI;
  #endif

  #if defined(VIDEO_GLX)
  if(driver == "OpenGL") video = new VideoGLX;
  #endif

  #if defined(VIDEO_GLX2)
  if(driver == "OpenGL2") video = new VideoGLX2;
  #endif

  #if defined(VIDEO_WGL)
  if(driver == "OpenGL") video = new VideoWGL;
  #endif

  #if defined(VIDEO_XSHM)
  if(driver == "XShm") video = new VideoXShm;
  #endif

  #if defined(VIDEO_XVIDEO)
  if(driver == "XVideo") video = new VideoXVideo;
  #endif

  if(!video) driver = "None", video = new Video;
  video->_driver = driver;
  return video;
}

auto Video::optimalDriver() -> string {
  #if defined(VIDEO_WGL)
  return "OpenGL";
  #elif defined(VIDEO_DIRECT3D)
  return "Direct3D";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw";
  #elif defined(VIDEO_GDI)
  return "GDI";
  #elif defined(VIDEO_CGL)
  return "OpenGL";
  #elif defined(VIDEO_GLX)
  return "OpenGL";
  #elif defined(VIDEO_GLX2)
  return "OpenGL2";
  #elif defined(VIDEO_XVIDEO)
  return "XVideo";
  #elif defined(VIDEO_XSHM)
  return "XShm";
  #else
  return "None";
  #endif
}

auto Video::safestDriver() -> string {
  #if defined(VIDEO_DIRECT3D)
  return "Direct3D";
  #elif defined(VIDEO_WGL)
  return "OpenGL";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw";
  #elif defined(VIDEO_GDI)
  return "GDI";
  #elif defined(VIDEO_CGL)
  return "OpenGL";
  #elif defined(VIDEO_XSHM)
  return "XShm";
  #elif defined(VIDEO_XVIDEO)
  return "XVideo";
  #elif defined(VIDEO_GLX2)
  return "OpenGL2";
  #elif defined(VIDEO_GLX)
  return "OpenGL";
  #else
  return "None";
  #endif
}

auto Video::availableDrivers() -> string_vector {
  return {

  #if defined(VIDEO_WGL)
  "OpenGL",
  #endif

  #if defined(VIDEO_DIRECT3D)
  "Direct3D",
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  "DirectDraw",
  #endif

  #if defined(VIDEO_GDI)
  "GDI",
  #endif

  #if defined(VIDEO_CGL)
  "OpenGL",
  #endif

  #if defined(VIDEO_GLX)
  "OpenGL",
  #endif

  #if defined(VIDEO_GLX2)
  "OpenGL2",
  #endif

  #if defined(VIDEO_XVIDEO)
  "XVideo",
  #endif

  #if defined(VIDEO_XSHM)
  "XShm",
  #endif

  "None"};
}

}

/* Audio */

#if defined(AUDIO_ALSA)
  #include <ruby/audio/alsa.cpp>
#endif

#if defined(AUDIO_AO)
  #include <ruby/audio/ao.cpp>
#endif

#if defined(AUDIO_ASIO)
  #include <ruby/audio/asio.cpp>
#endif

#if defined(AUDIO_DIRECTSOUND)
  #include <ruby/audio/directsound.cpp>
#endif

#if defined(AUDIO_OPENAL)
  #include <ruby/audio/openal.cpp>
#endif

#if defined(AUDIO_OSS)
  #include <ruby/audio/oss.cpp>
#endif

#if defined(AUDIO_PULSEAUDIO)
  #include <ruby/audio/pulseaudio.cpp>
#endif

#if defined(AUDIO_PULSEAUDIOSIMPLE)
  #include <ruby/audio/pulseaudiosimple.cpp>
#endif

#if defined(AUDIO_WASAPI)
  #include <ruby/audio/wasapi.cpp>
#endif

#if defined(AUDIO_XAUDIO2)
  #include <ruby/audio/xaudio2.cpp>
#endif

namespace ruby {

auto Audio::create(string driver) -> Audio* {
  Audio* audio = nullptr;
  if(!driver) driver = optimalDriver();

  #if defined(AUDIO_ALSA)
  if(driver == "ALSA") audio = new AudioALSA;
  #endif

  #if defined(AUDIO_AO)
  if(driver == "libao") audio = new AudioAO;
  #endif

  #if defined(AUDIO_ASIO)
  if(driver == "ASIO") audio = new AudioASIO;
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  if(driver == "DirectSound") audio = new AudioDirectSound;
  #endif

  #if defined(AUDIO_OPENAL)
  if(driver == "OpenAL") audio = new AudioOpenAL;
  #endif

  #if defined(AUDIO_OSS)
  if(driver == "OSS") audio = new AudioOSS;
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  if(driver == "PulseAudio") audio = new AudioPulseAudio;
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  if(driver == "PulseAudioSimple") audio = new AudioPulseAudioSimple;
  #endif

  #if defined(AUDIO_WASAPI)
  if(driver == "WASAPI") audio = new AudioWASAPI;
  #endif

  #if defined(AUDIO_XAUDIO2)
  if(driver == "XAudio2") audio = new AudioXAudio2;
  #endif

  if(!audio) driver = "None", audio = new Audio;
  audio->_driver = driver;
  return audio;
}

auto Audio::optimalDriver() -> string {
  #if defined(AUDIO_ASIO)
  return "ASIO";
  #elif defined(AUDIO_WASAPI)
  return "WASAPI";
  #elif defined(AUDIO_XAUDIO2)
  return "XAudio2";
  #elif defined(AUDIO_DIRECTSOUND)
  return "DirectSound";
  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OSS)
  return "OSS";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_PULSEAUDIO)
  return "PulseAudio";
  #elif defined(AUDIO_PULSEAUDIOSIMPLE)
  return "PulseAudioSimple";
  #elif defined(AUDIO_AO)
  return "libao";
  #else
  return "None";
  #endif
}

auto Audio::safestDriver() -> string {
  #if defined(AUDIO_DIRECTSOUND)
  return "DirectSound";
  #elif defined(AUDIO_WASAPI)
  return "WASAPI";
  #elif defined(AUDIO_XAUDIO2)
  return "XAudio2";
  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OSS)
  return "OSS";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_PULSEAUDIO)
  return "PulseAudio";
  #elif defined(AUDIO_PULSEAUDIOSIMPLE)
  return "PulseAudioSimple";
  #elif defined(AUDIO_AO)
  return "libao";
  #elif defined(AUDIO_ASIO)
  return "ASIO";
  #else
  return "None";
  #endif
}

auto Audio::availableDrivers() -> string_vector {
  return {

  #if defined(AUDIO_ASIO)
  "ASIO",
  #endif

  #if defined(AUDIO_WASAPI)
  "WASAPI",
  #endif

  #if defined(AUDIO_XAUDIO2)
  "XAudio2",
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  "DirectSound",
  #endif

  #if defined(AUDIO_ALSA)
  "ALSA",
  #endif

  #if defined(AUDIO_OSS)
  "OSS",
  #endif

  #if defined(AUDIO_OPENAL)
  "OpenAL",
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  "PulseAudio",
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  "PulseAudioSimple",
  #endif

  #if defined(AUDIO_AO)
  "libao",
  #endif

  "None"};
}

}

/* Input */

#if defined(INPUT_CARBON)
  #include <ruby/input/carbon.cpp>
#endif

#if defined(INPUT_QUARTZ)
  #include <ruby/input/quartz.cpp>
#endif

#if defined(INPUT_SDL)
  #include <ruby/input/sdl.cpp>
#endif

#if defined(INPUT_UDEV)
  #include <ruby/input/udev.cpp>
#endif

#if defined(INPUT_WINDOWS)
  #include <ruby/input/windows.cpp>
#endif

#if defined(INPUT_XLIB)
  #include <ruby/input/xlib.cpp>
#endif

namespace ruby {

auto Input::create(string driver) -> Input* {
  Input* input = nullptr;
  if(!driver) driver = optimalDriver();

  #if defined(INPUT_WINDOWS)
  if(driver == "Windows") input = new InputWindows;
  #endif

  #if defined(INPUT_QUARTZ)
  if(driver == "Quartz") input = new InputQuartz;
  #endif

  #if defined(INPUT_CARBON)
  if(driver == "Carbon") input = new InputCarbon;
  #endif

  #if defined(INPUT_UDEV)
  if(driver == "udev") input = new InputUdev;
  #endif

  #if defined(INPUT_SDL)
  if(driver == "SDL") input = new InputSDL;
  #endif

  #if defined(INPUT_XLIB)
  if(driver == "Xlib") input = new InputXlib;
  #endif

  if(!input) driver = "None", input = new Input;
  input->_driver = driver;
  return input;
}

auto Input::optimalDriver() -> string {
  #if defined(INPUT_WINDOWS)
  return "Windows";
  #elif defined(INPUT_QUARTZ)
  return "Quartz";
  #elif defined(INPUT_CARBON)
  return "Carbon";
  #elif defined(INPUT_UDEV)
  return "udev";
  #elif defined(INPUT_SDL)
  return "SDL";
  #elif defined(INPUT_XLIB)
  return "Xlib";
  #else
  return "None";
  #endif
}

auto Input::safestDriver() -> string {
  #if defined(INPUT_WINDOWS)
  return "Windows";
  #elif defined(INPUT_QUARTZ)
  return "Quartz";
  #elif defined(INPUT_CARBON)
  return "Carbon";
  #elif defined(INPUT_UDEV)
  return "udev";
  #elif defined(INPUT_SDL)
  return "SDL";
  #elif defined(INPUT_XLIB)
  return "Xlib";
  #else
  return "none";
  #endif
}

auto Input::availableDrivers() -> string_vector {
  return {

  #if defined(INPUT_WINDOWS)
  "Windows",
  #endif

  #if defined(INPUT_QUARTZ)
  "Quartz",
  #endif

  #if defined(INPUT_CARBON)
  "Carbon",
  #endif

  #if defined(INPUT_UDEV)
  "udev",
  #endif

  #if defined(INPUT_SDL)
  "SDL",
  #endif

  #if defined(INPUT_XLIB)
  "Xlib",
  #endif

  "None"};
}

}
