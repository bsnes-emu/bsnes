#pragma once

//license: GPLv3
//started: 2012-03-19

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>

#include <processor/arm/arm.hpp>

namespace GameBoyAdvance {
  #define platform Emulator::platform
  using File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  extern Scheduler scheduler;

  enum : uint {           //mode flags for bus read, write:
    Nonsequential =   1,  //N cycle
    Sequential    =   2,  //S cycle
    Prefetch      =   4,  //instruction fetch (eligible for prefetch)
    Byte          =   8,  //8-bit access
    Half          =  16,  //16-bit access
    Word          =  32,  //32-bit access
    Load          =  64,  //load operation
    Store         = 128,  //store operation
    Signed        = 256,  //sign extended
  };

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void {
      Emulator::Thread::create(entrypoint, frequency);
      scheduler.append(*this);
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }

    inline auto step(uint clocks) -> void {
      _clock += clocks;
    }
  };

  #include <gba/memory/memory.hpp>
  #include <gba/system/system.hpp>
  #include <gba/cartridge/cartridge.hpp>
  #include <gba/player/player.hpp>
  #include <gba/cpu/cpu.hpp>
  #include <gba/ppu/ppu.hpp>
  #include <gba/apu/apu.hpp>
}

#include <gba/interface/interface.hpp>
