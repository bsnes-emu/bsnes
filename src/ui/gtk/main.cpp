#include "main.h"

#include "bsnes.h"
#include "gtk_mainwindow.cpp"
#include "bsnes.cpp"

#include "../video/gtk.h"
#include "../video/gtk.cpp"
#include "../video/sdl.h"
#include "../video/sdl.cpp"

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

#ifdef PLATFORM_WIN
int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
int argc    = __argc;
char **argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif
  gtk_init(&argc, &argv);
  init_main_window();

  config_file.load("bsnes.cfg");
  init_snes();
  uiVideo = new VideoSDL((void*)GDK_WINDOW_XWINDOW(main_window.render->window));
  uiAudio = new Audio();
  uiInput = new InputSDL((void*)GDK_WINDOW_XWINDOW(main_window.render->window));
  uiVideo->init();
  uiAudio->init();
  uiInput->init();

//will not run if cartridge not loaded
  bsnes->set_status(bSNES::RUN);

  if(argc >= 2 && cartridge.load(argv[1]) == true) {
    snes->power();
  }

  while(running == true) {
    bsnes->run();

    while(gtk_events_pending() == true) {
      gtk_main_iteration_do(false);
    }

  SDL_Event event;
    while(SDL_PollEvent(&event)) {
    }
  }

end:
  config_file.save("bsnes.cfg");
  cartridge.unload();
  uiVideo->term();
  uiAudio->term();
  uiInput->term();
  SafeDelete(uiVideo);
  SafeDelete(uiAudio);
  SafeDelete(uiInput);
  term_snes();

  return 0;
}
