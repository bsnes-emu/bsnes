#define BSNES_VERSION "0.028"
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

#if !defined(ARCH_LSB) && !defined(ARCH_MSB)
  #define ARCH_LSB //guess
#endif

#include <nall/algorithm.hpp>
#include <nall/array.hpp>
#include <nall/bit.hpp>
#include <nall/config.hpp>
#include <nall/function.hpp>
#include <nall/new.hpp>
#include <nall/sort.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
using namespace nall;

#include <libco.h>
#include <bbase.h>

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
