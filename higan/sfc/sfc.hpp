#pragma once

//license: GPLv3
//started: 2004-10-14

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/random.hpp>
#include <emulator/cheat.hpp>

#include <processor/arm7tdmi/arm7tdmi.hpp>
#include <processor/gsu/gsu.hpp>
#include <processor/hg51b/hg51b.hpp>
#include <processor/spc700/spc700.hpp>
#include <processor/upd96050/upd96050.hpp>
#include <processor/wdc65816/wdc65816.hpp>

#if defined(SFC_SUPERGAMEBOY)
  #include <gb/gb.hpp>
#endif

namespace SuperFamicom {
  #define platform Emulator::platform
  namespace File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Random = Emulator::Random;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
  extern Random random;
  extern Cheat cheat;

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void {
      Emulator::Thread::create(entrypoint, frequency);
      scheduler.append(*this);
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }
  };

  struct Region {
    static inline auto NTSC() -> bool;
    static inline auto PAL() -> bool;
  };

  #include <sfc/system/system.hpp>
  #include <sfc/memory/memory.hpp>
  #include <sfc/ppu/counter/counter.hpp>

  #include <sfc/cpu/cpu.hpp>
  #include <sfc/smp/smp.hpp>
  #include <sfc/dsp/dsp.hpp>
  #include <sfc/ppu/ppu.hpp>
  #include <sfc/ppu-fast/ppu.hpp>

  #include <sfc/controller/controller.hpp>
  #include <sfc/expansion/expansion.hpp>
  #include <sfc/coprocessor/coprocessor.hpp>
  #include <sfc/slot/slot.hpp>
  #include <sfc/cartridge/cartridge.hpp>

  #include <sfc/memory/memory-inline.hpp>
  #include <sfc/ppu/counter/counter-inline.hpp>
}

#include <sfc/interface/interface.hpp>
