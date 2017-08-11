#pragma once

//license: GPLv3
//started: 2016-07-08

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/m68k/m68k.hpp>
#include <processor/z80/z80.hpp>

namespace MegaDrive {
  #define platform Emulator::platform
  namespace File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
  extern Cheat cheat;

  struct Wait {
    enum : uint {
      VDP_DMA = 1 << 0,
    };
  };

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void {
      Emulator::Thread::create(entrypoint, frequency);
      scheduler.append(*this);
      wait = 0;
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }

    uint wait = 0;
  };

  struct Region {
    inline static auto NTSCJ() -> bool;
    inline static auto NTSCU() -> bool;
    inline static auto PAL() -> bool;
  };

  #include <md/controller/controller.hpp>

  #include <md/cpu/cpu.hpp>
  #include <md/apu/apu.hpp>
  #include <md/vdp/vdp.hpp>
  #include <md/psg/psg.hpp>
  #include <md/ym2612/ym2612.hpp>

  #include <md/system/system.hpp>
  #include <md/cartridge/cartridge.hpp>
}

#include <md/interface/interface.hpp>
