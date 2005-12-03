#define INTERFACE_MAIN
#include "../base.h"
#include "sdlmain.h"

#include "config.cpp"

#ifdef _WIN32_
HWND hwnd;
#endif

#include "bsnes.h"
#include "render.cpp"
#include "bsnes.cpp"

void *memalloc(uint32 size, char *name, ...) {
  return (void*)malloc(size);
}

void memfree(void *mem, char *name, ...) {
  free(mem);
}

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
#ifdef _WIN32_
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

void init_snes() {
#ifdef POLYMORPHISM
  deref(mem) = new bMemBus();
  deref(cpu) = new bCPU();
  deref(apu) = new bAPU();
  deref(dsp) = new bDSP();
  deref(ppu) = new bPPU();
#endif
  snes  = new bSNES();
  bsnes = static_cast<bSNES*>(snes);

  snes->init();

//TODO: add sound support and remove this,
//this is used with linux/bsd and mkfifo to
//play audio in real-time while sound output
//isn't available.
  snes->log_audio_enable("output.wav");
}

void term_snes() {
  snes->term();
#ifdef POLYMORPHISM
  if(deref(mem)) { delete deref(mem); deref(mem) = 0; }
  if(deref(cpu)) { delete deref(cpu); deref(cpu) = 0; }
  if(deref(apu)) { delete deref(apu); deref(apu) = 0; }
  if(deref(dsp)) { delete deref(dsp); deref(dsp) = 0; }
  if(deref(ppu)) { delete deref(ppu); deref(ppu) = 0; }
#endif
  if(snes) { delete(snes); snes = 0; }
}

void center_window() {
#ifdef _WIN32_
RECT rc;
int x, y;
  GetWindowRect(hwnd, &rc);
  x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) >> 1;
  y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) >> 1;
  SetWindowPos(hwnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
#endif
}

void set_window_info() {
//SDL won't draw anything if you blit an image that's larger than
//the display mode/window, even if you use the SoftStretch blit and
//clip the source + dest rectangles properly...
  if((int)config::video.display_width < (int)config::video.output_width) {
    config::video.display_width = config::video.output_width;
  }
  if((int)config::video.display_height < (int)config::video.output_height) {
    config::video.display_height = config::video.output_height;
  }

  screen_info.rd.x = ((int)config::video.display_width  - (int)config::video.output_width ) >> 1;
  screen_info.rd.y = ((int)config::video.display_height - (int)config::video.output_height) >> 1;
  screen_info.rd.w = config::video.output_width;
  screen_info.rd.h = config::video.output_height;
}

#ifdef _WIN32_
int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
int argc    = __argc;
char **argv = __argv;
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

  if(cartridge.load(argv[1]) == false) {
    alert("Failed to load image. Usage: bsnes_sdl <filename.smc>");
    goto _end;
  }

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  set_window_info();
  screen = SDL_SetVideoMode(config::video.display_width, config::video.display_height, 16,
    SDL_SWSURFACE | ((config::video.fullscreen) ? SDL_FULLSCREEN : 0));
  if(!screen)     { alert("Failed to initialize SDL"); goto _end; }
  backbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 512, 448, 16, 0xf800, 0x07e0, 0x001f, 0x0000);
  if(!backbuffer) { alert("Failed to initialize SDL"); goto _end; }

  SDL_WM_SetCaption(BSNES_TITLE, 0);
#ifdef _WIN32_
  hwnd = FindWindow(0, BSNES_TITLE);
#endif
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
          snes->capture_screenshot();
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
  term_snes();

  return 0;
}
