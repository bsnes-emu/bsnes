#include <processor/processor.hpp>
#include "tlcs900h.hpp"

namespace Processor {

using Byte =  uint8;
using Word = uint16;
using Long = uint32;

template<typename Size> static constexpr auto isByte() -> bool { return is_same<Size, Byte>::value; }
template<typename Size> static constexpr auto isWord() -> bool { return is_same<Size, Word>::value; }
template<typename Size> static constexpr auto isLong() -> bool { return is_same<Size, Long>::value; }

#include "registers.cpp"
#include "memory.cpp"
#include "conditions.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "serialization.cpp"

auto TLCS900H::power() -> void {
  r = {};
}

}
