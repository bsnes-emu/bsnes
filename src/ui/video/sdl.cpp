uint16 *VideoSDL::lock(uint &pitch) {
  if(SDL_MUSTLOCK(backbuffer)) { SDL_LockSurface(backbuffer); }
  pitch = backbuffer->pitch;
  return (uint16*)backbuffer->pixels;
}

void VideoSDL::unlock() {
  if(SDL_MUSTLOCK(backbuffer)) { SDL_UnlockSurface(backbuffer); }
}

void VideoSDL::redraw() {
  SDL_BlitSurface(backbuffer, 0, screen, 0);
  SDL_UpdateRect(screen, 0, 0, 256, 224);
}

void VideoSDL::update() { redraw(); }

void VideoSDL::init() {
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  screen = SDL_SetVideoMode(256, 224, 16, SDL_HWSURFACE);
  backbuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 512, 448, 16, 0xf800, 0x07e0, 0x001f, 0x0000);
}

void VideoSDL::term() {}

VideoSDL::VideoSDL(void *handle) {
  window = handle;

//
//SDL hack
//Put SDL output into main GTK+ window, rather than
//creating a new window for SDL output.
//
  if(window) {
  char t[64];
    sprintf(t, "SDL_WINDOWID=%ld", window);
  #if defined(COMPILER_GCC)
    putenv(t);
  #elif defined(COMPILER_VISUALC)
    _putenv(t);
  #endif
  }
}
