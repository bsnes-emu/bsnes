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
  #define decimal CocoaDecimal
  #include <Cocoa/Cocoa.h>
  #include <Carbon/Carbon.h>
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

#if defined(VIDEO_SDL)
  #include <ruby/video/sdl.cpp>
#endif

#if defined(VIDEO_WGL)
  #include <ruby/video/wgl.cpp>
#endif

#if defined(VIDEO_XSHM)
  #include <ruby/video/xshm.cpp>
#endif

#if defined(VIDEO_XV)
  #include <ruby/video/xv.cpp>
#endif

namespace ruby {

const string Video::Handle = "Handle";
const string Video::Synchronize = "Synchronize";
const string Video::Depth = "Depth";
const string Video::Filter = "Filter";
const string Video::Shader = "Shader";

const unsigned Video::FilterNearest = 0;
const unsigned Video::FilterLinear = 1;

auto Video::create(const string& driver) -> Video* {
  if(!driver) return create(optimalDriver());

  #if defined(VIDEO_CGL)
  if(driver == "OpenGL") return new VideoCGL;
  #endif

  #if defined(VIDEO_DIRECT3D)
  if(driver == "Direct3D") return new VideoD3D;
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  if(driver == "DirectDraw") return new VideoDD;
  #endif

  #if defined(VIDEO_GDI)
  if(driver == "GDI") return new VideoGDI;
  #endif

  #if defined(VIDEO_GLX)
  if(driver == "OpenGL") return new VideoGLX;
  #endif

  #if defined(VIDEO_GLX2)
  if(driver == "OpenGL2") return new VideoGLX2;
  #endif

  #if defined(VIDEO_SDL)
  if(driver == "SDL") return new VideoSDL;
  #endif

  #if defined(VIDEO_WGL)
  if(driver == "OpenGL") return new VideoWGL;
  #endif

  #if defined(VIDEO_XSHM)
  if(driver == "XShm") return new VideoXShm;
  #endif

  #if defined(VIDEO_XV)
  if(driver == "X-Video") return new VideoXv;
  #endif

  return new Video;
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
  #elif defined(VIDEO_XV)
  return "X-Video";
  #elif defined(VIDEO_XSHM)
  return "XShm";
  #elif defined(VIDEO_SDL)
  return "SDL";
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
  #elif defined(VIDEO_SDL)
  return "SDL";
  #elif defined(VIDEO_XV)
  return "X-Video";
  #elif defined(VIDEO_GLX2)
  return "OpenGL2";
  #elif defined(VIDEO_GLX)
  return "OpenGL";
  #else
  return "None";
  #endif
}

auto Video::availableDrivers() -> lstring {
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

  #if defined(VIDEO_XV)
  "X-Video",
  #endif

  #if defined(VIDEO_XSHM)
  "XShm",
  #endif

  #if defined(VIDEO_SDL)
  "SDL",
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

#if defined(AUDIO_XAUDIO2)
  #include <ruby/audio/xaudio2.cpp>
#endif

namespace ruby {

const string Audio::Device = "Device";
const string Audio::Handle = "Handle";
const string Audio::Synchronize = "Synchronize";
const string Audio::Frequency = "Frequency";
const string Audio::Latency = "Latency";

auto Audio::create(const string& driver) -> Audio* {
  if(!driver) return create(optimalDriver());

  #if defined(AUDIO_ALSA)
  if(driver == "ALSA") return new AudioALSA;
  #endif

  #if defined(AUDIO_AO)
  if(driver == "libao") return new AudioAO;
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  if(driver == "DirectSound") return new AudioDS;
  #endif

  #if defined(AUDIO_OPENAL)
  if(driver == "OpenAL") return new AudioOpenAL;
  #endif

  #if defined(AUDIO_OSS)
  if(driver == "OSS") return new AudioOSS;
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  if(driver == "PulseAudio") return new AudioPulseAudio;
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  if(driver == "PulseAudioSimple") return new AudioPulseAudioSimple;
  #endif

  #if defined(AUDIO_XAUDIO2)
  if(driver == "XAudio2") return new AudioXAudio2;
  #endif

  return new Audio;
}

auto Audio::optimalDriver() -> string {
  #if defined(AUDIO_XAUDIO2)
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
  #else
  return "None";
  #endif
}

auto Audio::availableDrivers() -> lstring {
  return {

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

const string Input::Handle = "Handle";
const string Input::KeyboardSupport = "KeyboardSupport";
const string Input::MouseSupport = "MouseSupport";
const string Input::JoypadSupport = "JoypadSupport";
const string Input::JoypadRumbleSupport = "JoypadRumbleSupport";

auto Input::create(const string& driver) -> Input* {
  if(!driver) return create(optimalDriver());

  #if defined(INPUT_WINDOWS)
  if(driver == "Windows") return new InputWindows;
  #endif

  #if defined(INPUT_CARBON)
  if(driver == "Carbon") return new InputCarbon;
  #endif

  #if defined(INPUT_UDEV)
  if(driver == "udev") return new InputUdev;
  #endif

  #if defined(INPUT_SDL)
  if(driver == "SDL") return new InputSDL;
  #endif

  #if defined(INPUT_XLIB)
  if(driver == "Xlib") return new InputXlib;
  #endif

  return new Input;
}

auto Input::optimalDriver() -> string {
  #if defined(INPUT_WINDOWS)
  return "Windows";
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

auto Input::availableDrivers() -> lstring {
  return {

  #if defined(INPUT_WINDOWS)
  "Windows",
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
