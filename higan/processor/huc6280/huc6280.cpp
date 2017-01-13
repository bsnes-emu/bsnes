#include <processor/processor.hpp>
#include "huc6280.hpp"

namespace Processor {

#define A r.a
#define X r.x
#define Y r.y
#define S r.s
#define PC r.pc
#define PCH r.pc.byte(1)
#define PCL r.pc.byte(0)
#define P r.p
#define C r.p.c
#define Z r.p.z
#define I r.p.i
#define D r.p.d
#define B r.p.b
#define T r.p.t
#define V r.p.v
#define N r.p.n
#define EA r.ea
#define L lastCycle();
#define ALU (this->*alu)
#include "memory.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"
#undef A
#undef X
#undef Y
#undef S
#undef PC
#undef PCH
#undef PCL
#undef P
#undef C
#undef Z
#undef I
#undef D
#undef B
#undef T
#undef V
#undef N
#undef EA
#undef L
#undef ALU

auto HuC6280::power() -> void {
  r.a = 0x00;
  r.x = 0x00;
  r.y = 0x00;
  r.s = 0xff;
  r.pc = 0x0000;
  r.mpr[0] = 0xff;
  r.mpr[1] = 0xf8;
  r.mpr[2] = 0x00;
  r.mpr[3] = 0x00;
  r.mpr[4] = 0x00;
  r.mpr[5] = 0x00;
  r.mpr[6] = 0x00;
  r.mpr[7] = 0x00;
  r.mdr = 0x00;
  r.p = 0x04;
  r.cs = 3;
}

}
