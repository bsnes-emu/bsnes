#include <processor/processor.hpp>
#include "tlcs900h.hpp"

namespace Processor {

#define Byte uint8
#define Word uint16
#define Long uint32

#include "registers.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "serialization.cpp"

auto TLCS900H::power() -> void {
  r = {};
}

}
