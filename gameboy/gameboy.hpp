//bgameboy
//author: byuu
//project started: 2010-12-27

namespace GameBoy {
  namespace Info {
    static const char Name[] = "bgameboy";
    static const char Version[] = "000.05";
  }
}

#include <libco/libco.h>

#include <nall/foreach.hpp>
#include <nall/property.hpp>
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

  #include <system/system.hpp>
  #include <scheduler/scheduler.hpp>
  #include <memory/memory.hpp>
  #include <cartridge/cartridge.hpp>
  #include <cpu/cpu.hpp>
  #include <lcd/lcd.hpp>
};
