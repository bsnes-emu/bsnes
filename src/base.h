#define BSNES_VERSION "0.016.27a"
#define BSNES_TITLE   "bsnes v" BSNES_VERSION

#define MEMCORE bMemBus
#define CPUCORE sCPU
#define APUCORE sAPU
#define DSPCORE bDSP
#define PPUCORE bPPU

//#define FAVOR_ACCURACY
#define FAVOR_SPEED

//game genie + pro action replay code support (~1-3% speed hit)
#define CHEAT_SYSTEM

//enable GZ, ZIP format support
#define GZIP_SUPPORT

//enable JMA support
#define JMA_SUPPORT

//debugging extensions (~10% speed hit)
//#define DEBUGGER

//snes core polymorphism
//(allow mem/cpu/apu/ppu overriding, ~10% speed hit)
//#define POLYMORPHISM

//this should be declared in the port-specific makefile
//#define ARCH_LSB
//#define ARCH_MSB

#ifndef ARCH_LSB
  #ifndef ARCH_MSB
    #define ARCH_LSB
  #endif
#endif

#if defined(_WIN32)
  #define _WIN32_
  #undef _UNIX_
#elif defined(__GNUC__)
  #define _UNIX_
  #undef _WIN32_
#else
  #error "unknown architecture"
#endif

#include "lib/libco_x86.h"
#include "lib/libbase.h"
#include "lib/libvector.h"
#include "lib/libstring.h"
#include "lib/libconfig.h"
#include "lib/libbpf.h"

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
    apu,
  };
};

//various class interfaces
#include "interface.h"
