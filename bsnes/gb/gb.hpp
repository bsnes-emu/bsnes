#ifndef GB_HPP
#define GB_HPP

#include <base/base.hpp>

namespace GB {
  namespace Info {
    static const char Name[] = "bsgbc";
    static const unsigned SerializerVersion = 3;
  }
}

/*
  bsgbc - Game Boy, Super Game Boy, and Game Boy Color emulator
  author: byuu
  license: GPLv3
  project started: 2010-12-27
*/

#include <libco/libco.h>
#include <nall/gameboy/cartridge.hpp>

namespace GB {
  struct Processor {
    cothread_t thread;
    unsigned frequency;
    int64 clock;

    inline void create(void (*entrypoint)(), unsigned frequency) {
      if(thread) co_delete(thread);
      thread = co_create(65536 * sizeof(void*), entrypoint);
      this->frequency = frequency;
      clock = 0;
    }

    inline void serialize(serializer &s) {
      s.integer(frequency);
      s.integer(clock);
    }

    inline Processor() : thread(nullptr) {
    }

    inline ~Processor() {
      if(thread) co_delete(thread);
    }
  };

  #include <gb/memory/memory.hpp>
  #include <gb/system/system.hpp>
  #include <gb/scheduler/scheduler.hpp>
  #include <gb/cartridge/cartridge.hpp>
  #include <gb/cpu/cpu.hpp>
  #include <gb/apu/apu.hpp>
  #include <gb/lcd/lcd.hpp>
  #include <gb/cheat/cheat.hpp>
  #include <gb/video/video.hpp>
};

#endif
