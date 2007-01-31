#define INTERFACE_MAIN

#include "../base.h"
#include "main.h"
#include "config.cpp"

void init_snes();
void term_snes();

/*****
 * hardware abstraction layer
 *****/

#include "video/video.h"
#include "audio/audio.h"
#include "input/input.h"

#include "video/video.cpp"
#include "audio/audio.cpp"
#include "input/input.cpp"

#include "interface.cpp"

/*****
 * platform abstraction layer
 *****/

#if defined(UI_LUI)
  #include "lui/main.cpp"
#elif defined(UI_WIN)
  #include "win/main.cpp"
#elif defined(UI_SDL)
  #include "sdl/main.cpp"
#else
  #error "unsupported platform"
#endif

/*****
 * platform independent code
 *****/

void init_snes() {
#if defined(POLYMORPHISM)
  deref(mem) = new MEMCORE();
  deref(cpu) = new CPUCORE();
  deref(apu) = new APUCORE();
  deref(dsp) = new DSPCORE();
  deref(ppu) = new PPUCORE();
#endif

  snes.init();
}

void term_snes() {
  snes.term();

#if defined(POLYMORPHISM)
  safe_delete(deref(mem));
  safe_delete(deref(cpu));
  safe_delete(deref(apu));
  safe_delete(deref(dsp));
  safe_delete(deref(ppu));
#endif
}
