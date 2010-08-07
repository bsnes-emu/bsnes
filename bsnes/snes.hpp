namespace SNES {
  namespace Info {
    static const char Name[] = "bsnes";
    static const char Version[] = "067.06";
    static const unsigned SerializerVersion = 12;
  }
}

#define CORE_SMEMORY
#define CORE_SCPU
#define CORE_SSMP
#define CORE_SDSP
#define CORE_BPPU

//S-DSP can be encapsulated into a state machine using #define magic
//this avoids ~2.048m co_switch() calls per second (~5% speedup)
#define DSP_STATE_MACHINE

//game genie + pro action replay code support (~2% speed hit)
#define CHEAT_SYSTEM

//enable debugging extensions (~15% speed hit)
//#define DEBUGGER

#include <libco/libco.h>

#include <nall/algorithm.hpp>
#include <nall/any.hpp>
#include <nall/array.hpp>
#include <nall/detect.hpp>
#include <nall/dl.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/foreach.hpp>
#include <nall/function.hpp>
#include <nall/moduloarray.hpp>
#include <nall/platform.hpp>
#include <nall/property.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>
using namespace nall;

#ifdef DEBUGGER
  #define debugvirtual virtual
#else
  #define debugvirtual
#endif

namespace SNES {
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

    inline void serialize(serializer &s) {
      s.integer(frequency);
      s.integer(clock);
    }

    inline Processor() : thread(0) {}
  };

  struct ChipDebugger {
    virtual bool property(unsigned id, string &name, string &value) = 0;
  };

  #include <memory/memory.hpp>
  #if defined(CORE_SMEMORY)
  #include <memory/smemory/smemory.hpp>
  #endif

  #include <ppu/ppu.hpp>
  #if defined(CORE_SPPU)
  #include <ppu/sppu/sppu.hpp>
  #elif defined(CORE_BPPU)
  #include <ppu/bppu/bppu.hpp>
  #endif

  #include <cpu/cpu.hpp>
  #include <cpu/core/core.hpp>
  #if defined(CORE_SCPU)
  #include <cpu/scpu/scpu.hpp>
  #endif

  #include <smp/smp.hpp>
  #include <smp/core/core.hpp>
  #if defined(CORE_SSMP)
  #include <smp/ssmp/ssmp.hpp>
  #endif

  #include <dsp/dsp.hpp>
  #if defined(CORE_SDSP)
  #include <dsp/sdsp/sdsp.hpp>
  #elif defined(CORE_ADSP)
  #include <dsp/adsp/adsp.hpp>
  #endif

  #include <system/system.hpp>
  #include <chip/chip.hpp>
  #include <cartridge/cartridge.hpp>
  #include <cheat/cheat.hpp>

  #include <cpu/synchronization.hpp>
  #include <smp/synchronization.hpp>
  #include <dsp/synchronization.hpp>
  #include <ppu/synchronization.hpp>

  #include <memory/memory-inline.hpp>
  #include <ppu/ppu-inline.hpp>
  #include <cheat/cheat-inline.hpp>
}

namespace nall {
  template<> struct has_size<SNES::MappedRAM> { enum { value = true }; };
  template<> struct has_size<SNES::StaticRAM> { enum { value = true }; };
}

#undef debugvirtual
