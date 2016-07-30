#pragma once

//license: GPLv3
//started: 2011-09-05

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/r6502/r6502.hpp>

namespace Famicom {
  using File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
  extern Cheat cheat;

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void;
    auto synchronize(Thread& thread) -> void;
  };

  #include <fc/controller/controller.hpp>
  #include <fc/system/system.hpp>
  #include <fc/memory/memory.hpp>
  #include <fc/cartridge/cartridge.hpp>
  #include <fc/cpu/cpu.hpp>
  #include <fc/apu/apu.hpp>
  #include <fc/ppu/ppu.hpp>

  inline auto Thread::create(auto (*entrypoint)() -> void, double frequency) -> void {
    Emulator::Thread::create(entrypoint, frequency);
    scheduler.append(*this);
  }

  inline auto Thread::synchronize(Thread& thread) -> void {
    if(_clock > thread._clock) scheduler.resume(thread);
  }
}

#include <fc/interface/interface.hpp>
