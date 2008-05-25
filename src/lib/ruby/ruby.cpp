#include <ruby/ruby.h>
#include <ruby/ruby_impl.cpp>

namespace ruby {

VideoInterface video;
AudioInterface audio;
InputInterface input;

/* VideoInterface */

void VideoInterface::driver(const char *driver) {
  if(p) term();

  if(!strcmp(driver, "none")) p = new Video();

  #ifdef VIDEO_DIRECT3D
  else if(!strcmp(driver, "direct3d")) p = new VideoD3D();
  #endif

  #ifdef VIDEO_DIRECTDRAW
  else if(!strcmp(driver, "directdraw")) p = new VideoDD();
  #endif

  #ifdef VIDEO_GDI
  else if(!strcmp(driver, "gdi")) p = new VideoGDI();
  #endif

  #ifdef VIDEO_GLX
  else if(!strcmp(driver, "glx")) p = new VideoGLX();
  #endif

  #ifdef VIDEO_SDL
  else if(!strcmp(driver, "sdl")) p = new VideoSDL();
  #endif

  #ifdef VIDEO_WGL
  else if(!strcmp(driver, "wgl")) p = new VideoWGL();
  #endif

  #ifdef VIDEO_XV
  else if(!strcmp(driver, "xv")) p = new VideoXv();
  #endif

  else //select the *safest* available driver, not the fastest

  #if defined(VIDEO_DIRECT3D)
  p = new VideoD3D();
  #elif defined(VIDEO_WGL)
  p = new VideoWGL();
  #elif defined(VIDEO_DIRECTDRAW)
  p = new VideoDD();
  #elif defined(VIDEO_GDI)
  p = new VideoGDI();
  #elif defined(VIDEO_SDL)
  p = new VideoSDL();
  #elif defined(VIDEO_XV)
  p = new VideoXv();
  #elif defined(VIDEO_GLX)
  p = new VideoGLX();
  #else
  p = new Video();
  #endif
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

  if(!strcmp(driver, "none")) p = new Audio();

  #ifdef AUDIO_ALSA
  else if(!strcmp(driver, "alsa")) p = new AudioALSA();
  #endif

  #ifdef AUDIO_AO
  else if(!strcmp(driver, "ao")) p = new AudioAO();
  #endif

  #ifdef AUDIO_DIRECTSOUND
  else if(!strcmp(driver, "directsound")) p = new AudioDS();
  #endif

  #ifdef AUDIO_OPENAL
  else if(!strcmp(driver, "openal")) p = new AudioOpenAL();
  #endif

  #ifdef AUDIO_OSS
  else if(!strcmp(driver, "oss")) p = new AudioOSS();
  #endif

  else //select the *safest* available driver, not the fastest

  #if defined(AUDIO_DIRECTSOUND)
  p = new AudioDS();
  #elif defined(AUDIO_AO)
  p = new AudioAO();
  #elif defined(AUDIO_ALSA)
  p = new AudioALSA();
  #elif defined(AUDIO_OPENAL)
  p = new AudioOpenAL();
  #elif defined(AUDIO_OSS)
  p = new AudioOSS();
  #else
  p = new Audio();
  #endif
}

bool AudioInterface::init() {
  if(!p) driver();
  return p->init();
}

void AudioInterface::term() {
  if(p) {
    delete p;
    p = 0;
  }
}

bool AudioInterface::cap(Audio::Setting setting) { return p ? p->cap(setting) : false; }
uintptr_t AudioInterface::get(Audio::Setting setting) { return p ? p->get(setting) : false; }
bool AudioInterface::set(Audio::Setting setting, uintptr_t param) { return p ? p->set(setting, param) : false; }
void AudioInterface::sample(uint16_t left, uint16_t right) { if(p) p->sample(left, right); }
void AudioInterface::clear() { if(p) p->clear(); }
AudioInterface::AudioInterface() : p(0) {}
AudioInterface::~AudioInterface() { term(); }

/* InputInterface */

void InputInterface::driver(const char *driver) {
  if(p) term();

  if(!strcmp(driver, "none")) p = new Input();

  #ifdef INPUT_DIRECTINPUT
  else if(!strcmp(driver, "directinput")) p = new InputDI();
  #endif

  #ifdef INPUT_SDL
  else if(!strcmp(driver, "sdl")) p = new InputSDL();
  #endif

  #ifdef INPUT_X
  else if(!strcmp(driver, "x")) p = new InputX();
  #endif

  else //select the *safest* available driver, not the fastest

  #if defined(INPUT_DIRECTINPUT)
  p = new InputDI();
  #elif defined(INPUT_SDL)
  p = new InputSDL();
  #elif defined(INPUT_X)
  p = new InputX();
  #else
  p = new Input();
  #endif
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
