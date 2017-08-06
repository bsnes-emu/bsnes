#include <processor/processor.hpp>
#include "arm7tdmi.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions-arm.cpp"
#include "instructions-thumb.cpp"
#include "serialization.cpp"

auto ARM7TDMI::power() -> void {
  processor = {};
  interrupt(PSR::SVC, 0x00);
  processor.r15.modify = [&] { pipeline.reload = true; };
  pipeline = {};
  carry = 0;
  irq = 0;
}

}
