#include <processor/processor.hpp>
#include "tlcs900h.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
#include "conditions.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "serialization.cpp"

TLCS900H tlcs900h;

auto TLCS900H::power() -> void {
  r = {};
}

TLCS900H::TLCS900H() {
}

}
