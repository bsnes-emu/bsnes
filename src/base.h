#define BSNES_VERSION "0.029"
#define BSNES_TITLE   "bsnes v" BSNES_VERSION

#define BUSCORE sBus
#define CPUCORE sCPU
#define SMPCORE sSMP
#define DSPCORE bDSP
#define PPUCORE bPPU

//FAST_FRAMESKIP disables calculation of RTO during frameskip
//frameskip offers near-zero speedup if RTO is calculated
//accuracy is not affected by this define when frameskipping is off
#define FAST_FRAMESKIP

//game genie + pro action replay code support (~1-3% speed hit)
#define CHEAT_SYSTEM

#include <nall/algorithm.hpp>
#include <nall/array.hpp>
#include <nall/bit.hpp>
#include <nall/config.hpp>
#include <nall/detect.hpp>
#include <nall/function.hpp>
#include <nall/new.hpp>
#include <nall/sort.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
using namespace nall;

#include <libco/libco.h>
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
