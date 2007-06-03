#include "../../lib/libwin32.h"
#include "../../lib/libwin32.cpp"

#include "config.cpp"

#include "bsnes.h"

#include "event.h"
#include "ui.h"

#include "bsnes.cpp"

#include "../video/d3d.h"
#include "../video/ddraw.h"
#include "../audio/dsound.h"
#include "../input/dinput.h"

#include "../video/d3d.cpp"
#include "../video/ddraw.cpp"
#include "../audio/dsound.cpp"
#include "../input/dinput.cpp"

#include "event.cpp"
#include "ui.cpp"

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  wDebug.Print(source::none, str);
}

void dprintf(uint source, char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  wDebug.Print(source, str);
}

void get_base_path() {
char full_name[4095];
  GetFullPathName(__argv[0], 4095, full_name, 0);

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
}

int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
  timeBeginPeriod(1);
  InitCommonControls();
  get_base_path();

string cfg_fn;
  strcpy(cfg_fn, config::path.base);
  strcat(cfg_fn, "bsnes.cfg");
  config_file.load(cfg_fn);

  init_snes();
  init_ui();

  if(__argc >= 2) {
    cartridge.load_begin(Cartridge::CART_NORMAL);
    cartridge.load(__argv[1]);
    cartridge.load_end();
    snes.power();
  }

MSG msg;
  while(1) {
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_QUIT)goto end;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    bsnes.run();
  }

end:
  if(cartridge.loaded() == true)cartridge.unload();

  term_ui();
  term_snes();
  config_file.save(cfg_fn);
  timeEndPeriod(1);
  return 0;
}
