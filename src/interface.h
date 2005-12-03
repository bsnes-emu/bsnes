#define BSNES_VERSION "0.015"
#define BSNES_TITLE "bsnes v" BSNES_VERSION

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

extern MMIO mmio_unmapped;
#ifdef POLYMORPHISM
  extern MemBus *deref(mem);
  extern CPU    *deref(cpu);
  extern APU    *deref(apu);
  extern DSP    *deref(dsp);
  extern PPU    *deref(ppu);
#else
  extern bMemBus deref(mem);
  extern bCPU    deref(cpu);
  extern bAPU    deref(apu);
  extern bDSP    deref(dsp);
  extern bPPU    deref(ppu);
  #endif
extern SNES *snes;

extern SRTC *srtc;
extern SDD1 *sdd1;
extern C4   *c4;

#include "config/config.h"

#ifdef INTERFACE_MAIN
Cartridge cartridge;

#include "config/config.cpp"
#ifdef POLYMORPHISM
  MemBus *deref(mem);
  CPU    *deref(cpu);
  APU    *deref(apu);
  DSP    *deref(dsp);
  PPU    *deref(ppu);
#else
  bMemBus deref(mem);
  bCPU    deref(cpu);
  bAPU    deref(apu);
  bDSP    deref(dsp);
  bPPU    deref(ppu);
#endif
SNES *snes;

SRTC *srtc;
SDD1 *sdd1;
C4   *c4;

#endif
