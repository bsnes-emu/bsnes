#ifdef _WIN32_
#include <windows.h>
#include <SDL.h>
#else
#include "SDL.h"
#endif

SDL_Surface *screen, *backbuffer;

struct {
  SDL_Rect rs, rd;
} screen_info;
