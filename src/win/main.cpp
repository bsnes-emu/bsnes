#define INTERFACE_MAIN

#include "../base.h"
#include "main.h"

#include "config.cpp"

#include "bsnes.h"
#include "ui.h"

#include "bsnes.cpp"
#include "ui.cpp"

void *memalloc(uint32 size, char *name, ...) {
  return (void*)malloc(size);
}

void memfree(void *mem, char *name, ...) {
  free(mem);
}

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  MessageBox(0, str, "bsnes", MB_OK);
}

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

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
  if(deref(mem)) { delete deref(mem); deref(mem) = 0; }
  if(deref(cpu)) { delete deref(cpu); deref(cpu) = 0; }
  if(deref(apu)) { delete deref(apu); deref(apu) = 0; }
  if(deref(dsp)) { delete deref(dsp); deref(dsp) = 0; }
  if(deref(ppu)) { delete deref(ppu); deref(ppu) = 0; }
#endif
  if(snes) { delete(snes); snes = 0; }
}

void get_config_fn(string &str) {
char *t = (char*)malloc(4096);
  _getcwd(t, 4095);
  strcpy(str, t);
  free(t);
  strcat(str, "\\bsnes.cfg");
}

int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
MSG msg;
string cfg_fn;
  get_config_fn(cfg_fn);
  config_file.load(cfg_fn);
  init_snes();
  init_ui();

int argc    = __argc;
char **argv = __argv;
  if(argc >= 2) {
    if(cartridge.load(argv[1]) == true) {
      snes->power();
    }
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
  term_snes();
  cartridge.unload();
  config_file.save(cfg_fn);
  return 0;
}
