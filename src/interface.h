#ifdef POLYMORPHISM
  #define ref(x) x
#else
  #define ref(x) (&x)
#endif
#define deref(x) __##x

#define r_mem ref(deref(mem))
#define r_cpu ref(deref(cpu))
#define r_apu ref(deref(apu))
#define r_dsp ref(deref(dsp))
#define r_ppu ref(deref(ppu))

#include "reader/reader.h"
#include "cart/cart.h"
#include "cheat/cheat.h"

#include "memory/memory.h"
#include "memory/bmemory/bmemory.h"

#include "cpu/cpu.h"
#include "cpu/bcpu/bcpu.h"

#include "apu/apu.h"
#include "apu/bapu/bapu.h"

#include "dsp/dsp.h"
#include "dsp/bdsp/bdsp.h"

#include "ppu/ppu.h"
#include "ppu/bppu/bppu.h"

#include "snes/snes.h"

#include "chip/srtc/srtc.h"
#include "chip/sdd1/sdd1.h"
#include "chip/c4/c4.h"
#include "chip/dsp2/dsp2.h"
#include "chip/obc1/obc1.h"

extern MMIO mmio_unmapped;
#ifdef POLYMORPHISM
  extern MemBus *deref(mem);
  extern CPU    *deref(cpu);
  extern APU    *deref(apu);
  extern DSP    *deref(dsp);
  extern PPU    *deref(ppu);
#else
  extern MEMCORE deref(mem);
  extern CPUCORE deref(cpu);
  extern APUCORE deref(apu);
  extern DSPCORE deref(dsp);
  extern PPUCORE deref(ppu);
  #endif
extern SNES *snes;

extern SRTC *srtc;
extern SDD1 *sdd1;
extern C4   *c4;
extern DSP2 *dsp2;
extern OBC1 *obc1;

#include "config/config.h"

#ifdef INTERFACE_MAIN
Cartridge cartridge;
Cheat     cheat;

#include "config/config.cpp"
#ifdef POLYMORPHISM
  MemBus *deref(mem);
  CPU    *deref(cpu);
  APU    *deref(apu);
  DSP    *deref(dsp);
  PPU    *deref(ppu);
#else
  MEMCORE deref(mem);
  CPUCORE deref(cpu);
  APUCORE deref(apu);
  DSPCORE deref(dsp);
  PPUCORE deref(ppu);
#endif
SNES *snes;

SRTC *srtc;
SDD1 *sdd1;
C4   *c4;
DSP2 *dsp2;
OBC1 *obc1;

#endif
