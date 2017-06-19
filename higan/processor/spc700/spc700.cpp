#include <processor/processor.hpp>
#include "spc700.hpp"

namespace Processor {

#define PC r.pc.w
#define YA r.ya.w
#define A r.ya.byte.l
#define X r.x
#define Y r.ya.byte.h
#define S r.s
#define P r.p

#define CF r.p.c
#define ZF r.p.z
#define IF r.p.i
#define HF r.p.h
#define BF r.p.b
#define PF r.p.p
#define VF r.p.v
#define NF r.p.n

#define alu (this->*op)

#include "memory.cpp"
#include "algorithms.cpp"
#include "instructions.cpp"
#include "instruction.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

#undef PC
#undef YA
#undef A
#undef X
#undef Y
#undef S
#undef P

#undef CF
#undef ZF
#undef IF
#undef HF
#undef BF
#undef PF
#undef VF
#undef NF

#undef alu

auto SPC700::power() -> void {
  r.pc.w = 0x0000;
  r.ya.w = 0x0000;
  r.x = 0x00;
  r.s = 0xef;
  r.p = 0x02;

  r.wai = false;
  r.stp = false;
}

}
