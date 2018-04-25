#pragma once

//license: GPLv3
//started: 2016-01-26

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <processor/v30mz/v30mz.hpp>

namespace WonderSwan {
  #define platform Emulator::platform
  namespace File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  using Cheat = Emulator::Cheat;
  extern Scheduler scheduler;
  extern Cheat cheat;

  enum : uint { Byte = 1, Word = 2, Long = 4 };

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
    inline static auto WonderSwan() -> bool;         //SW-001 (ASWAN)
    inline static auto WonderSwanColor() -> bool;    //WSC-001 (SPHINX)
    inline static auto SwanCrystal() -> bool;        //SCT-001 (SPHINX2)
    inline static auto PocketChallengeV2() -> bool;  //(ASWAN)
  };

  #include <ws/memory/memory.hpp>
  #include <ws/eeprom/eeprom.hpp>
  #include <ws/system/system.hpp>
  #include <ws/cartridge/cartridge.hpp>
  #include <ws/cpu/cpu.hpp>
  #include <ws/ppu/ppu.hpp>
  #include <ws/apu/apu.hpp>
}

#include <ws/interface/interface.hpp>
