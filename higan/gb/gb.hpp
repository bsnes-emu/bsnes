#pragma once

//license: GPLv3
//started: 2010-12-27

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/lr35902/lr35902.hpp>

namespace GameBoy {
  using File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
  extern Cheat cheat;

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency, bool resetClock) -> void;
    auto synchronize(Thread& thread) -> void;
  };

  #include <gb/memory/memory.hpp>
  #include <gb/system/system.hpp>
  #include <gb/cartridge/cartridge.hpp>
  #include <gb/cpu/cpu.hpp>
  #include <gb/ppu/ppu.hpp>
  #include <gb/apu/apu.hpp>

  inline auto Thread::create(auto (*entrypoint)() -> void, double frequency, bool resetClock = true) -> void {
    Emulator::Thread::create(entrypoint, frequency, resetClock);
    scheduler.append(*this);
  }

  inline auto Thread::synchronize(Thread& thread) -> void {
    if(_clock > thread._clock) scheduler.resume(thread);
  }
}

#include <gb/interface/interface.hpp>
