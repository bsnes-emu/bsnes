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

const char* Video::Handle = "Handle";
const char* Video::Synchronize = "Synchronize";
const char* Video::Depth = "Depth";
const char* Video::Filter = "Filter";
const char* Video::Shader = "Shader";

const unsigned Video::FilterNearest = 0;
const unsigned Video::FilterLinear  = 1;

void VideoInterface::driver(const char* driver) {
  if(p) term();

  if(!driver || !*driver) driver = optimalDriver();

  if(0);

  #ifdef VIDEO_CGL
  else if(!strcmp(driver, "OpenGL")) p = new VideoCGL();
  #endif

  #ifdef VIDEO_DIRECT3D
  else if(!strcmp(driver, "Direct3D")) p = new VideoD3D();
  #endif

  #ifdef VIDEO_DIRECTDRAW
  else if(!strcmp(driver, "DirectDraw")) p = new VideoDD();
  #endif

  #ifdef VIDEO_GDI
  else if(!strcmp(driver, "GDI")) p = new VideoGDI();
  #endif

  #ifdef VIDEO_GLX
  else if(!strcmp(driver, "OpenGL")) p = new VideoGLX();
  #endif

  #ifdef VIDEO_QTOPENGL
  else if(!strcmp(driver, "Qt-OpenGL")) p = new VideoQtOpenGL();
  #endif

  #ifdef VIDEO_QTRASTER
  else if(!strcmp(driver, "Qt-Raster")) p = new VideoQtRaster();
  #endif

  #ifdef VIDEO_SDL
  else if(!strcmp(driver, "SDL")) p = new VideoSDL();
  #endif

  #ifdef VIDEO_WGL
  else if(!strcmp(driver, "OpenGL")) p = new VideoWGL();
  #endif

  #ifdef VIDEO_XSHM
  else if(!strcmp(driver, "XShm")) p = new VideoXShm();
  #endif

  #ifdef VIDEO_XV
  else if(!strcmp(driver, "X-Video")) p = new VideoXv();
  #endif

  else p = new Video();
}

const char* VideoInterface::optimalDriver() {
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

const char* VideoInterface::safestDriver() {
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

const char* VideoInterface::availableDrivers() {
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

bool VideoInterface::init() {
  if(!p) driver();
  return p->init();
}

void VideoInterface::term() {
  if(p) {
    p->term();
    delete p;
    p = nullptr;
  }
}

bool VideoInterface::cap(const string& name) { return p ? p->cap(name) : false; }
any VideoInterface::get(const string& name) { return p ? p->get(name) : false; }
bool VideoInterface::set(const string& name, const any& value) { return p ? p->set(name, value) : false; }
bool VideoInterface::lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) { return p ? p->lock(data, pitch, width, height) : false; }
void VideoInterface::unlock() { if(p) p->unlock(); }
void VideoInterface::clear() { if(p) p->clear(); }
void VideoInterface::refresh() { if(p) p->refresh(); }
VideoInterface::VideoInterface() : p(nullptr) {}
VideoInterface::~VideoInterface() { term(); }

/* AudioInterface */

const char* Audio::Handle = "Handle";
const char* Audio::Synchronize = "Synchronize";
const char* Audio::Frequency = "Frequency";
const char* Audio::Latency = "Latency";

void AudioInterface::driver(const char* driver) {
  if(p) term();

  if(!driver || !*driver) driver = optimalDriver();

  if(0);

  #ifdef AUDIO_ALSA
  else if(!strcmp(driver, "ALSA")) p = new AudioALSA();
  #endif

  #ifdef AUDIO_AO
  else if(!strcmp(driver, "libao")) p = new AudioAO();
  #endif

  #ifdef AUDIO_DIRECTSOUND
  else if(!strcmp(driver, "DirectSound")) p = new AudioDS();
  #endif

  #ifdef AUDIO_OPENAL
  else if(!strcmp(driver, "OpenAL")) p = new AudioOpenAL();
  #endif

  #ifdef AUDIO_OSS
  else if(!strcmp(driver, "OSS")) p = new AudioOSS();
  #endif

  #ifdef AUDIO_PULSEAUDIO
  else if(!strcmp(driver, "PulseAudio")) p = new AudioPulseAudio();
  #endif

  #ifdef AUDIO_PULSEAUDIOSIMPLE
  else if(!strcmp(driver, "PulseAudioSimple")) p = new AudioPulseAudioSimple();
  #endif

  #ifdef AUDIO_XAUDIO2
  else if(!strcmp(driver, "XAudio2")) p = new AudioXAudio2();
  #endif

  else p = new Audio();
}

const char* AudioInterface::optimalDriver() {
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

const char* AudioInterface::safestDriver() {
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

const char* AudioInterface::availableDrivers() {
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

bool AudioInterface::init() {
  if(!p) driver();
  return p->init();
}

void AudioInterface::term() {
  if(p) {
    p->term();
    delete p;
    p = nullptr;
  }
}

bool AudioInterface::cap(const string& name) { return p ? p->cap(name) : false; }
any AudioInterface::get(const string& name) { return p ? p->get(name) : false; }
bool AudioInterface::set(const string& name, const any& value) { return p ? p->set(name, value) : false; }
void AudioInterface::sample(uint16_t left, uint16_t right) { if(p) p->sample(left, right); }
void AudioInterface::clear() { if(p) p->clear(); }
AudioInterface::AudioInterface() : p(nullptr) {}
AudioInterface::~AudioInterface() { term(); }

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
