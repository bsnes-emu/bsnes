//bgameboy
//author: byuu
//project started: 2010-12-27

namespace GameBoy {
  namespace Info {
    static const char Name[] = "bgameboy";
    static const char Version[] = "000.14";
    static unsigned SerializerVersion = 1;
  }
}

#include <libco/libco.h>

#include <nall/foreach.hpp>
#include <nall/platform.hpp>
#include <nall/property.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
using namespace nall;

namespace GameBoy {
  typedef int8_t   int8;
  typedef int16_t  int16;
  typedef int32_t  int32;
  typedef int64_t  int64;

  typedef uint8_t  uint8;
  typedef uint16_t uint16;
  typedef uint32_t uint32;
  typedef uint64_t uint64;

  template<uint16 lo, uint16 hi>
  alwaysinline bool within(uint16 addr) {
    static const uint16 mask = ~(hi ^ lo);
    return (addr & mask) == lo;
  }

  struct Processor {
    cothread_t thread;
    unsigned frequency;
    int64 clock;

    inline void create(void (*entrypoint_)(), unsigned frequency_) {
      if(thread) co_delete(thread);
      thread = co_create(65536 * sizeof(void*), entrypoint_);
      frequency = frequency_;
      clock = 0;
    }

    inline Processor() : thread(0) {}
  };

  #include <gameboy/memory/memory.hpp>
  #include <gameboy/system/system.hpp>
  #include <gameboy/scheduler/scheduler.hpp>
  #include <gameboy/cartridge/cartridge.hpp>
  #include <gameboy/cpu/cpu.hpp>
  #include <gameboy/apu/apu.hpp>
  #include <gameboy/lcd/lcd.hpp>
};
