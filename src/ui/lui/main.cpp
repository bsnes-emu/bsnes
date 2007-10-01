void run();

#if defined(PLATFORM_WIN)
  #include "../../lib/libui_win.h"
#elif defined(PLATFORM_X)
  #include "../../lib/libui_gtk.h"
#else
  #error "unsupported platform"
#endif

namespace ui = libui;

bool _term_ = false;

#include "ui.h"
#include "event.h"

#include "ui.cpp"
#include "event.cpp"

void alert(const char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(const char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(uint source, const char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "[%d]: %s\r\n", source, str);
}

void set_config_filename() {
  userpath(config::filename);
  strcat(config::filename, "/.bsnes");
  mkdir(config::filename); //always make directory in case it does not exist, fail silently if it does
  strcat(config::filename, "/bsnes.cfg");
}

void get_base_path() {
#if defined(PLATFORM_WIN)
char full_name[PATH_MAX];
  GetFullPathName(__argv[0], PATH_MAX, full_name, 0);

string t;
  strcpy(t, full_name);

  if(strlen(t) != 0) {
  //remove program name
    replace(t, "\\", "/");
    for(int i = strlen(t) - 1; i >= 0; i--) {
      if(strptr(t)[i] == '/' || strptr(t)[i] == '\\') {
        strptr(t)[i] = 0;
        break;
      }
    }
  }

  if(strend(t, "/") == false) { strcat(t, "/"); }
  config::path.base = strptr(t);
#endif
}

void run() {
  while(ui::events_pending() == true) { ui::run(); }
  if(cartridge.loaded() == true) {
    snes.runtoframe();
    event::update_frame_counter();
  } else { //prevent bsnes from consuming 100% CPU resources when idle
  #if defined(PLATFORM_WIN)
    Sleep(1);
  #elif defined(PLATFORM_X)
    usleep(1);
  #endif
  }
}

#if defined(PLATFORM_WIN)
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
int argc = __argc;
char **argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif

//int main(int argc, char *argv[]) {
  set_config_filename();
  get_base_path();

  ui::init();
  config::config().load(config::filename);
  config::config().save(config::filename); //in case program crashes on first run, config file settings can be modified
  init_snes();
  ui_init();

  if(argc >= 2) {
    cartridge.load_begin(Cartridge::CART_NORMAL);
    cartridge.load(argv[1]);
    cartridge.load_end();
    snes.power();
  }

  while(_term_ == false) {
    run();
  }

  event::unload_rom();

  config::config().save(config::filename);
  term_snes();
  ui_term();
  ui::term();
  return 0;
}
