#ifndef NES_HPP
#define NES_HPP

namespace NES {
  namespace Info {
    static const char Name[] = "bnes";
    static const unsigned SerializerVersion = 1;
  }
}

/*
  bnes - NES emulator
  authors: byuu, Ryphecha
  license: GPLv3
  project started: 2011-09-05
*/

#include <libco/libco.h>

#include <nall/platform.hpp>
#include <nall/algorithm.hpp>
#include <nall/array.hpp>
#include <nall/crc32.hpp>
#include <nall/dl.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/function.hpp>
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

  typedef uint_t< 1> uint1;
  typedef uint_t< 2> uint2;
  typedef uint_t< 3> uint3;
  typedef uint_t< 4> uint4;
  typedef uint_t< 5> uint5;
  typedef uint_t< 6> uint6;
  typedef uint_t< 7> uint7;
  typedef uint8_t    uint8;

  typedef uint_t< 9> uint9;
  typedef uint_t<10> uint10;
  typedef uint_t<11> uint11;
  typedef uint_t<12> uint12;
  typedef uint_t<13> uint13;
  typedef uint_t<14> uint14;
  typedef uint_t<15> uint15;
  typedef uint16_t   uint16;

  typedef uint_t<17> uint17;
  typedef uint_t<18> uint18;
  typedef uint_t<19> uint19;
  typedef uint_t<20> uint20;
  typedef uint_t<21> uint21;
  typedef uint_t<22> uint22;
  typedef uint_t<23> uint23;
  typedef uint_t<24> uint24;
  typedef uint_t<25> uint25;
  typedef uint_t<26> uint26;
  typedef uint_t<27> uint27;
  typedef uint_t<28> uint28;
  typedef uint_t<29> uint29;
  typedef uint_t<30> uint30;
  typedef uint_t<31> uint31;
  typedef uint32_t   uint32;

  typedef uint64_t   uint64;

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

  #include <nes/system/system.hpp>
  #include <nes/scheduler/scheduler.hpp>
  #include <nes/input/input.hpp>
  #include <nes/memory/memory.hpp>
  #include <nes/cartridge/cartridge.hpp>
  #include <nes/cpu/cpu.hpp>
  #include <nes/apu/apu.hpp>
  #include <nes/ppu/ppu.hpp>
  #include <nes/cheat/cheat.hpp>
  #include <nes/interface/interface.hpp>
}

#endif
