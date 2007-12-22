#include "reader/reader.h"
#include "cheat/cheat.h"
#include "config/config.h"

#include "memory/memory.h"
#include "memory/smemory/smemory.h"

#include "cart/cart.h"

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
#include "chip/chip.h"

#ifdef INTERFACE_MAIN
  #include "config/config.cpp"
  #define extern
#endif

extern BUSCORE bus;
extern CPUCORE cpu;
extern SMPCORE smp;
extern DSPCORE dsp;
extern PPUCORE ppu;

#undef extern
