#ifndef GB_HPP
#define GB_HPP

#include <emulator/emulator.hpp>
#include <processor/lr35902/lr35902.hpp>

namespace GameBoy {
  namespace Info {
    static const string Name = "bgb";
    static const unsigned SerializerVersion = 4;
  }
}

/*
  bgb - Game Boy, Super Game Boy, and Game Boy Color emulator
  author: byuu
  license: GPLv3
  project started: 2010-12-27
*/

#include <libco/libco.h>

namespace GameBoy {
  struct Thread {
    ~Thread() {
      if(thread) co_delete(thread);
    }

    auto create(void (*entrypoint)(), unsigned frequency) -> void {
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
    unsigned frequency = 0;
    int64_t clock = 0;
  };

  #include <gb/memory/memory.hpp>
  #include <gb/system/system.hpp>
  #include <gb/scheduler/scheduler.hpp>
  #include <gb/cartridge/cartridge.hpp>
  #include <gb/cpu/cpu.hpp>
  #include <gb/ppu/ppu.hpp>
  #include <gb/apu/apu.hpp>
  #include <gb/cheat/cheat.hpp>
  #include <gb/video/video.hpp>
};

#endif
