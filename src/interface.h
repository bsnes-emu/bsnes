#define BSNES_VERSION "0.013 wip2"
#define BSNES_TITLE "bsnes v" BSNES_VERSION

#include "reader/reader.h"

#include "memory/memory.h"
#include "memory/bmemory/bmemory.h"
extern MMIO mmio_unmapped;
extern MemBus *mem_bus;

#include "cpu/cpu.h"
#include "cpu/bcpu/bcpu.h"
extern CPU *cpu;

#include "apu/apu.h"
#include "apu/bapu/bapu.h"
#include "apu/bapuskip/bapuskip.h"
extern APU *apu;

#include "dsp/dsp.h"
#include "dsp/bdsp/bdsp.h"
extern DSP *dsp;

#include "ppu/ppu.h"
#include "ppu/bppu/bppu.h"
extern PPU *ppu;

#include "snes/snes.h"
extern SNES *snes;

#include "chip/srtc/srtc.h"
#include "chip/sdd1/sdd1.h"
extern SRTC *srtc;
extern SDD1 *sdd1;

#include "config/config.h"

#ifdef INTERFACE_MAIN
#include "config/config.cpp"
  MemBus *mem_bus;
  CPU    *cpu;
  APU    *apu;
  DSP    *dsp;
  PPU    *ppu;
  SNES   *snes;

  SRTC   *srtc;
  SDD1   *sdd1;
#endif
