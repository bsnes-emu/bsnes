#include <processor/processor.hpp>
#include "tlcs900h.hpp"

namespace Processor {

//todo: these defines should not be necessary; yet ADL doesn't work without them ... why not?
#define Byte uint8
#define Word uint16
#define Long uint32

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
