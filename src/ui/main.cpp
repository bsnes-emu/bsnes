#define INTERFACE_MAIN

#include "../base.h"

#include "main.h"
#include "config.cpp"

void init_snes();
void term_snes();

/*****
 * hardware abstraction layer
 *****/

#include <ruby/ruby.h>
using namespace ruby;

#include <libfilter/libfilter.h>

#include "inputmanager.cpp"
#include "interface.cpp"

/*****
 * platform abstraction layer
 *****/

#include <hiro/hiro.h>
using namespace libhiro;

/*****
 * core
 *****/

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

  window_message.show(str);
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

void get_base_path(const char *image) {
  char full_name[PATH_MAX] = "";
  realpath(image, full_name);

  string t = full_name;
  if(strlen(t) != 0) {
    //remove program name
    //TODO: rewrite this to be cleaner ...
    replace(t, "\\", "/");
    for(int i = strlen(t) - 1; i >= 0; i--) {
      if(t()[i] == '/' || t()[i] == '\\') {
        t()[i] = 0;
        break;
      }
    }
  }

  if(strend(t, "/") == false) strcat(t, "/");
  config::path.base = t;
}

void run() {
  while(hiro().pending()) hiro().run();
  input_manager.refresh();

  if(config::input.capture_mode == 2) {
    bool inactive = window_main.focused() == false;
    if(app.autopause == false && inactive == true) {
      app.autopause = true;
      audio.clear();
      if(cartridge.loaded()) event::update_status();
    } else if(app.autopause == true && inactive == false) {
      app.autopause = false;
    }
  } else {
    app.autopause = false;
  }

  if(cartridge.loaded() == false || app.pause == true || app.autopause == true) {
    //prevent bsnes from consuming 100% CPU resources when idle
    usleep(20 * 1000);
  } else {
    snes.runtoframe();
    event::update_status();
  }
}

#if defined(PLATFORM_WIN)
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
int argc = __argc;
char **argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif
/*
int main(int argc, char *argv[]) { */
  set_config_filename();
  get_base_path(argv[0]);

  config::config().load(config::filename);
  if(fexists(config::filename) == false) {
    //in case program crashes on first run, save config file
    //settings, so that they can be modified by hand ...
    config::config().save(config::filename);
  }
  hiro().init();
  ui_init();
  snes.init();

  if(argc >= 2) {
    cartridge.load_cart_normal(argv[1]);
    snes.power();
  }

  while(app.term == false) run();

  event::unload_rom();

  config::config().save(config::filename);
  snes.term();
  ui_term();
  hiro().term();
  return 0;
}
