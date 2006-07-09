#include "../../base.h"

#include "core/core.cpp"
#include "memory/memory.cpp"
#include "timing/timing.cpp"

void sapu_entry_point() {
  r_apu->main();
}

void sAPU::run() {
  co_call(thread);
}

void sAPU::power() {
  memset(spcram, 0x00, 65536);
  reset();
}

void sAPU::reset() {
  if(thread)co_delete(thread);
  thread = co_create(sapu_entry_point, 65536);

  regs.pc = 0xffc0;
  regs.a  = 0x00;
  regs.x  = 0x00;
  regs.y  = 0x00;
  regs.sp = 0xef;
  regs.p  = 0x02;

  status.clocks_executed = 0;

//$f1
  status.iplrom_enabled = true;

//$f2
  status.dsp_addr = 0x00;

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

sAPU::sAPU() {
//#include "core/optable.cpp"

  thread = 0;

  t0.cycle_frequency = 128; //1.024mhz /  8khz = 128
  t1.cycle_frequency = 128; //1.024mhz /  8khz = 128
  t2.cycle_frequency =  16; //1.024mhz / 64khz =  16

//targets not initialized/changed upon reset
  t0.target = 0;
  t1.target = 0;
  t2.target = 0;
}

sAPU::~sAPU() {
  if(thread)co_delete(thread);
}
