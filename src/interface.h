#include "reader/reader.h"

#include "clock/clock.h"
#include "clock/bclock/bclock.h"
extern Clock *clock;

#include "memory/memory.h"
#include "memory/bmemory/bmemory.h"
extern MMIO mmio_unmapped;
extern MemBus *mem_bus;

#include "cpu/cpu.h"
#include "cpu/bcpu/bcpu.h"
extern CPU *cpu;

#include "ppu/ppu.h"
#include "ppu/bppu/bppu.h"
extern PPU *ppu;

#include "snes/snes.h"
extern SNES *snes;

#ifdef INTERFACE_MAIN
  Clock       *clock;
  MemBus      *mem_bus;
  CPU         *cpu;
  PPU         *ppu;
  SNES        *snes;
#endif
