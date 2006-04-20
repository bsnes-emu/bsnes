#define INTERFACE_MAIN

#include "../base.h"
#include "../lib/libwin32.h"
#include "../lib/libwin32.cpp"

#include "config.cpp"

#include "bsnes.h"
#include "event.h"
#include "ui.h"

#include "bsnes.cpp"
#include "event.cpp"
#include "ui.cpp"

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
#ifdef DEBUGGER
  wDebug.Print(str);
#else
  fprintf(stdout, "%s\r\n", str);
#endif
}

void init_snes() {
#ifdef POLYMORPHISM
  deref(mem) = new MEMCORE();
  deref(cpu) = new CPUCORE();
  deref(apu) = new APUCORE();
  deref(dsp) = new DSPCORE();
  deref(ppu) = new PPUCORE();
#endif
  snes  = new bSNES();
  bsnes = static_cast<bSNES*>(snes);

  snes->init();
}

void term_snes() {
  snes->term();
#ifdef POLYMORPHISM
  SafeDelete(deref(mem));
  SafeDelete(deref(cpu));
  SafeDelete(deref(apu));
  SafeDelete(deref(dsp));
  SafeDelete(deref(ppu));
#endif
  SafeDelete(snes);
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

  config::fs.base_path.sset(strptr(t));
}

int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
  InitCommonControls();
  get_base_path();

string cfg_fn;
  strcpy(cfg_fn, config::fs.base_path.sget());
  strcat(cfg_fn, "bsnes.cfg");
  config_file.load(cfg_fn);

  init_snes();
  init_ui();

  if(__argc >= 2) {
    if(cartridge.load(__argv[1]) == true) {
      snes->power();
    }
  }

MSG msg;
  while(1) {
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_QUIT)goto _end;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    bsnes->run();
  }

_end:
  cartridge.unload();

  term_ui();
  term_snes();
  config_file.save(cfg_fn);
  return 0;
}
