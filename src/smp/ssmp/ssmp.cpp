#include <../base.hpp>

#define SSMP_CPP
namespace SNES {

#include "memory/memory.cpp"
#include "timing/timing.cpp"

void sSMP::enter() {
  while(true) {
    tracer.trace_smpop();  //traces SMP opcode (only if tracer is enabled)

    (this->*opcode_table[op_readpc()])();

    //forcefully sync S-CPU and S-SMP, in case chips are not communicating
    static unsigned counter = 0;
    if(++counter >= 128) {
      counter = 0;
      scheduler.sync_smpcpu();
    }
  }
}

void sSMP::power() {
  //targets not initialized/changed upon reset
  t0.target = 0;
  t1.target = 0;
  t2.target = 0;

  reset();
}

void sSMP::reset() {
  regs.pc = 0xffc0;
  regs.a  = 0x00;
  regs.x  = 0x00;
  regs.y  = 0x00;
  regs.sp = 0xef;
  regs.p  = 0x02;

  for(unsigned i = 0; i < memory::apuram.size(); i++) {
    memory::apuram.write(i, 0x00);
  }

  status.clock_counter = 0;
  status.dsp_counter   = 0;

  //$00f0
  status.clock_speed   = 24 * 3 / 3;
  status.mmio_disabled = false;
  status.ram_writable  = true;

  //$00f1
  status.iplrom_enabled = true;

  //$00f2
  status.dsp_addr = 0x00;

  //$00f8,$00f9
  status.smp_f8 = 0x00;
  status.smp_f9 = 0x00;

  t0.enabled = false;
  t1.enabled = false;
  t2.enabled = false;

  t0.stage1_ticks = 0;
  t1.stage1_ticks = 0;
  t2.stage1_ticks = 0;

  t0.stage2_ticks = 0;
  t1.stage2_ticks = 0;
  t2.stage2_ticks = 0;

  t0.stage3_ticks = 0;
  t1.stage3_ticks = 0;
  t2.stage3_ticks = 0;
}

sSMP::sSMP() {
  initialize_opcode_table();
}

sSMP::~sSMP() {
}
};

