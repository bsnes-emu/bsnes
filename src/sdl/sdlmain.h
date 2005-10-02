#ifdef _WIN32_
#include <windows.h>
#include <SDL.h>
#else
#include "SDL.h"
#endif

#include "../lib/libbase.h"
#include "../lib/libvector.h"
#include "../lib/libstring.h"
#include "../lib/libconfig.h"

SDL_Surface *screen, *backbuffer;

struct {
  SDL_Rect rs, rd;
} screen_info;
