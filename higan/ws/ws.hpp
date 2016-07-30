#pragma once

//license: GPLv3
//started: 2016-01-26

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/v30mz/v30mz.hpp>

namespace WonderSwan {
  using File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
  extern Cheat cheat;

  enum class Model : uint {
    WonderSwan,       //SW-001  (ASWAN)
    WonderSwanColor,  //WSC-001 (SPHINX)
    SwanCrystal,      //SCT-001 (SPHINX2)
  };

  enum : uint { Byte = 1, Word = 2, Long = 4 };

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void;
    auto synchronize(Thread& thread) -> void;
    auto step(uint clocks) -> void;
  };

  #include <ws/memory/memory.hpp>
  #include <ws/eeprom/eeprom.hpp>
  #include <ws/system/system.hpp>
  #include <ws/cartridge/cartridge.hpp>
  #include <ws/cpu/cpu.hpp>
  #include <ws/ppu/ppu.hpp>
  #include <ws/apu/apu.hpp>

  inline auto Thread::create(auto (*entrypoint)() -> void, double frequency) -> void {
    Emulator::Thread::create(entrypoint, frequency);
    scheduler.append(*this);
  }

  inline auto Thread::synchronize(Thread& thread) -> void {
    if(_clock > thread._clock) scheduler.resume(thread);
  }

  inline auto Thread::step(uint clocks) -> void {
    _clock += clocks;
  }
}

#include <ws/interface/interface.hpp>
