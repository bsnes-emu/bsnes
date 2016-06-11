#pragma once

//license: GPLv3
//started: 2016-01-26

#include <emulator/emulator.hpp>
#include <processor/v30mz/v30mz.hpp>

namespace WonderSwan {
  namespace Info {
    static const uint SerializerVersion = 2;
  }
}

#include <libco/libco.h>

namespace WonderSwan {
  enum class Model : uint {
    WonderSwan,       //SW-001  (ASWAN)
    WonderSwanColor,  //WSC-001 (SPHINX)
    SwanCrystal,      //SCT-001 (SPHINX2)
  };

  enum : uint { Byte = 1, Word = 2, Long = 4 };

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

  #include <ws/memory/memory.hpp>
  #include <ws/eeprom/eeprom.hpp>
  #include <ws/system/system.hpp>
  #include <ws/scheduler/scheduler.hpp>
  #include <ws/cartridge/cartridge.hpp>
  #include <ws/cpu/cpu.hpp>
  #include <ws/ppu/ppu.hpp>
  #include <ws/apu/apu.hpp>
  #include <ws/cheat/cheat.hpp>
}

#include <ws/interface/interface.hpp>
