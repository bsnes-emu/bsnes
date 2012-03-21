#include "registers.cpp"
#include "arm.cpp"
#include "thumb.cpp"
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
