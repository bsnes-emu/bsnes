#include <processor/processor.hpp>
#include "mos6502.hpp"

namespace Processor {

#define A r.a
#define X r.x
#define Y r.y
#define S r.s
#define P r.p
#define PC r.pc
#define PCH r.pc.byte(1)
#define PCL r.pc.byte(0)
#define ALU (this->*alu)
#define C r.p.c
#define Z r.p.z
#define I r.p.i
#define D r.p.d
#define V r.p.v
#define N r.p.n
#define L lastCycle();

#include "memory.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

#undef A
#undef X
#undef Y
#undef S
#undef P
#undef PC
#undef PCH
#undef PCL
#undef ALU
#undef C
#undef Z
#undef I
#undef D
#undef V
#undef N
#undef L

auto MOS6502::mdr() const -> uint8 {
  return r.mdr;
}

auto MOS6502::power() -> void {
  r.a = 0x00;
  r.x = 0x00;
  r.y = 0x00;
  r.s = 0xff;
  r.p = 0x04;
  r.mdr = 0x00;
}

}
