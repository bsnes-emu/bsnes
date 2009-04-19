#define BSNES_VERSION "0.044"
#define BSNES_TITLE   "bsnes v" BSNES_VERSION

#define BUSCORE sBus
#define CPUCORE sCPU
#define SMPCORE sSMP
#define DSPCORE sDSP
#define PPUCORE bPPU

//S-DSP can be encapsulated into a state machine using #define magic
//this avoids ~2.048m co_switch() calls per second (~5% speedup)
#define DSP_STATE_MACHINE

//game genie + pro action replay code support (~2% speed hit)
#define CHEAT_SYSTEM

#include <libco/libco.h>

#include <nall/algorithm.hpp>
#include <nall/array.hpp>
#include <nall/bit.hpp>
#include <nall/detect.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/moduloarray.hpp>
#include <nall/new.hpp>
#include <nall/platform.hpp>
#include <nall/property.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>
using namespace nall;

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#include "interface.hpp"
