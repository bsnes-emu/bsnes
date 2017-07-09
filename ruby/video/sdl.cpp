#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>
#include <SDL/SDL.h>

struct VideoSDL : Video {
  ~VideoSDL() { term(); }

  Display* display = nullptr;
  SDL_Surface* screen = nullptr;
  SDL_Surface* buffer = nullptr;
  uint iwidth = 0;
  uint iheight = 0;

  struct {
    uintptr handle = 0;

    uint width = 0;
    uint height = 0;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return settings.handle;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr>()) {
      settings.handle = value.get<uintptr>();
      return true;
    }

    return false;
  }

  auto resize(uint width, uint height) -> void {
    if(iwidth >= width && iheight >= height) return;

    iwidth  = max(width,  iwidth);
    iheight = max(height, iheight);

    if(buffer) SDL_FreeSurface(buffer);
    buffer = SDL_CreateRGBSurface(
      SDL_SWSURFACE, iwidth, iheight, 32,
      0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
    );
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(width != settings.width || height != settings.height) {
      resize(settings.width = width, settings.height = height);
    }

    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    pitch = buffer->pitch;
    return data = (uint32_t*)buffer->pixels;
  }

  auto unlock() -> void {
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);
  }

  auto clear() -> void {
    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    for(uint y : range(iheight)) {
      uint32_t* data = (uint32_t*)buffer->pixels + y * (buffer->pitch >> 2);
      for(uint x : range(iwidth)) *data++ = 0xff000000;
    }
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);
    refresh();
  }

  auto refresh() -> void {
    //ruby input is X8R8G8B8, top 8-bits are ignored.
    //as SDL forces us to use a 32-bit buffer, we must set alpha to 255 (full opacity)
    //to prevent blending against the window beneath when X window visual is 32-bits.
    if(SDL_MUSTLOCK(buffer)) SDL_LockSurface(buffer);
    for(uint y : range(settings.height)) {
      uint32_t* data = (uint32_t*)buffer->pixels + y * (buffer->pitch >> 2);
      for(uint x : range(settings.width)) *data++ |= 0xff000000;
    }
    if(SDL_MUSTLOCK(buffer)) SDL_UnlockSurface(buffer);

    XWindowAttributes attributes;
    XGetWindowAttributes(display, settings.handle, &attributes);

    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = settings.width;
    src.h = settings.height;

    dest.x = 0;
    dest.y = 0;
    dest.w = attributes.width;
    dest.h = attributes.height;

    SDL_SoftStretch(buffer, &src, screen, &dest);
    SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
  }

  auto init() -> bool {
    display = XOpenDisplay(0);

    //todo: this causes a segfault inside SDL_SetVideoMode on FreeBSD (works under Linux)
    char env[512];
    sprintf(env, "SDL_WINDOWID=%ld", (long)settings.handle);
    putenv(env);

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(2560, 1600, 32, SDL_HWSURFACE);
    XUndefineCursor(display, settings.handle);

    buffer = nullptr;
    iwidth = 0;
    iheight = 0;
    resize(settings.width = 256, settings.height = 256);

    return true;
  }

  auto term() -> void {
    XCloseDisplay(display);
    SDL_FreeSurface(buffer);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
};
