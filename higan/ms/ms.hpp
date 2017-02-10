#pragma once

//license: GPLv3
//started: 2016-08-17

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>

#include <processor/z80/z80.hpp>

namespace MasterSystem {
  #define platform Emulator::platform
  namespace File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  extern Scheduler scheduler;
  struct Interface;

  enum class Model : uint {
    MasterSystem,
    GameGear,
  };

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void {
      Emulator::Thread::create(entrypoint, frequency);
      scheduler.append(*this);
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }
  };

  #include <ms/controller/controller.hpp>

  #include <ms/cpu/cpu.hpp>
  #include <ms/vdp/vdp.hpp>
  #include <ms/psg/psg.hpp>

  #include <ms/system/system.hpp>
  #include <ms/cartridge/cartridge.hpp>
  #include <ms/bus/bus.hpp>
}

#include <ms/interface/interface.hpp>
