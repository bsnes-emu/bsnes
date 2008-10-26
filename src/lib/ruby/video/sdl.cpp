#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>
#include <SDL/SDL.h>

namespace ruby {

#include "sdl.hpp"

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

  bool lock(uint32_t *&data, unsigned &pitch) {
    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    pitch = buffer->pitch;
    return data = (uint32_t*)buffer->pixels;
  }

  void unlock() {
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);
  }

  void clear() {
    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    uint32_t *data = (uint32_t*)buffer->pixels;
    for(unsigned y = 0; y < 1024; y++) {
      for(unsigned x = 0; x < 1024; x++) {
        *data++ |= 0xff000000;
      }
      data += (buffer->pitch >> 2) - 1024;
    }
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);
    refresh(1024, 1024);
  }

  void refresh(unsigned width, unsigned height) {
    //ruby input is X8R8G8B8, top 8-bits are ignored.
    //as SDL forces us to use a 32-bit buffer, we must set alpha to 255 (full opacity)
    //to prevent blending against the window beneath when X window visual is 32-bits.
    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    uint32_t *data = (uint32_t*)buffer->pixels;
    for(unsigned y = 0; y < height; y++) {
      for(unsigned x = 0; x < width; x++) {
        *data++ |= 0xff000000;
      }
      data += (buffer->pitch >> 2) - width;
    }
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);

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
    //screen depth must be 32, as 24bpp with a 32-bit X window visual produces no output.
    screen = SDL_SetVideoMode(2560, 1600, 32, SDL_HWSURFACE);
    //buffer depth must be 32, as this is the input format used by all ruby drivers.
    buffer = SDL_CreateRGBSurface(SDL_HWSURFACE,
      1024, 1024, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
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
bool VideoSDL::lock(uint32_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoSDL::unlock() { p.unlock(); }
void VideoSDL::clear() { p.clear(); }
void VideoSDL::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoSDL::init() { return p.init(); }
void VideoSDL::term() { p.term(); }
VideoSDL::VideoSDL() : p(*new pVideoSDL(*this)) {}
VideoSDL::~VideoSDL() { delete &p; }

} //namespace ruby
