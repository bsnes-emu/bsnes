#pragma once

//license: GPLv3
//started: 2017-01-11

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>

#include <processor/huc6280/huc6280.hpp>

namespace PCEngine {
  #define platform Emulator::platform
  using File = Emulator::File;
  using Scheduler = Emulator::Scheduler;
  extern Scheduler scheduler;

  struct Thread : Emulator::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void {
      Emulator::Thread::create(entrypoint, frequency);
      scheduler.append(*this);
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }
  };

  #include <pce/controller/controller.hpp>

  #include <pce/cpu/cpu.hpp>
  #include <pce/vdc/vdc.hpp>
  #include <pce/psg/psg.hpp>

  #include <pce/system/system.hpp>
  #include <pce/cartridge/cartridge.hpp>
}

#include <pce/interface/interface.hpp>
