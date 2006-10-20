#define BSNES_VERSION "0.018.04"
#define BSNES_TITLE   "bsnes v" BSNES_VERSION

#define MEMCORE bMemBus
#define CPUCORE sCPU
#define SMPCORE sSMP
#define DSPCORE bDSP
#define PPUCORE bPPU

//#define FAVOR_ACCURACY
#define FAVOR_SPEED

//game genie + pro action replay code support (~1-3% speed hit)
#define CHEAT_SYSTEM

//enable GZ, ZIP format support
//#define GZIP_SUPPORT

//enable JMA support
//#define JMA_SUPPORT

//snes core polymorphism
//(allow runtime cpu/smp/dsp/ppu/bus selection, ~10% speed hit)
//#define POLYMORPHISM

#if defined(PROCESSOR_X86)
  #define ARCH_LSB
#elif defined(PROCESSOR_X86_64)
  #define ARCH_LSB
#elif defined(PROCESSOR_G5)
  #define ARCH_MSB
#else
  #error "unsupported processor"
#endif

#include "lib/libbase.h"
#include "lib/libco_x86.h"
#include "lib/libarray.h"
#include "lib/libvector.h"
#include "lib/libfile.h"
#include "lib/libups.h"
#include "lib/libstring.h"
#include "lib/libconfig.h"

inline uint16 read16(uint8 *addr, uint pos) {
#ifdef ARCH_LSB
  return *((uint16*)(addr + pos));
#else
  return (addr[pos]) | (addr[pos + 1] << 8);
#endif
}

//platform-specific global functions
void alert(char *, ...);
void dprintf(char *, ...);
void dprintf(uint, char *, ...);

namespace source {
  enum {
    none = 0,
    debug,
    cpu,
    ppu,
    smp,
    dsp,
    bus,
  };
};

//various class interfaces
#include "interface.h"
