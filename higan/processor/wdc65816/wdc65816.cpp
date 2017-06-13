#include <processor/processor.hpp>
#include "wdc65816.hpp"

namespace Processor {

#define A r.a.w
#define X r.x.w
#define Y r.y.w
#define Z r.z.w
#define S r.s.w
#define D r.d.w
#define B r.db
#define P r.p
#define PC r.pc.d
#define CF r.p.c
#define ZF r.p.z
#define IF r.p.i
#define DF r.p.d
#define XF r.p.x
#define MF r.p.m
#define VF r.p.v
#define NF r.p.n
#define EF r.e
#define AW r.a.w
#define AH r.a.h
#define AL r.a.l
#define XW r.x.w
#define XH r.x.h
#define XL r.x.l
#define YW r.y.w
#define YH r.y.h
#define YL r.y.l
#define ZW r.x.w
#define ZH r.z.h
#define ZL r.z.l
#define SH r.s.h
#define SL r.s.l
#define DH r.d.h
#define DL r.d.l
#define PCB r.pc.b
#define PCW r.pc.w
#define PCH r.pc.h
#define PCL r.pc.l

#define E if(r.e)
#define N if(!r.e)
#define L lastCycle();
#define call(op) (this->*op)()

#define LO(n) n.byte(0)
#define HI(n) n.byte(1)

#include "memory.cpp"
#include "algorithms.cpp"

#include "instructions-read.cpp"
#include "instructions-write.cpp"
#include "instructions-modify.cpp"
#include "instructions-pc.cpp"
#include "instructions-misc.cpp"
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

  r.mdr    = 0x00;
  r.wai    = false;
  r.stp    = false;
  r.vector = 0xfffc;  //reset vector address
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
#undef B
#undef P
#undef PC
#undef CF
#undef ZF
#undef IF
#undef DF
#undef XF
#undef MF
#undef VF
#undef NF
#undef EF
#undef AW
#undef AH
#undef AL
#undef XW
#undef XH
#undef XL
#undef YW
#undef YH
#undef YL
#undef ZW
#undef ZH
#undef ZL
#undef SH
#undef SL
#undef DH
#undef DL
#undef PCB
#undef PCW
#undef PCH
#undef PCL

#undef E
#undef N
#undef L
#undef call

#undef LO
#undef HI

//#include "disassembler.cpp"
#include "serialization.cpp"

}
