#pragma once

//license: GPLv3
//started: 2016-07-08

#include <emulator/emulator.hpp>
#include <processor/m68k/m68k.hpp>
#include <processor/z80/z80.hpp>

namespace MegaDrive {
  using File = Emulator::File;

  struct Thread {
    ~Thread() {
      if(thread) co_delete(thread);
    }

    auto create(auto (*entrypoint)() -> void, uint frequency) -> void {
      if(thread) co_delete(thread);
      thread = co_create(65'536 * sizeof(void*), entrypoint);
      this->frequency = frequency;
      clock = 0;
    }

    auto serialize(serializer& s) -> void {
      s.integer(frequency);
      s.integer(clock);
    }

    cothread_t thread = nullptr;
    uint frequency = 0;
    int64 clock = 0;
  };

  #include <md/cpu/cpu.hpp>
  #include <md/apu/apu.hpp>
  #include <md/vdp/vdp.hpp>
  #include <md/ym2612/ym2612.hpp>

  #include <md/system/system.hpp>
  #include <md/scheduler/scheduler.hpp>
  #include <md/cartridge/cartridge.hpp>
}

#include <md/interface/interface.hpp>
