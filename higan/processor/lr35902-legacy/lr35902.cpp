#include <processor/processor.hpp>
#include "lr35902.hpp"

namespace Processor {

#include "instructions.cpp"
#include "instruction.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

auto LR35902::power() -> void {
  r[AF] = 0x0000;
  r[BC] = 0x0000;
  r[DE] = 0x0000;
  r[HL] = 0x0000;
  r[SP] = 0x0000;
  r[PC] = 0x0000;

  r.halt = false;
  r.stop = false;
  r.ei = false;
  r.ime = false;
}

}
