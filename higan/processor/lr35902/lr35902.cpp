#include <processor/processor.hpp>
#include "lr35902.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

auto LR35902::power() -> void {
  r = {};
}

}
