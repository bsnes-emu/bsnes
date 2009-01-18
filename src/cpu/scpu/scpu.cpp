#include <../base.hpp>
#define SCPU_CPP

#include <nall/priorityqueue.hpp>
priority_queue<unsigned> event(512, bind(&sCPU::queue_event, &cpu));

#include "core/core.cpp"
#include "dma/dma.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"

void sCPU::power() {
  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;

  mmio_power();
  dma_power();
  timing_power();

  reset();
}

void sCPU::reset() {
  regs.pc.d = 0x000000;
  regs.pc.l = bus.read(0xfffc);
  regs.pc.h = bus.read(0xfffd);

  //note: some registers are not fully reset by SNES
  regs.x.h  = 0x00;
  regs.y.h  = 0x00;
  regs.s.h  = 0x01;
  regs.d    = 0x0000;
  regs.db   = 0x00;
  regs.p    = 0x34;
  regs.e    = 1;
  regs.mdr  = 0x00;

  status.wai_lock = false;
  status.interrupt_pending = false;
  status.interrupt_vector  = 0xfffc;  //reset vector address

  mmio_reset();
  dma_reset();
  timing_reset();

  apu_port[0] = 0x00;
  apu_port[1] = 0x00;
  apu_port[2] = 0x00;
  apu_port[3] = 0x00;
}

sCPU::sCPU() {
}

sCPU::~sCPU() {
}
