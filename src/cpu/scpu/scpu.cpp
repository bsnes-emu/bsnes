#include "../../base.h"

#include "core/core.cpp"
#include "dma/dma.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"

void scpu_entry_point() {
  r_cpu->main();
}

void sCPU::run() {
  co_call(thread);
}

void sCPU::power() {
  status.region = bool(snes->region());

  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;
  reset();
}

void sCPU::reset() {
  if(thread)co_delete(thread);
  thread = co_create(scpu_entry_point, 65536);

  regs.pc.d = 0x000000;
  regs.pc.l = r_mem->read(0xfffc);
  regs.pc.h = r_mem->read(0xfffd);

//some registers are not fully reset by SNES
  regs.x.h  = 0x00;
  regs.y.h  = 0x00;
  regs.s.h  = 0x01;
  regs.d    = 0x0000;
  regs.db   = 0x00;
  regs.p    = 0x34;
  regs.e    = 1;
  regs.mdr  = 0x00;

  event.wai = false;
  event.irq = false;
  event.irq_vector = 0xfffc; //reset vector address

  mmio_reset();
  dma_reset();
  timing_reset();

  apu_port[0] = 0x00;
  apu_port[1] = 0x00;
  apu_port[2] = 0x00;
  apu_port[3] = 0x00;
}

sCPU::sCPU() {
//#include "core/optable.cpp"
  thread = 0;
}

sCPU::~sCPU() {
  if(thread)co_delete(thread);
}
