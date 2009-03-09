#include <../base.hpp>
#include <../cart/cart.hpp>

#include "main.hpp"
#include "config.cpp"

#include <ruby/ruby.hpp>
using namespace ruby;

#include <hiro/hiro.hpp>
using namespace libhiro;

#include <libfilter/libfilter.hpp>

#include "ui.hpp"
#include "status.hpp"
#include "event/event.hpp"

#include "inputmanager.cpp"
#include "interface.cpp"

#include "ui.cpp"
#include "status.cpp"
#include "event/event.cpp"

void get_paths(const char *image) {
  char temp[PATH_MAX] = "";

  if(realpath(image, temp)) {
    //remove program name
    for(signed i = strlen(temp) - 1; i >= 0; i--) {
      if(temp[i] == '/' || temp[i] == '\\') {
        temp[i] = 0;
        break;
      }
    }

    if(strend(temp, "/") == false) strcat(temp, "/");
    snes.config.path.base = temp;
  } else {
    snes.config.path.base = "";
  }

  if(userpath(temp)) {
    if(strend(temp, "/") == false) strcat(temp, "/");
    snes.config.path.user = temp;
  } else {
    snes.config.path.user = "";
  }
}

void set_config_filenames() {
  char filename[PATH_MAX];

  //locate bsnes.cfg
  strcpy(filename, snes.config.path.base);
  strcat(filename, "bsnes.cfg");
  if(!file::exists(filename)) {
    strcpy(filename, snes.config.path.user);
    strcat(filename, ".bsnes");
    mkdir(filename);
    strcat(filename, "/bsnes.cfg");
  }
  strcpy(config.bsnes_cfg, filename);

  //locate locale.cfg
  strcpy(filename, snes.config.path.base);
  strcat(filename, "locale.cfg");
  if(!file::exists(filename)) {
    strcpy(filename, snes.config.path.user);
    strcat(filename, ".bsnes");
    mkdir(filename);
    strcat(filename, "/locale.cfg");
  }
  strcpy(config.locale_cfg, filename);
}

void run() {
  while(hiro().pending()) hiro().run();
  status.update();
  input_manager.refresh();

  if(config.input.capture_mode == 2) {
    bool inactive = (window_main.focused() == false);
    if(app.autopause == false && inactive == true) {
      app.autopause = true;
      audio.clear();
    } else if(app.autopause == true && inactive == false) {
      app.autopause = false;
    }
  } else {
    app.autopause = false;
  }

  if(cartridge.loaded() == false || app.pause == true || app.autopause == true) {
    usleep(20 * 1000);  //prevent bsnes from consuming 100% CPU resources when idle
  } else {
    snes.runtoframe();
  }
}

int hiromain(int argc, const char *const argv[]) {
  get_paths(argv[0]);
  set_config_filenames();

  config.load(config.bsnes_cfg);
  if(file::exists(config.bsnes_cfg) == false) {
    //in case program crashes on first run, save config file
    //settings, so that they can be modified by hand ...
    config.save(config.bsnes_cfg);
  }
  translate.import(config.locale_cfg);

  ui_init();
  if(app.term == false) {
    snes.init();
    if(argc >= 2) event::load_image(argv[1]);
    while(app.term == false) run();
    event::unload_cart();
  }

  config.save(config.bsnes_cfg);
  snes.term();
  ui_term();
  return 0;
}
