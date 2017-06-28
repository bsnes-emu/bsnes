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
  create(Enter, 32040.0 * 768.0);

  r.pc.byte.l = iplrom[62];
  r.pc.byte.h = iplrom[63];

  for(auto& byte : apuram) byte = random(0x00);
  apuram[0x00f4] = 0x00;
  apuram[0x00f5] = 0x00;
  apuram[0x00f6] = 0x00;
  apuram[0x00f7] = 0x00;

  io.clockCounter = 0;
  io.dspCounter = 0;
  io.timerStep = 3;

  //$00f0
  io.clockSpeed = 0;
  io.timerSpeed = 0;
  io.timersEnable = true;
  io.ramDisable = false;
  io.ramWritable = true;
  io.timersDisable = false;

  //$00f1
  io.iplromEnable = true;

  //$00f2
  io.dspAddr = 0x00;

  //$00f8,$00f9
  io.ram00f8 = 0x00;
  io.ram00f9 = 0x00;

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
