#define BSNES_VERSION "0.015 rc2" //wip45
#define BSNES_TITLE "bsnes v" BSNES_VERSION

#define MEMCORE bMemBus
#define CPUCORE bCPU
#define APUCORE bAPU
#define DSPCORE bDSP
#define PPUCORE bPPU

//debugging extensions (~10% speed hit)
#define DEBUGGER

//game genie + pro action replay code support (~1-3% speed hit)
#define CHEAT_SYSTEM

//enable GZ, ZIP format support
//#define GZIP_SUPPORT

//enable JMA support
//#define JMA_SUPPORT

//snes core polymorphism (allow mem/cpu/apu/ppu overriding,
//~10% speed hit)
//#define POLYMORPHISM

//this should be declared in the port-specific makefiles
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
void alert(char *s, ...);
void dprintf(char *s, ...);

//various class interfaces
#include "interface.h"
