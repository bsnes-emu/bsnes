#include <processor/processor.hpp>
#include "m68k.hpp"

namespace Processor {

#include "memory.cpp"

auto M68K::instruction() -> void {
  instructionsExecuted++;

  auto opcode = readWord(r.pc);
  r.pc += 2;

  //nop
  if(opcode == 0x4e71) {
    step(5);
    return;
  }

  //bra disp
  if((opcode & 0xff00) == 0x6000) {
    int displacement = (int8)opcode;
    if(!displacement) displacement = (int16)readWord(r.pc);
    r.pc += displacement;
    step(12);
    return;
  }

  instructionsExecuted--;
  r.pc -= 2;
  print("[M68K] unimplemented instruction: ", hex(r.pc, 6L), " = ", hex(opcode, 4L), "\n");
  print("[M68K] executed ", instructionsExecuted, " instructions\n");
  while(true) step(5);
}

auto M68K::power() -> void {
}

auto M68K::reset() -> void {
  instructionsExecuted = 0;

  for(auto& n : r.d) n = 0;
  for(auto& n : r.a) n = 0;
  r.pc = 0x000200;
  r.ccr.value = 0;
}

}
