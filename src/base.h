#define BSNES_VERSION "0.034.03"
#define BSNES_TITLE   "bsnes v" BSNES_VERSION

#define BUSCORE sBus
#define CPUCORE sCPU
#define SMPCORE sSMP
#define DSPCORE sDSP
#define PPUCORE bPPU

//S-DSP can be encapsulated into a state machine using #define magic
//this avoids ~2.048m co_switch() calls per second (~5% speedup)
#define USE_STATE_MACHINE

//FAST_FRAMESKIP disables calculation of RTO during frameskip
//frameskip offers near-zero speedup if RTO is calculated
//accuracy is not affected by this define when frameskipping is off
#define FAST_FRAMESKIP

//game genie + pro action replay code support (~2% speed hit)
#define CHEAT_SYSTEM

#include <nall/algorithm.hpp>
#include <nall/array.hpp>
#include <nall/bit.hpp>
#include <nall/config.hpp>
#include <nall/detect.hpp>
#include <nall/function.hpp>
#include <nall/modulo.hpp>
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

#include "interface.h"
