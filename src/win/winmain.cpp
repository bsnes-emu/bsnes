#define INTERFACE_MAIN

//requires visual c++
#define USE_X86_ASM

#include "winmain.h"
#include "../base.h"

#include "config.cpp"

#define DSP_BUFFER_SIZE 8000

#include "bsnes.h"
#include "ui.h"
#include "dd_renderer.h"
#include "ds_sound.h"

#include "timer.cpp"
fpstimer *fps_timer;

#include "lib.cpp"
#include "rom.cpp"

#include "bsnes.cpp"
#include "ui.cpp"

void init_snes() {
  mem_bus = new bMemBus();
  cpu     = new bCPU();
cpu->cpu_version = 1;
  apu     = new bAPU();
  dsp     = new bDSP();
  ppu     = new bPPU();
  snes    = new bSNES();
  bsnes   = static_cast<bSNES*>(snes);

  snes->init();
  snes->set_playback_buffer_size(DSP_BUFFER_SIZE);
}

void term_snes() {
  snes->term();

//static casting is neccesary to call derived class deconstructor...
  if(mem_bus) {
    delete(static_cast<bMemBus*>(mem_bus));
    mem_bus = 0;
  }
  if(cpu) {
    delete(static_cast<bCPU*>(cpu));
    cpu = 0;
  }
  if(apu) {
    delete(static_cast<bAPU*>(apu));
    apu = 0;
  }
  if(dsp) {
    delete(static_cast<bDSP*>(dsp));
    dsp = 0;
  }
  if(ppu) {
    delete(static_cast<bPPU*>(ppu));
    ppu = 0;
  }
  if(snes) {
    delete(static_cast<bSNES*>(snes));
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
  fps_timer = new fpstimer();
  fps_timer->start();
  rom_image = new ROMImage();

  init_ui0();
  init_snes();
  init_ui1();

int argc    = __argc;
char **argv = __argv;

  if(argc >= 2) {
    rom_image->select(argv[1]);
    rom_image->load();
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
  delete(rom_image);
  term_snes();
  memterm();
  return 0;
}
