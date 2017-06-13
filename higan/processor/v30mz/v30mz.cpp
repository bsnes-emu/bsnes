#include <processor/processor.hpp>
#include "v30mz.hpp"

namespace Processor {

#include "registers.cpp"
#include "modrm.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instructions-adjust.cpp"
#include "instructions-alu.cpp"
#include "instructions-exec.cpp"
#include "instructions-flag.cpp"
#include "instructions-group.cpp"
#include "instructions-misc.cpp"
#include "instructions-move.cpp"
#include "instructions-string.cpp"
#include "instruction.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

auto V30MZ::warning(string text) -> void {
//print(text, "\n");
}

auto V30MZ::power() -> void {
  state.halt = false;
  state.poll = true;
  state.prefix = false;
  prefixes.reset();

  r.ax = 0x0000;
  r.cx = 0x0000;
  r.dx = 0x0000;
  r.bx = 0x0000;
  r.sp = 0x2000;
  r.bp = 0x0000;
  r.si = 0x0000;
  r.di = 0x0000;
  r.es = 0x0000;
  r.cs = 0xffff;
  r.ss = 0x0000;
  r.ds = 0x0000;
  r.ip = 0x0000;
  r.f  = 0x8000;
}

auto V30MZ::exec() -> void {
  state.poll = true;
  state.prefix = false;
  if(state.halt) return wait(1);

  instruction();
  if(!state.prefix) prefixes.reset();
}

}
