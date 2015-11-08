#if defined(DEBUGGER)
  #error "bsnes: debugger not supported with performance profile."
#endif

#include <nall/priority-queue.hpp>
#include <sfc/alt/cpu/cpu.hpp>
#include <sfc/alt/smp/smp.hpp>
#include <sfc/alt/dsp/dsp.hpp>
#include <sfc/alt/ppu-performance/ppu.hpp>
