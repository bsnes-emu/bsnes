#define INTERFACE_MAIN

#include "winmain.h"
#include "../base.h"

#include "config.cpp"

#include "bsnes.h"
#include "ui.h"
#include "dd_renderer.h"
#include "ds_sound.h"

#include "lib.cpp"

#include "bsnes.cpp"
#include "ui.cpp"

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
}

void term_snes() {
  snes->term();

#ifdef POLYMORPHISM
//static casting is neccesary to call derived class deconstructor...
  if(deref(mem)) {
    delete static_cast<bMemBus*>(deref(mem));
    deref(mem) = 0;
  }
  if(deref(cpu)) {
    delete static_cast<bCPU*>(deref(cpu));
    deref(cpu) = 0;
  }
  if(deref(apu)) {
    delete static_cast<bAPU*>(deref(apu));
    deref(apu) = 0;
  }
  if(deref(dsp)) {
    delete static_cast<bDSP*>(deref(dsp));
    deref(dsp) = 0;
  }
  if(deref(ppu)) {
    delete static_cast<bPPU*>(deref(ppu));
    deref(ppu) = 0;
  }
#endif
  if(snes) {
    delete static_cast<bSNES*>(snes);
    snes = 0;
  }
}

void get_config_fn(string &str) {
char *t = (char*)malloc(4096);
  _getcwd(t, 4095);
  strcpy(str, t);
  free(t);
  strcat(str, "\\bsnes.cfg");
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
MSG msg;
string cfg_fn;
  get_config_fn(cfg_fn);
  config_file.load(cfg_fn);
  meminit();

  init_ui0();
  init_snes();
  init_ui1();

int argc    = __argc;
char **argv = __argv;

  if(argc >= 2) {
    cartridge.load(argv[1]);
    bsnes->debugger_activate();
    snes->power();
    bsnes->debugger_update();
  }

  while(1) {
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_QUIT)goto _end;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    bsnes->run();
  }

_end:
  config_file.save(cfg_fn);
  term_snes();
  cartridge.unload();
  memterm();
  return 0;
}
