#include "../../base.h"

#include "core/op0x.cpp"

#include "memory/read.cpp"
#include "memory/write.cpp"

void SuperFX::init() {
}

void SuperFX::enable() {
  for(uint i = 0x3000; i <= 0x32ff; i++) {
    r_mem->set_mmio_mapper(i, this);
  }
}

void SuperFX::power() {
  reset();
}

void SuperFX::reset() {
  regs.r0  = 0;
  regs.r1  = 0;
  regs.r2  = 0;
  regs.r3  = 0;
  regs.r4  = 0;
  regs.r5  = 0;
  regs.r6  = 0;
  regs.r7  = 0;
  regs.r8  = 0;
  regs.r9  = 0;
  regs.r10 = 0;
  regs.r11 = 0;
  regs.r12 = 0;
  regs.r13 = 0;
  regs.r14 = 0;
  regs.r15 = 0;

  regs.sfr = 0;

  memset(cache, 0, sizeof cache);
}
