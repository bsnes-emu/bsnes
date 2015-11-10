#ifndef GBA_HPP
#define GBA_HPP

#include <emulator/emulator.hpp>
#include <processor/arm/arm.hpp>

namespace GameBoyAdvance {
  namespace Info {
    static const string Name = "bgba";
    static const uint SerializerVersion = 3;
  }
}

/*
  bgba - Game Boy Advance emulator
  authors: byuu, Cydrak
  license: GPLv3
  project started: 2012-03-19
*/

#include <libco/libco.h>

namespace GameBoyAdvance {
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
    int clock = 0;
  };

  #include <gba/memory/memory.hpp>
  #include <gba/interface/interface.hpp>
  #include <gba/scheduler/scheduler.hpp>
  #include <gba/system/system.hpp>
  #include <gba/cartridge/cartridge.hpp>
  #include <gba/player/player.hpp>
  #include <gba/cpu/cpu.hpp>
  #include <gba/ppu/ppu.hpp>
  #include <gba/apu/apu.hpp>
  #include <gba/video/video.hpp>
}

#endif
