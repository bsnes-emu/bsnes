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

  struct ChipDebugger {
    virtual bool property(unsigned id, string &name, string &value) = 0;
  };

  #include "memory/memory.hpp"
  #include "memory/smemory/smemory.hpp"

  #include "ppu/ppu.hpp"
  #include "ppu/bppu/bppu.hpp"

  #include "cpu/cpu.hpp"
  #include "cpu/core/core.hpp"
  #include "cpu/scpu/scpu.hpp"

  #include "smp/smp.hpp"
  #include "smp/core/core.hpp"
  #include "smp/ssmp/ssmp.hpp"

  #include "dsp/dsp.hpp"
  #include "dsp/sdsp/sdsp.hpp"

  #include "system/system.hpp"
  #include "chip/chip.hpp"
  #include "cartridge/cartridge.hpp"
  #include "cheat/cheat.hpp"

  #include "memory/memory-inline.hpp"
  #include "ppu/ppu-inline.hpp"
  #include "cheat/cheat-inline.hpp"
}

namespace nall {
  template<> struct has_size<SNES::MappedRAM> { enum { value = true }; };
  template<> struct has_size<SNES::StaticRAM> { enum { value = true }; };
}

#undef debugvirtual
