#pragma once

//license: GPLv3
//started: 2018-12-28

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/z80/z80.hpp>

namespace MSX {
  #define platform Emulator::platform
  namespace File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
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

  struct Model {
    inline static auto MSX() -> bool;
    inline static auto MSX2() -> bool;
    inline static auto MSX2Plus() -> bool;
    inline static auto MSXTurboR() -> bool;
  };

  struct Region {
    inline static auto NTSC() -> bool;
    inline static auto PAL() -> bool;
  };

  #include <msx/system/system.hpp>
  #include <msx/cartridge/cartridge.hpp>

  #include <msx/cpu/cpu.hpp>
  #include <msx/vdp/vdp.hpp>
  #include <msx/psg/psg.hpp>
}

#include <msx/interface/interface.hpp>
