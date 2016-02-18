#pragma once

#include <emulator/emulator.hpp>
#include <processor/v30mz/v30mz.hpp>

namespace WonderSwan {
  namespace Info {
    static const string Name = "bws";
    static const uint SerializerVersion = 0;
  }
}

/*
  bws - WonderSwan, WonderSwan Color, and SwanCrystal emulator
  author: byuu
  license: GPLv3
  project started: 2016-01-26
*/

#include <libco/libco.h>

namespace WonderSwan {
  struct Thread {
    ~Thread() {
      if(thread) co_delete(thread);
    }

    auto create(auto (*entrypoint)() -> void, uint frequency) -> void {
      if(thread) co_delete(thread);
      thread = co_create(65536 * sizeof(void*), entrypoint);
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

  #include <ws/memory/memory.hpp>
  #include <ws/eeprom/eeprom.hpp>
  #include <ws/system/system.hpp>
  #include <ws/scheduler/scheduler.hpp>
  #include <ws/cartridge/cartridge.hpp>
  #include <ws/cpu/cpu.hpp>
  #include <ws/ppu/ppu.hpp>
  #include <ws/apu/apu.hpp>

  inline auto WS() { return system.revision() == System::Revision::WonderSwan; }
  inline auto WSC() { return system.revision() == System::Revision::WonderSwanColor; }
  inline auto SC() { return system.revision() == System::Revision::SwanCrystal; }
}

#include <ws/interface/interface.hpp>
