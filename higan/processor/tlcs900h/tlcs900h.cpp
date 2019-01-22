#include <processor/processor.hpp>
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

auto TLCS900H::interrupt(uint24 address) -> void {
  push(PC);
  push(SR);
  store(PC, load(Memory<uint24>{address}));
  store(INTNEST, load(INTNEST) + 1);
}

auto TLCS900H::power() -> void {
  r = {};
  r.xsp.l.l0 = 0x100;
}

}
