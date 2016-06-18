#pragma once

//license: GPLv3
//started: 2011-09-05

#include <emulator/emulator.hpp>
#include <processor/r6502/r6502.hpp>

namespace Famicom {
  namespace Info {
    static const uint SerializerVersion = 3;
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

  struct Cothread : Thread {
    auto step(uint clocks) -> void;
    auto synchronizeCPU() -> void;
  };

  #include <fc/scheduler/scheduler.hpp>
  #include <fc/controller/controller.hpp>
  #include <fc/system/system.hpp>
  #include <fc/memory/memory.hpp>
  #include <fc/cartridge/cartridge.hpp>
  #include <fc/cpu/cpu.hpp>
  #include <fc/apu/apu.hpp>
  #include <fc/ppu/ppu.hpp>
  #include <fc/cheat/cheat.hpp>

  inline auto Cothread::step(uint clocks) -> void {
    clock += clocks * (uint64)cpu.frequency;
    synchronizeCPU();
  }

  inline auto Cothread::synchronizeCPU() -> void {
    if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
  }
}

#include <fc/interface/interface.hpp>
