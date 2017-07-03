#include <sfc/sfc.hpp>

namespace SuperFamicom {

SMP smp;

#include "memory.cpp"
#include "timing.cpp"
#include "serialization.cpp"

auto SMP::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

auto SMP::Enter() -> void {
  while(true) scheduler.synchronize(), smp.main();
}

auto SMP::main() -> void {
  if(r.wait) return instructionWait();
  if(r.stop) return instructionStop();
  instruction();
}

auto SMP::load(Markup::Node node) -> bool {
  if(auto name = node["smp/rom/name"].text()) {
    if(auto fp = platform->open(ID::System, name, File::Read, File::Required)) {
      fp->read(iplrom, 64);
      return true;
    }
  }
  return false;
}

auto SMP::power() -> void {
  SPC700::power();
  create(Enter, system.apuFrequency() / 12.0);

  r.pc.byte.l = iplrom[62];
  r.pc.byte.h = iplrom[63];

  //timing
  io.clockCounter = 0;
  io.dspCounter = 0;

  //external
  io.apu0 = 0x00;
  io.apu1 = 0x00;
  io.apu2 = 0x00;
  io.apu3 = 0x00;

  //$00f0
  io.timersDisable = false;
  io.ramWritable = true;
  io.ramDisable = false;
  io.timersEnable = true;
  io.externalWaitStates = 0;
  io.internalWaitStates = 0;

  //$00f1
  io.iplromEnable = true;

  //$00f2
  io.dspAddr = 0x00;

  //$00f4-00f7
  io.cpu0 = 0x00;
  io.cpu1 = 0x00;
  io.cpu2 = 0x00;
  io.cpu3 = 0x00;

  //$00f8-$00f9
  io.aux4 = 0x00;
  io.aux5 = 0x00;

  timer0.stage0 = 0;
  timer1.stage0 = 0;
  timer2.stage0 = 0;

  timer0.stage1 = 0;
  timer1.stage1 = 0;
  timer2.stage1 = 0;

  timer0.stage2 = 0;
  timer1.stage2 = 0;
  timer2.stage2 = 0;

  timer0.stage3 = 0;
  timer1.stage3 = 0;
  timer2.stage3 = 0;

  timer0.line = 0;
  timer1.line = 0;
  timer2.line = 0;

  timer0.enable = false;
  timer1.enable = false;
  timer2.enable = false;

  timer0.target = 0;
  timer1.target = 0;
  timer2.target = 0;
}

}
