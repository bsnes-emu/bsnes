#include <../base.hpp>
#define SCPU_CPP

#include <nall/priorityqueue.hpp>
priority_queue<unsigned> event(512, bind(&sCPU::queue_event, &cpu));

#include "dma/dma.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"

void sCPU::enter() {
  regs.pc.l = bus.read(0xfffc);
  regs.pc.h = bus.read(0xfffd);

  //initial latch values for $213c/$213d
  //[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
  //[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  add_clocks(186);

  while(true) {
    if(status.interrupt_pending) {
      status.interrupt_pending = false;
      if(status.nmi_pending) {
        status.nmi_pending = false;
        status.interrupt_vector = (regs.e == false ? 0xffea : 0xfffa);
      } else if(status.irq_pending) {
        status.irq_pending = false;
        status.interrupt_vector = (regs.e == false ? 0xffee : 0xfffe);
      }
      op_irq();
    }

    tracer.trace_cpuop();  //traces CPU opcode (only if tracer is enabled)
    (this->*opcode_table[op_readpc()])();
  }
}

void sCPU::op_irq() {
  op_read(regs.pc.d);
  op_io();
  if(!regs.e) op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.e ? (regs.p & ~0x10) : regs.p);
  rd.l = op_read(status.interrupt_vector + 0);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  rd.h = op_read(status.interrupt_vector + 1);
  regs.pc.w = rd.w;
}

void sCPU::power() {
  CPU::power();

  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;

  mmio_power();
  dma_power();
  timing_power();

  reset();
}

void sCPU::reset() {
  CPU::reset();

  //note: some registers are not fully reset by SNES
  regs.pc   = 0x000000;
  regs.x.h  = 0x00;
  regs.y.h  = 0x00;
  regs.s.h  = 0x01;
  regs.d    = 0x0000;
  regs.db   = 0x00;
  regs.p    = 0x34;
  regs.e    = 1;
  regs.mdr  = 0x00;
  regs.wai  = false;
  update_table();

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
