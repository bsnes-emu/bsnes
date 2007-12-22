#define BSNES_VERSION "0.027"
#define BSNES_TITLE   "bsnes v" BSNES_VERSION

#define BUSCORE sBus
#define CPUCORE sCPU
#define SMPCORE sSMP
#define DSPCORE bDSP
#define PPUCORE bPPU

//FAVOR_ACCURACY calculates RTO during frameskip, whereas FAVOR_SPEED does not
//frameskip offers near-zero speedup if RTO is calculated
//accuracy is not affected by this define when frameskipping is off

//#define FAVOR_ACCURACY
#define FAVOR_SPEED

//game genie + pro action replay code support (~1-3% speed hit)
#define CHEAT_SYSTEM

#if defined(PROCESSOR_X86) || defined(PROCESSOR_X86_64)
  #define ARCH_LSB
#elif defined(PROCESSOR_PPC) || defined(PROCESSOR_PPC64)
  #define ARCH_MSB
#else //guess
  #define ARCH_LSB
#endif

#include "lib/libco.h"
#include "lib/bbase.h"
#include "lib/bfunction.h"
#include "lib/barray.h"
#include "lib/bvector.h"
#include "lib/bkeymap.h"
#include "lib/bstring.h"
#include "lib/bconfig.h"

//platform-specific global functions
void alert(const char*, ...);
void dprintf(const char*, ...);
void dprintf(uint, const char*, ...);

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

#include "interface.h"
