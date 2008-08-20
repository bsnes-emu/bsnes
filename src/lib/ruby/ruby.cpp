#include <ruby/ruby.h>
#include <ruby/ruby_impl.cpp>

namespace ruby {

VideoInterface video;
AudioInterface audio;
InputInterface input;

/* VideoInterface */

void VideoInterface::driver(const char *driver) {
  if(p) term();

  if(driver == "") driver = default_driver();

  if(0);

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

  #ifdef VIDEO_SDL
  else if(!strcmp(driver, "SDL")) p = new VideoSDL();
  #endif

  #ifdef VIDEO_WGL
  else if(!strcmp(driver, "OpenGL")) p = new VideoWGL();
  #endif

  #ifdef VIDEO_XV
  else if(!strcmp(driver, "X-Video")) p = new VideoXv();
  #endif

  else p = new Video();
}

//select the *safest* available driver, not the fastest
const char* VideoInterface::default_driver() {
  #if defined(VIDEO_DIRECT3D)
  return "Direct3D";
  #elif defined(VIDEO_WGL)
  return "OpenGL";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw";
  #elif defined(VIDEO_GDI)
  return "GDI";
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

//returns list of available drivers, sorted from most to least optimal
const char* VideoInterface::driver_list() {
  return

  //Windows

  #if defined(VIDEO_DIRECT3D)
  "Direct3D;"
  #endif

  #if defined(VIDEO_WGL)
  "OpenGL;"
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  "DirectDraw;"
  #endif

  #if defined(VIDEO_GDI)
  "GDI;"
  #endif

  //Linux

  #if defined(VIDEO_GLX)
  "OpenGL;"
  #endif

  #if defined(VIDEO_XV)
  "X-Video;"
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
    delete p;
    p = 0;
  }
}

bool VideoInterface::cap(Video::Setting setting) { return p ? p->cap(setting) : false; }
uintptr_t VideoInterface::get(Video::Setting setting) { return p ? p->get(setting) : false; }
bool VideoInterface::set(Video::Setting setting, uintptr_t param) { return p ? p->set(setting, param) : false; }
bool VideoInterface::lock(uint32_t *&data, unsigned &pitch) { return p ? p->lock(data, pitch) : false; }
void VideoInterface::unlock() { if(p) p->unlock(); }
void VideoInterface::clear() { if(p) p->clear(); }
void VideoInterface::refresh(unsigned width, unsigned height) { if(p) p->refresh(width, height); }
VideoInterface::VideoInterface() : p(0) {}
VideoInterface::~VideoInterface() { term(); }

/* AudioInterface */

void AudioInterface::driver(const char *driver) {
  if(p) term();

  if(driver == "") driver = default_driver();

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

  else p = new Audio();
}

//select the *safest* available driver, not the fastest
const char* AudioInterface::default_driver() {
  #if defined(AUDIO_DIRECTSOUND)
  return "DirectSound";
  #elif defined(AUDIO_AO)
  return "libao";
  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_OSS)
  return "OSS";
  #else
  return "None";
  #endif
}

//returns list of available drivers, sorted from most to least optimal
const char* AudioInterface::driver_list() {
  return

  //Windows

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

  #if defined(AUDIO_AO)
  "libao;"
  #endif

  "None";
}

#include "ruby_audio.cpp"

/* InputInterface */

void InputInterface::driver(const char *driver) {
  if(p) term();

  if(driver == "") driver = default_driver();

  if(0);

  #ifdef INPUT_DIRECTINPUT
  else if(!strcmp(driver, "DirectInput")) p = new InputDI();
  #endif

  #ifdef INPUT_SDL
  else if(!strcmp(driver, "SDL")) p = new InputSDL();
  #endif

  #ifdef INPUT_X
  else if(!strcmp(driver, "X-Windows")) p = new InputX();
  #endif

  else p = new Input();
}

//select the *safest* available driver, not the fastest
const char* InputInterface::default_driver() {
  #if defined(INPUT_DIRECTINPUT)
  return "DirectInput";
  #elif defined(INPUT_SDL)
  return "SDL";
  #elif defined(INPUT_X)
  return "X-Windows";
  #else
  return "none";
  #endif
}

const char* InputInterface::driver_list() {
  return

  //Windows

  #if defined(INPUT_DIRECTINPUT)
  "DirectInput;"
  #endif

  //Linux

  #if defined(INPUT_SDL)
  "SDL;"
  #endif

  #if defined(INPUT_X)
  "X-Windows;"
  #endif

  "None";
}

bool InputInterface::init() {
  if(!p) driver();
  return p->init();
}

void InputInterface::term() {
  if(p) {
    delete p;
    p = 0;
  }
}

bool InputInterface::cap(Input::Setting setting) { return p ? p->cap(setting) : false; }
uintptr_t InputInterface::get(Input::Setting setting) { return p ? p->get(setting) : false; }
bool InputInterface::set(Input::Setting setting, uintptr_t param) { return p ? p->set(setting, param) : false; }
bool InputInterface::key_down(uint16_t key) { return p ? p->key_down(key) : false; }
bool InputInterface::key_up(uint16_t key) { return p ? p->key_up(key) : true; }
void InputInterface::clear() { if(p) p->clear(); }
void InputInterface::poll() { if(p) p->poll(); }
InputInterface::InputInterface() : p(0) {}
InputInterface::~InputInterface() { term(); }

} //namespace ruby
