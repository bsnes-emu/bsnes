#define INTERFACE_MAIN
#define BSNES_VERSION "0.011"
#define BSNES_TITLE "bsnes/SDL v" BSNES_VERSION
#include "../base.h"
#include "sdlmain.h"

#ifdef _WIN32_
HWND hwnd;
#endif

#include "config.cpp"
#include "bsnes.h"
#include "rom.cpp"
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
  mem_bus = new bMemBus();
  cpu     = new bCPU();
  if(cfg.apu.enabled) {
    apu = new bAPU();
  } else {
    apu = new bAPUSkip();
  }
  ppu     = new bPPU();
  snes    = new bSNES();
  bsnes   = static_cast<bSNES*>(snes);

  snes->init();
}

void term_snes() {
  if(mem_bus) { delete(mem_bus); mem_bus = 0; }
  if(cpu)     { delete(cpu);     cpu     = 0; }
  if(apu)     { delete(apu);     apu     = 0; }
  if(ppu)     { delete(ppu);     ppu     = 0; }
  if(snes)    { delete(snes);    snes    = 0; }
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
  if(cfg.video.display_width < cfg.video.output_width) {
    cfg.video.display_width = cfg.video.output_width;
  }
  if(cfg.video.display_height < cfg.video.output_height) {
    cfg.video.display_height = cfg.video.output_height;
  }

  screen_info.rd.x = (cfg.video.display_width  - cfg.video.output_width ) >> 1;
  screen_info.rd.y = (cfg.video.display_height - cfg.video.output_height) >> 1;
  screen_info.rd.w = cfg.video.output_width;
  screen_info.rd.h = cfg.video.output_height;

  screen_info.rs.x = 0;
  screen_info.rs.y = 0;
  screen_info.rs.w = 256;
  screen_info.rs.h = 223;
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

  cfg.load("bsnes_sdl.cfg");

  rom_image = new ROMImage();
  init_snes();

  rom_image->select(argv[1]);
  rom_image->load();
  snes->power();

  if(rom_image->loaded() == false) {
    alert("Failed to load image. Usage: bsnes_sdl <filename.smc>");
    goto _end;
  }

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  set_window_info();
  screen = SDL_SetVideoMode(cfg.video.display_width, cfg.video.display_height, 16,
    SDL_SWSURFACE | ((cfg.video.fullscreen)?SDL_FULLSCREEN:0));
  if(!screen) {
    alert("Failed to initialize SDL");
    goto _end;
  }
  backbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 256, 223, 16, 0xf800, 0x07e0, 0x001f, 0x0000);

  SDL_WM_SetCaption(BSNES_TITLE, 0);
#ifdef _WIN32_
  hwnd = FindWindow(0, BSNES_TITLE);
#endif
  center_window();

  update_color_lookup_table();
  bsnes->set_status(bSNES::RUN);

int cursor_status;
  while(1) {
    bsnes->snes_run();
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_KEYUP:
        switch(event.key.keysym.sym) {
        case SDLK_ESCAPE:
          goto _end;
        case SDLK_F10: //toggle cursor display
          cursor_status = (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)?SDL_DISABLE:SDL_ENABLE;
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
  cfg.save("bsnes_sdl.cfg");
  term_snes();

  return 0;
}
