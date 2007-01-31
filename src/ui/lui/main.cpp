#if defined(PLATFORM_WIN)
  #include "../../lib/libui_win.cpp"
#elif defined(PLATFORM_X)
  #include "../../lib/libui_gtk.cpp"
#else
  #error "unsupported platform"
#endif

namespace ui = libui;

bool _term_ = false;

#include "ui.h"
#include "event.h"

#include "ui.cpp"
#include "event.cpp"

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(uint source, char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "[%d]: %s\r\n", source, str);
}

#if defined(PLATFORM_WIN)
int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
int argc = __argc;
char **argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif

//int main(int argc, char *argv[]) {
  ui::init();
  config_file.load("bsnes_lui.cfg");
  init_snes();
  ui_init();

  while(_term_ == false) {
    while(ui::events_pending() == true) { ui::run(); }
    if(cartridge.loaded() == true)snes.runtoframe();
  }

  if(cartridge.loaded() == true)cartridge.unload();

  config_file.save("bsnes_lui.cfg");
  term_snes();
  ui_term();
  ui::term();
  return 0;
}
