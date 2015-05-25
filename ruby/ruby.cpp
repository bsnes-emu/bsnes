#include <ruby/ruby.hpp>

#undef deprecated
#undef mkdir
#undef usleep
#include <ruby/implementation.cpp>

namespace ruby {

VideoInterface video;
AudioInterface audio;
InputInterface input;

/* VideoInterface */

const string Video::Handle = "Handle";
const string Video::Synchronize = "Synchronize";
const string Video::Depth = "Depth";
const string Video::Filter = "Filter";
const string Video::Shader = "Shader";

const unsigned Video::FilterNearest = 0;
const unsigned Video::FilterLinear = 1;

auto VideoInterface::driver(string driver) -> void {
  if(p) term();

  if(!driver) driver = optimalDriver();

  if(0);

  #ifdef VIDEO_CGL
  else if(driver == "OpenGL") p = new VideoCGL();
  #endif

  #ifdef VIDEO_DIRECT3D
  else if(driver == "Direct3D") p = new VideoD3D();
  #endif

  #ifdef VIDEO_DIRECTDRAW
  else if(driver == "DirectDraw") p = new VideoDD();
  #endif

  #ifdef VIDEO_GDI
  else if(driver == "GDI") p = new VideoGDI();
  #endif

  #ifdef VIDEO_GLX
  else if(driver == "OpenGL") p = new VideoGLX();
  #endif

  #ifdef VIDEO_QTOPENGL
  else if(driver == "Qt-OpenGL") p = new VideoQtOpenGL();
  #endif

  #ifdef VIDEO_QTRASTER
  else if(driver == "Qt-Raster") p = new VideoQtRaster();
  #endif

  #ifdef VIDEO_SDL
  else if(driver == "SDL") p = new VideoSDL();
  #endif

  #ifdef VIDEO_WGL
  else if(driver == "OpenGL") p = new VideoWGL();
  #endif

  #ifdef VIDEO_XSHM
  else if(driver == "XShm") p = new VideoXShm();
  #endif

  #ifdef VIDEO_XV
  else if(driver == "X-Video") p = new VideoXv();
  #endif

  else p = new Video();
}

auto VideoInterface::optimalDriver() -> string {
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

auto VideoInterface::safestDriver() -> string {
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
  #elif defined(VIDEO_GLX)
  return "OpenGL";

  #else
  return "None";
  #endif
}

auto VideoInterface::availableDrivers() -> string {
  return

  //Windows

  #if defined(VIDEO_WGL)
  "OpenGL;"
  #endif

  #if defined(VIDEO_DIRECT3D)
  "Direct3D;"
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  "DirectDraw;"
  #endif

  #if defined(VIDEO_GDI)
  "GDI;"
  #endif

  //OS X

  #if defined(VIDEO_CGL)
  "OpenGL;"
  #endif

  //Linux

  #if defined(VIDEO_GLX)
  "OpenGL;"
  #endif

  #if defined(VIDEO_XV)
  "X-Video;"
  #endif

  #if defined(VIDEO_XSHM)
  "XShm;"
  #endif

  #if defined(VIDEO_SDL)
  "SDL;"
  #endif

  "None";
}

auto VideoInterface::init() -> bool {
  if(!p) driver();
  return p->init();
}

auto VideoInterface::term() -> void {
  if(p) {
    p->term();
    delete p;
    p = nullptr;
  }
}

VideoInterface::~VideoInterface() { term(); }
auto VideoInterface::cap(const string& name) -> bool { return p ? p->cap(name) : false; }
auto VideoInterface::get(const string& name) -> any { return p ? p->get(name) : false; }
auto VideoInterface::set(const string& name, const any& value) -> bool { return p ? p->set(name, value) : false; }
auto VideoInterface::lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool { return p ? p->lock(data, pitch, width, height) : false; }
auto VideoInterface::unlock() -> void { if(p) p->unlock(); }
auto VideoInterface::clear() -> void { if(p) p->clear(); }
auto VideoInterface::refresh() -> void { if(p) p->refresh(); }

/* AudioInterface */

const string Audio::Device = "Device";
const string Audio::Handle = "Handle";
const string Audio::Synchronize = "Synchronize";
const string Audio::Frequency = "Frequency";
const string Audio::Latency = "Latency";

auto AudioInterface::driver(string driver) -> void {
  if(p) term();

  if(!driver) driver = optimalDriver();

  if(0);

  #ifdef AUDIO_ALSA
  else if(driver == "ALSA") p = new AudioALSA();
  #endif

  #ifdef AUDIO_AO
  else if(driver == "libao") p = new AudioAO();
  #endif

  #ifdef AUDIO_DIRECTSOUND
  else if(driver == "DirectSound") p = new AudioDS();
  #endif

  #ifdef AUDIO_OPENAL
  else if(driver == "OpenAL") p = new AudioOpenAL();
  #endif

  #ifdef AUDIO_OSS
  else if(driver == "OSS") p = new AudioOSS();
  #endif

  #ifdef AUDIO_PULSEAUDIO
  else if(driver == "PulseAudio") p = new AudioPulseAudio();
  #endif

  #ifdef AUDIO_PULSEAUDIOSIMPLE
  else if(driver == "PulseAudioSimple") p = new AudioPulseAudioSimple();
  #endif

  #ifdef AUDIO_XAUDIO2
  else if(driver == "XAudio2") p = new AudioXAudio2();
  #endif

  else p = new Audio();
}

auto AudioInterface::optimalDriver() -> string {
  #if defined(AUDIO_XAUDIO2)
  return "XAudio2";
  #elif defined(AUDIO_DIRECTSOUND)
  return "DirectSound";

  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_OSS)
  return "OSS";
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

auto AudioInterface::safestDriver() -> string {
  #if defined(AUDIO_DIRECTSOUND)
  return "DirectSound";
  #elif defined(AUDIO_XAUDIO2)
  return "XAudio2";

  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_PULSEAUDIO)
  return "PulseAudio";
  #elif defined(AUDIO_PULSEAUDIOSIMPLE)
  return "PulseAudioSimple";
  #elif defined(AUDIO_AO)
  return "libao";
  #elif defined(AUDIO_OSS)
  return "OSS";

  #else
  return "None";
  #endif
}

auto AudioInterface::availableDrivers() -> string {
  return

  //Windows

  #if defined(AUDIO_XAUDIO2)
  "XAudio2;"
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  "DirectSound;"
  #endif

  //Linux

  #if defined(AUDIO_ALSA)
  "ALSA;"
  #endif

  #if defined(AUDIO_OPENAL)
  "OpenAL;"
  #endif

  #if defined(AUDIO_OSS)
  "OSS;"
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  "PulseAudio;"
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  "PulseAudioSimple;"
  #endif

  #if defined(AUDIO_AO)
  "libao;"
  #endif

  "None";
}

auto AudioInterface::init() -> bool {
  if(!p) driver();
  return p->init();
}

auto AudioInterface::term() -> void {
  if(p) {
    p->term();
    delete p;
    p = nullptr;
  }
}

AudioInterface::~AudioInterface() { term(); }
auto AudioInterface::cap(const string& name) -> bool { return p ? p->cap(name) : false; }
auto AudioInterface::get(const string& name) -> any { return p ? p->get(name) : false; }
auto AudioInterface::set(const string& name, const any& value) -> bool { return p ? p->set(name, value) : false; }
auto AudioInterface::sample(uint16_t left, uint16_t right) -> void { if(p) p->sample(left, right); }
auto AudioInterface::clear() -> void { if(p) p->clear(); }

/* InputInterface */

const string Input::Handle = "Handle";
const string Input::KeyboardSupport = "KeyboardSupport";
const string Input::MouseSupport = "MouseSupport";
const string Input::JoypadSupport = "JoypadSupport";
const string Input::JoypadRumbleSupport = "JoypadRumbleSupport";

auto InputInterface::driver(string driver) -> void {
  if(p) term();

  if(!driver) driver = optimalDriver();

  if(0);

  #ifdef INPUT_WINDOWS
  else if(driver == "Windows") p = new InputWindows();
  #endif

  #ifdef INPUT_CARBON
  else if(driver == "Carbon") p = new InputCarbon();
  #endif

  #ifdef INPUT_UDEV
  else if(driver == "udev") p = new InputUdev();
  #endif

  #ifdef INPUT_SDL
  else if(driver == "SDL") p = new InputSDL();
  #endif

  #ifdef INPUT_XLIB
  else if(driver == "Xlib") p = new InputXlib();
  #endif

  else p = new Input();
}

auto InputInterface::optimalDriver() -> string {
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

auto InputInterface::safestDriver() -> string {
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

auto InputInterface::availableDrivers() -> string {
  return

  //Windows

  #if defined(INPUT_WINDOWS)
  "Windows;"
  #endif

  //OS X

  #if defined(INPUT_CARBON)
  "Carbon;"
  #endif

  //Linux

  #if defined(INPUT_UDEV)
  "udev;"
  #endif

  #if defined(INPUT_SDL)
  "SDL;"
  #endif

  #if defined(INPUT_XLIB)
  "Xlib;"
  #endif

  "None";
}

auto InputInterface::init() -> bool {
  if(!p) driver();
  return p->init();
}

auto InputInterface::term() -> void {
  if(p) {
    p->term();
    delete p;
    p = nullptr;
  }
}

InputInterface::~InputInterface() { term(); }
auto InputInterface::cap(const string& name) -> bool { return p ? p->cap(name) : false; }
auto InputInterface::get(const string& name) -> any { return p ? p->get(name) : false; }
auto InputInterface::set(const string& name, const any& value) -> bool { return p ? p->set(name, value) : false; }
auto InputInterface::acquire() -> bool { return p ? p->acquire() : false; }
auto InputInterface::unacquire() -> bool { return p ? p->unacquire() : false; }
auto InputInterface::acquired() -> bool { return p ? p->acquired() : false; }
auto InputInterface::poll() -> vector<shared_pointer<HID::Device>> { return p ? p->poll() : vector<shared_pointer<HID::Device>>(); }
auto InputInterface::rumble(uint64_t id, bool enable) -> bool { return p ? p->rumble(id, enable) : false; }

};
