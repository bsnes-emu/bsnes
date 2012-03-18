#ifndef GBA_HPP
#define GBA_HPP

#include <base/base.hpp>

namespace GBA {
  namespace Info {
    static const char Name[] = "bgba";
    static const unsigned SerializerVersion = 1;
  }
}

/*
  bgba - Game Boy Advance emulator
  author: byuu
  license: GPLv3
  project started: 2012-03-17
*/

#include <libco/libco.h>

namespace GBA {
  struct Processor {
    cothread_t thread;
    unsigned frequency;
    signed clock;

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

  #include <gba/interface/interface.hpp>
  #include <gba/system/system.hpp>
  #include <gba/scheduler/scheduler.hpp>
  #include <gba/cartridge/cartridge.hpp>
  #include <gba/memory/memory.hpp>
  #include <gba/cpu/cpu.hpp>
  #include <gba/ppu/ppu.hpp>
  #include <gba/apu/apu.hpp>
  #include <gba/video/video.hpp>

  #include <gba/scheduler/scheduler-inline.hpp>
}

#endif
