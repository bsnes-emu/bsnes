#include <processor/processor.hpp>
#include "lr35902.hpp"

namespace Processor {

#include "instructions.cpp"
#include "instruction.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

auto LR35902::power() -> void {
  r.halt = false;
  r.stop = false;
  r.ei = false;
  r.ime = false;
}

}
