#include "../base.h"
#include "../timing/timing.h"
#include "../bridge/bridge.h"
#include "spc700_iplrom.h"
#include "spc700.h"

extern snes_timer  *snes_time;
extern debugstate   debugger;
extern port_bridge *cpu_apu_bridge;
sony_spc700        *spc700;

#include "spc700_ops.cpp"

void sony_spc700::Reset(void) {
  memset(ram, 0, 65536);
  memcpy(ram + 0xffc0, spc700_iplrom, 64);
  regs.pc = 0xffc0;
  regs.a = regs.x = regs.y = regs.sp = 0x00;
  regs.p = 0x02;
  regs.dp = 0x0000;
}

void sony_spc700::exec_op(void) {
byte op;
  op = spc700->ram[spc700->regs.pc];
  spc700_optbl[op]();
  debugger.apu_op_executed = true;
  debugger.disas_apu_op    = true;

  debug_test_bp(BPSRC_SPCRAM, BP_EXEC, spc700->regs.pc, 0);
}

void sony_spc700::Run(void) {
  if(snes_time->bridge.cpu_cycles >= snes_time->bridge.apu_cycles) {
    exec_op();
  }
  if(snes_time->bridge.cpu_cycles >= 65536 && snes_time->bridge.apu_cycles >= 65536) {
    snes_time->bridge.cpu_cycles &= 65535;
    snes_time->bridge.apu_cycles &= 65535;
  }
}

sony_spc700::sony_spc700() {
  ram = (byte*)malloc(65536);
}

sony_spc700::~sony_spc700() {
  if(ram)free(ram);
}
