#ifndef NES_HPP
#define NES_HPP

namespace NES {
  namespace Info {
    static const char Name[] = "bnes";
    static const char Version[] = "000.03";
  }
}

/*
  bnes - NES emulator
  author: byuu
  license: GPLv2
  project started: 2011-09-05
*/

#include <libco/libco.h>

#include <nall/algorithm.hpp>
#include <nall/array.hpp>
#include <nall/detect.hpp>
#include <nall/dl.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/foreach.hpp>
#include <nall/function.hpp>
#include <nall/platform.hpp>
#include <nall/property.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
using namespace nall;

namespace NES {
  typedef int8_t  int8;
  typedef int16_t int16;
  typedef int32_t int32;
  typedef int64_t int64;

  typedef uint8_t  uint8;
  typedef uint16_t uint16;
  typedef uint32_t uint32;
  typedef uint64_t uint64;

  typedef uint_t<15> uint15;

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

    inline Processor() : thread(0) {}
  };

  #include <nes/interface/interface.hpp>
  #include <nes/system/system.hpp>
  #include <nes/scheduler/scheduler.hpp>
  #include <nes/cartridge/cartridge.hpp>
  #include <nes/memory/memory.hpp>
  #include <nes/cpu/cpu.hpp>
  #include <nes/ppu/ppu.hpp>
}

#endif
