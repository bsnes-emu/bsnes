#include <ws/ws.hpp>

namespace WonderSwan {

APU apu;
#include "io.cpp"

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  step(128);
  interface->audioSample(0, 0);
}

auto APU::step(uint clocks) -> void {
  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto APU::power() -> void {
  create(APU::Enter, 3'072'000);

  for(uint n = 0x0080; n <= 0x0094; n++) iomap[n] = this;

  channel1.r.pitch = 0;
  channel1.r.volumeLeft = 0;
  channel1.r.volumeRight = 0;
  channel1.r.enable = 0;

  channel2.r.pitch = 0;
  channel2.r.volumeLeft = 0;
  channel2.r.volumeRight = 0;
  channel2.r.enable = 0;
  channel2.r.voice = 0;

  channel3.r.pitch = 0;
  channel3.r.volumeLeft = 0;
  channel3.r.volumeRight = 0;
  channel3.r.sweepValue = 0;
  channel3.r.sweepTime = 0;
  channel3.r.enable = 0;
  channel3.r.sweep = 0;

  channel4.r.pitch = 0;
  channel4.r.volumeLeft = 0;
  channel4.r.volumeRight = 0;
  channel4.r.noiseMode = 0;
  channel4.r.enable = 0;
  channel4.r.noise = 0;

  r.waveBase = 0;
  r.speakerEnable = 0;
  r.speakerShift = 0;
  r.headphoneEnable = 0;
  r.voiceEnableLeft = 0;
  r.voiceEnableRight = 0;
}

}
