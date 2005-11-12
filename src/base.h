//#define DEBUGGER
//#define POLYMORPHISM

//this should be declared in the port-specific makefiles
//#define ARCH_LSB
//#define ARCH_MSB

#ifndef ARCH_LSB
  #ifndef ARCH_MSB
    #define ARCH_LSB
  #endif
#endif

#include <time.h>
#include "lib/libbase.h"
#include "lib/libvector.h"
#include "lib/libstring.h"
#include "lib/libconfig.h"

inline uint16 read16(uint8 *addr, uint pos) {
#ifdef ARCH_LSB
  return *((uint16*)(addr + pos));
#else
  return (addr[pos]) | (addr[pos + 1] << 8);
#endif
}

#if defined(_WIN32)
  #define _WIN32_
  #undef _UNIX_
#elif defined(__GNUC__)
  #define _UNIX_
  #undef _WIN32_
#else
  #error "unknown architecture"
#endif

//platform-specific global functions
void *memalloc(uint32 size, char *name = 0, ...);
void memfree(void *mem, char *name = 0, ...);
void alert(char *s, ...);
void dprintf(char *s, ...);

//various class interfaces
#include "interface.h"
