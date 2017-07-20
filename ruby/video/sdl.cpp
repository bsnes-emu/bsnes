#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>
#include <SDL/SDL.h>

struct VideoSDL : Video {
  VideoSDL() { initialize(); }
  ~VideoSDL() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto context() -> uintptr { return _context; }

  auto setContext(uintptr context) -> bool {
    if(_context == context) return true;
    _context = context;
    return initialize();
  }

  auto clear() -> void {
    if(SDL_MUSTLOCK(_buffer)) SDL_LockSurface(_buffer);
    for(uint y : range(_bufferHeight)) {
      uint32_t* data = (uint32_t*)_buffer->pixels + y * (_buffer->pitch >> 2);
      for(uint x : range(_bufferWidth)) *data++ = 0xff000000;
    }
    if(SDL_MUSTLOCK(_buffer)) SDL_UnlockSurface(_buffer);
    output();
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(width != _width || height != _height) {
      resize(_width = width, _height = height);
    }

    if(SDL_MUSTLOCK(_buffer)) SDL_LockSurface(_buffer);
    pitch = _buffer->pitch;
    return data = (uint32_t*)_buffer->pixels;
  }

  auto unlock() -> void {
    if(SDL_MUSTLOCK(_buffer)) SDL_UnlockSurface(_buffer);
  }

  auto output() -> void {
    //ruby input is X8R8G8B8, top 8-bits are ignored.
    //as SDL forces us to use a 32-bit buffer, we must set alpha to 255 (full opacity)
    //to prevent blending against the window beneath when X window visual is 32-bits.
    if(SDL_MUSTLOCK(_buffer)) SDL_LockSurface(_buffer);
    for(uint y : range(_height)) {
      uint32_t* data = (uint32_t*)_buffer->pixels + y * (_buffer->pitch >> 2);
      for(uint x : range(_width)) *data++ |= 0xff000000;
    }
    if(SDL_MUSTLOCK(_buffer)) SDL_UnlockSurface(_buffer);

    XWindowAttributes attributes;
    XGetWindowAttributes(_display, _context, &attributes);

    SDL_Rect source;
    SDL_Rect target;

    source.x = 0;
    source.y = 0;
    source.w = _width;
    source.h = _height;

    target.x = 0;
    target.y = 0;
    target.w = attributes.width;
    target.h = attributes.height;

    SDL_SoftStretch(_buffer, &source, _screen, &target);
    SDL_UpdateRect(_screen, target.x, target.y, target.w, target.h);
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    _display = XOpenDisplay(0);

    char env[512];
    sprintf(env, "SDL_WINDOWID=%ld", (long)_context);
    putenv(env);

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    _screen = SDL_SetVideoMode(2560, 1600, 32, SDL_HWSURFACE);
    XUndefineCursor(_display, _context);

    _buffer = nullptr;
    _bufferWidth = 0;
    _bufferHeight = 0;
    resize(_width = 256, _height = 256);

    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_buffer) SDL_FreeSurface(_buffer), _buffer = nullptr;
    if(_screen) SDL_QuitSubSystem(SDL_INIT_VIDEO), _screen = nullptr;
    if(_display) XCloseDisplay(_display), _display = nullptr;
  }

  auto resize(uint width, uint height) -> void {
    if(_bufferWidth >= width && _bufferHeight >= height) return;

    _bufferWidth = max(width, _bufferWidth);
    _bufferHeight = max(height, _bufferHeight);

    if(_buffer) SDL_FreeSurface(_buffer);
    _buffer = SDL_CreateRGBSurface(
      SDL_SWSURFACE, _bufferWidth, _bufferHeight, 32,
      0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
    );
  }

  bool _ready = false;
  uintptr _context = 0;

  Display* _display = nullptr;
  SDL_Surface* _screen = nullptr;
  SDL_Surface* _buffer = nullptr;
  uint _bufferWidth = 0;
  uint _bufferHeight = 0;
  uint _width = 0;
  uint _height = 0;
};
