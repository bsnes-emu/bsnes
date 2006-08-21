#include "main.h"

#include "bsnes.h"
#include "render.cpp"
#include "bsnes.cpp"

#ifdef PLATFORM_WIN
#include "../audio/dsound.h"
#include "../audio/dsound.cpp"
#endif

#include "../input/sdl.h"
#include "../input/sdl.cpp"

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
#ifdef PLATFORM_WIN
  MessageBox(0, str, "bsnes", MB_OK);
#else
  fprintf(stdout, "%s\r\n", str);
#endif
}

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void center_window() {
#ifdef PLATFORM_WIN
HWND hwnd;
RECT rc;
int x, y;
  hwnd = FindWindow(0, BSNES_TITLE);
  GetWindowRect(hwnd, &rc);
  x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) >> 1;
  y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) >> 1;
  SetWindowPos(hwnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
#endif
}

#ifdef PLATFORM_WIN
int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
int argc    = __argc;
char **argv = __argv;
  SDL_SetModuleHandle(GetModuleHandle(0));
#else
int main(int argc, char *argv[]) {
#endif

SDL_Event event;

  if(argc < 2) {
    alert("Usage: bsnes_sdl <filename.smc>");
    return 0;
  }

  config_file.load("bsnes_sdl.cfg");

  init_snes();

#ifdef PLATFORM_WIN
  uiAudio = new AudioDS();
#else
  uiAudio = new Audio();
#endif
  uiInput = new InputSDL();
  uiAudio->init();
  uiInput->init();

  if(cartridge.load(argv[1]) == false) {
    alert("Failed to load image. Usage: bsnes_sdl <filename.smc>");
    goto _end;
  }

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  screen_rect.x = 0;
  screen_rect.y = 0;
  screen_rect.w = 256;
  screen_rect.h = 224;

  screen = SDL_SetVideoMode(screen_rect.w, screen_rect.h, 16,
    SDL_HWSURFACE | (0 ? SDL_FULLSCREEN : 0));
  if(!screen)     { alert("Failed to initialize SDL"); goto _end; }

  backbuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 512, 448, 16, 0xf800, 0x07e0, 0x001f, 0x0000);
  if(!backbuffer) { alert("Failed to initialize SDL"); goto _end; }

  SDL_WM_SetCaption(BSNES_TITLE, 0);
  center_window();

  snes->power();
  bsnes->set_status(bSNES::RUN);

int cursor_status;
  while(1) {
    bsnes->run();
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_KEYUP:
        switch(event.key.keysym.sym) {
        case SDLK_ESCAPE:
          goto _end;
        case SDLK_BACKSPACE:
        //snes->capture_screenshot();
          break;
        case SDLK_F10: //toggle cursor display
          cursor_status = (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE) ? SDL_DISABLE : SDL_ENABLE;
          SDL_ShowCursor(cursor_status);
          break;
        case SDLK_F11: //only supported on X11
          SDL_WM_ToggleFullScreen(screen);
          break;
        }
        break;
      case SDL_QUIT:
        goto _end;
      }
    }
  }

_end:
  config_file.save("bsnes_sdl.cfg");
  cartridge.unload();
  uiAudio->term();
  uiInput->term();
  SafeDelete(uiAudio);
  SafeDelete(uiInput);
  term_snes();

  return 0;
}
