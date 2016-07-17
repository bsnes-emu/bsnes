#include <processor/processor.hpp>
#include "m68k.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
#include "ea.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"

auto M68K::power() -> void {
}

auto M68K::reset() -> void {
  instructionsExecuted = 0;

  for(uint n : range(8)) r.d(n) = 0;
  for(uint n : range(7)) r.a(n) = 0;
  r.ssp = 0;
  r.usp = 0;
  r.pc = 0;
  r.sr = 0x2000;
}

auto M68K::sign(uint2 size, uint32 data) -> int32 {
  if(size == Byte) return  (int8)data;
  if(size == Word) return (int16)data;
  if(size == Long) return (int32)data;
  return 0;
}

}
