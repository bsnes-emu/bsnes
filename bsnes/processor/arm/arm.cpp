#include <processor/processor.hpp>
#include "arm.hpp"

namespace Processor {

#include "registers.cpp"
#include "instructions-arm.cpp"
#include "instructions-thumb.cpp"
#include "disassembler.cpp"

void ARM::power() {
  processor.power();
  pipeline.reload = true;
  exception = false;
  r(15).modify = [&] {
    pipeline.reload = true;
    r(15).data &= cpsr().t ? ~1 : ~3;
  };
}

void ARM::serialize(serializer &s) {
}

}
