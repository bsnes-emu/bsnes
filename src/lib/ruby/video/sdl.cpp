#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>
#include <SDL/SDL.h>

#include <ruby/ruby.h>

namespace ruby {

#include "sdl.h"

class pVideoSDL {
public:
  VideoSDL &self;
  Display *display;
  SDL_Surface *screen, *buffer;

  struct {
    uintptr_t handle;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return settings.handle;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = param;
      return true;
    }
    return false;
  }

  bool lock(uint16_t *&data, unsigned &pitch) {
    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    pitch = buffer->pitch;
    return data = (uint16_t*)buffer->pixels;
  }

  void unlock() {
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);
  }

  void clear() {
  }

  void refresh(unsigned width, unsigned height) {
    XWindowAttributes attributes;
    XGetWindowAttributes(display, settings.handle, &attributes);

    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;

    dest.x = 0;
    dest.y = 0;
    dest.w = attributes.width;
    dest.h = attributes.height;

    SDL_SoftStretch(buffer, &src, screen, &dest);
    SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
  }

  bool init() {
    display = XOpenDisplay(0);
    char env[512];
    sprintf(env, "SDL_WINDOWID=%ld", settings.handle);
    putenv(env);
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(2560, 1600, 16, SDL_HWSURFACE);
    buffer = SDL_CreateRGBSurface(SDL_HWSURFACE,
      1024, 1024,
      16, 0xf800, 0x07e0, 0x001f, 0x0000
    );
    return true;
  }

  void term() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }

  pVideoSDL(VideoSDL &self_) : self(self_) {
    settings.handle = 0;
  }
};

bool VideoSDL::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoSDL::get(Setting setting) { return p.get(setting); }
bool VideoSDL::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoSDL::lock(uint16_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoSDL::unlock() { p.unlock(); }
void VideoSDL::clear() { p.clear(); }
void VideoSDL::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoSDL::init() { return p.init(); }
void VideoSDL::term() { p.term(); }
VideoSDL::VideoSDL() : p(*new pVideoSDL(*this)) {}
VideoSDL::~VideoSDL() { delete &p; }

} //namespace ruby
