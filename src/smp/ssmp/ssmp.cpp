#include "../../base.h"

#include "core/core.cpp"
#include "memory/memory.cpp"
#include "timing/timing.cpp"

void ssmp_entry_point() {
  r_smp->main();
}

void sSMP::run() {
  co_call(thread);
}

void sSMP::power() {
//for(int i = 0; i < 65536; i += 64) {
//  memset(spcram + i,      0x00, 32);
//  memset(spcram + i + 32, 0xff, 32);
//}

  memset(spcram, 0x00, 65536);

//targets not initialized/changed upon reset
  t0.target = 0;
  t1.target = 0;
  t2.target = 0;

  reset();
}

void sSMP::reset() {
  if(thread)co_delete(thread);
  thread = co_create(ssmp_entry_point, 65536);

  regs.pc = 0xffc0;
  regs.a  = 0x00;
  regs.x  = 0x00;
  regs.y  = 0x00;
  regs.sp = 0xef;
  regs.p  = 0x02;

  status.clocks_executed = 0;

//$00f0
  status.clock_speed   = 24 * 3 / 3;
  status.mmio_disabled = false;
  status.ram_writable  = true;

//$00f1
  status.iplrom_enabled = true;

//$00f2
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

sSMP::sSMP() {
  thread = 0;
}

sSMP::~sSMP() {
  if(thread)co_delete(thread);
}
