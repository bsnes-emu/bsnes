#pragma once

//license: GPLv3
//started: 2011-09-05

#include <emulator/emulator.hpp>
#include <processor/r6502/r6502.hpp>

namespace Famicom {
  namespace Info {
    static const uint SerializerVersion = 2;
  }
}

#include <libco/libco.h>

namespace Famicom {
  struct Thread {
    ~Thread() {
      if(thread) co_delete(thread);
    }

    auto create(auto (*entrypoint)() -> void, uint frequency) -> void {
      if(thread) co_delete(thread);
      thread = co_create(262'144 * sizeof(void*), entrypoint);
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

  #include <fc/system/system.hpp>
  #include <fc/scheduler/scheduler.hpp>
  #include <fc/input/input.hpp>
  #include <fc/memory/memory.hpp>
  #include <fc/cartridge/cartridge.hpp>
  #include <fc/cpu/cpu.hpp>
  #include <fc/apu/apu.hpp>
  #include <fc/ppu/ppu.hpp>
  #include <fc/cheat/cheat.hpp>
}

#include <fc/interface/interface.hpp>
