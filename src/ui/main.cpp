#define INTERFACE_MAIN

#include "../base.h"
#include "main.h"
#include "config.cpp"

void init_snes();
void term_snes();

/*****
 * OS abstraction layer
 *****/

#include "video/video.h"
#include "audio/audio.h"
#include "input/input.h"

#include "video/video.cpp"
#include "input/input.cpp"

/*****
 * platform abstraction layer
 *****/

#if defined(PLATFORM_WIN)
  #include "win/main.cpp"
#elif defined(PLATFORM_SDL)
  #include "sdl/main.cpp"
#else
  #error "unsupported platform"
#endif

/*****
 * platform independent code
 *****/

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
