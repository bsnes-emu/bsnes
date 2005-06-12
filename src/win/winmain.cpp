#define INTERFACE_MAIN
#define BSNES_VERSION "0.007"
#include "winmain.h"
#include "../base.h"

#include "config.cpp"

#include "bsnes.h"
#include "ui.h"

#include "timer.cpp"
fpstimer *fps_timer;

#include "lib.cpp"
#include "rom.cpp"

#include "bsnes.cpp"
#include "ui.cpp"

void init_snes() {
  clock   = new bClock();
  mem_bus = new bMemBus();
  cpu     = new bCPU();
  ppu     = new bPPU();
  snes    = new bSNES();
  bsnes   = static_cast<bSNES*>(snes);
}

void term_snes() {
  if(clock)   { delete(clock);   clock   = 0; }
  if(mem_bus) { delete(mem_bus); mem_bus = 0; }
  if(cpu)     { delete(cpu);     cpu     = 0; }
  if(ppu)     { delete(ppu);     ppu     = 0; }
  if(snes)    { delete(snes);    snes    = 0; }
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
MSG msg;
string cfg_fn;
  _getcwd(cfg_fn, 4096);
  cfg_fn += "\\bsnes.cfg";
  cfg.load(cfg_fn);
  meminit();
  fps_timer = new fpstimer();
  fps_timer->start();
  rom_image = new ROMImage();

  init_ui0();
  init_snes();
  init_ui1();

  if(lpCmdLine) {
    if(*lpCmdLine) {
      rom_image->select(lpCmdLine);
      rom_image->load();
      snes->power();
      bsnes->debugger_update();
    }
  }

  while(1) {
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_QUIT)break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    bsnes->run();
  }

  cfg.save(cfg_fn);
  delete(rom_image);
  term_snes();
  memterm();
  return 0;
}
