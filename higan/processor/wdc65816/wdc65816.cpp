#include <processor/processor.hpp>
#include "wdc65816.hpp"

namespace Processor {

#define PC r.pc
#define A r.a
#define X r.x
#define Y r.y
#define Z r.z
#define S r.s
#define D r.d
#define B r.b
#define P r.p

#define CF r.p.c
#define ZF r.p.z
#define IF r.p.i
#define DF r.p.d
#define XF r.p.x
#define MF r.p.m
#define VF r.p.v
#define NF r.p.n
#define EF r.e

#define E if(r.e)
#define N if(!r.e)
#define L lastCycle();

#define lo(n) n.byte(0)
#define hi(n) n.byte(1)
#define db(n) n.byte(2)
#define aa(n) n.bits(0,15)
#define alu(...) (this->*op)(__VA_ARGS__)

#include "memory.cpp"
#include "algorithms.cpp"

#include "instructions-read.cpp"
#include "instructions-write.cpp"
#include "instructions-modify.cpp"
#include "instructions-pc.cpp"
#include "instructions-other.cpp"
#include "instruction.cpp"

auto WDC65816::power() -> void {
  PC = 0x000000;
  A  = 0x0000;
  X  = 0x0000;
  Y  = 0x0000;
  S  = 0x01ff;
  D  = 0x0000;
  B  = 0x00;
  P  = 0x34;
  EF = 1;

  r.mdr = 0x00;
  r.wai = false;
  r.stp = false;
  r.vector = 0xfffc;  //reset vector address
}

#undef PC
#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
#undef B
#undef P

#undef CF
#undef ZF
#undef IF
#undef DF
#undef XF
#undef MF
#undef VF
#undef NF
#undef EF

#undef E
#undef N
#undef L

#undef lo
#undef hi
#undef db
#undef aa
#undef alu

#include "serialization.cpp"
#include "disassembler.cpp"

}
