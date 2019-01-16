#include <processor/processor.hpp>
#include <nall/inline-if.hpp>
#include "tlcs900h.hpp"

namespace Processor {

#define CF r.c
#define NF r.n
#define VF r.v
#define PF r.v
#define HF r.h
#define ZF r.z
#define SF r.s

#define RFP r.rfp
#define IFF r.iff

#include "registers.cpp"
#include "control-registers.cpp"
#include "memory.cpp"
#include "conditions.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

TLCS900H tlcs900h;

auto TLCS900H::power() -> void {
  r = {};
}

TLCS900H::TLCS900H() {
}

}
