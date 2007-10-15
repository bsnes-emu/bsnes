#ifdef POLYMORPHISM
  #define ref(x) x
#else
  #define ref(x) (&x)
#endif
#define deref(x) __##x

#define r_mem ref(deref(mem))
#define r_cpu ref(deref(cpu))
#define r_smp ref(deref(smp))
#define r_dsp ref(deref(dsp))
#define r_ppu ref(deref(ppu))

#include "reader/reader.h"
#include "cart/cart.h"
#include "cheat/cheat.h"

#include "memory/memory.h"
#include "memory/bmemory/bmemory.h"

#include "cpu/cpu.h"
#include "cpu/scpu/scpu.h"

#include "smp/smp.h"
#include "smp/ssmp/ssmp.h"

#include "dsp/dsp.h"
//#include "dsp/adsp/adsp.h"
#include "dsp/bdsp/bdsp.h"

#include "ppu/ppu.h"
#include "ppu/bppu/bppu.h"

#include "snes/snes.h"

#include "chip/superfx/superfx.h"
#include "chip/srtc/srtc.h"
#include "chip/sdd1/sdd1.h"
#include "chip/c4/c4.h"
#include "chip/dsp1/dsp1.h"
#include "chip/dsp2/dsp2.h"
#include "chip/dsp3/dsp3.h"
#include "chip/dsp4/dsp4.h"
#include "chip/obc1/obc1.h"
#include "chip/st010/st010.h"

extern MMIO mmio_unmapped;
#ifdef POLYMORPHISM
  extern MemBus *deref(mem);
  extern CPU    *deref(cpu);
  extern SMP    *deref(smp);
  extern DSP    *deref(dsp);
  extern PPU    *deref(ppu);
#else
  extern MEMCORE deref(mem);
  extern CPUCORE deref(cpu);
  extern SMPCORE deref(smp);
  extern DSPCORE deref(dsp);
  extern PPUCORE deref(ppu);
#endif

#include "config/config.h"

#ifdef INTERFACE_MAIN
  #include "config/config.cpp"
  #ifdef POLYMORPHISM
    MemBus *deref(mem);
    CPU    *deref(cpu);
    APU    *deref(smp);
    DSP    *deref(dsp);
    PPU    *deref(ppu);
  #else
    MEMCORE deref(mem);
    CPUCORE deref(cpu);
    SMPCORE deref(smp);
    DSPCORE deref(dsp);
    PPUCORE deref(ppu);
  #endif
#endif
