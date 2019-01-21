#pragma once

//license: GPLv3
//started: 2019-01-03

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/tlcs900h/tlcs900h.hpp>
#include <processor/z80/z80.hpp>

namespace NeoGeoPocket {
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
    inline static auto NeoGeoPocket() -> bool;
    inline static auto NeoGeoPocketColor() -> bool;
  };

  #include <ngp/system/system.hpp>
  #include <ngp/cartridge/cartridge.hpp>
  #include <ngp/cpu/cpu.hpp>
  #include <ngp/apu/apu.hpp>
  #include <ngp/vpu/vpu.hpp>
  #include <ngp/psg/psg.hpp>
}

#include <ngp/interface/interface.hpp>
