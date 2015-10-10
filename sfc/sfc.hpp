#ifndef SFC_HPP
#define SFC_HPP

#include <emulator/emulator.hpp>
#include <processor/arm/arm.hpp>
#include <processor/gsu/gsu.hpp>
#include <processor/hg51b/hg51b.hpp>
#include <processor/r65816/r65816.hpp>
#include <processor/spc700/spc700.hpp>
#include <processor/upd96050/upd96050.hpp>

namespace SuperFamicom {
  namespace Info {
    static const string Name = "bsnes";
    static const unsigned SerializerVersion = 29;
  }
}

/*
  bsnes - Super Famicom emulator
  author: byuu
  license: GPLv3
  project started: 2004-10-14
*/

#include <libco/libco.h>
#include <gb/gb.hpp>

namespace SuperFamicom {
  struct Thread {
    ~Thread() {
      if(thread) co_delete(thread);
    }

    auto create(auto (*entrypoint)() -> void, unsigned frequency) -> void {
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
    int64 clock = 0;
  };

  #include <sfc/memory/memory.hpp>
  #include <sfc/ppu/counter/counter.hpp>

  #if defined(PROFILE_ACCURACY)
  #include "profile-accuracy.hpp"
  #elif defined(PROFILE_BALANCED)
  #include "profile-balanced.hpp"
  #elif defined(PROFILE_PERFORMANCE)
  #include "profile-performance.hpp"
  #endif

  #include <sfc/controller/controller.hpp>
  #include <sfc/system/system.hpp>
  #include <sfc/base/base.hpp>
  #include <sfc/chip/chip.hpp>
  #include <sfc/slot/slot.hpp>
  #include <sfc/cartridge/cartridge.hpp>
  #include <sfc/cheat/cheat.hpp>
  #include <sfc/interface/interface.hpp>

  #include <sfc/memory/memory-inline.hpp>
  #include <sfc/ppu/counter/counter-inline.hpp>
}

#endif
