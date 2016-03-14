#include <ws/ws.hpp>

namespace WonderSwan {

APU apu;
#include "io.cpp"
#include "channel.cpp"
#include "channel0.cpp"
#include "channel1.cpp"
#include "channel2.cpp"
#include "channel3.cpp"
#include "channel4.cpp"

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  channel0.run();
  channel1.run();
  channel2.run();
  channel3.run();

  if(s.clock.bits(0,12) == 0) {
    channel2.sweep();
  }

  if(s.clock.bits(0,6) == 0) {
    channel4.run();
    dacRun();
  }

  s.clock++;
  step(1);
}

auto APU::dacRun() -> void {
  int left = 0;
  if(channel0.r.enable) left += channel0.o.left;
  if(channel1.r.enable) left += channel1.o.left;
  if(channel2.r.enable) left += channel2.o.left;
  if(channel3.r.enable) left += channel3.o.left;
  left = (left >> r.speakerShift) << 5;
  if(channel4.r.enable) left += channel4.o.left;
  left = sclamp<16>(left << 3);

  int right = 0;
  if(channel0.r.enable) right += channel0.o.right;
  if(channel1.r.enable) right += channel1.o.right;
  if(channel2.r.enable) right += channel2.o.right;
  if(channel3.r.enable) right += channel3.o.right;
  right = (right >> r.speakerShift) << 5;
  if(channel4.r.enable) right += channel4.o.right;
  right = sclamp<16>(right << 3);

  if(!r.speakerEnable) {
    left = 0;
    right = 0;
  }

  interface->audioSample(left, right);
}

auto APU::step(uint clocks) -> void {
  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto APU::power() -> void {
  create(APU::Enter, 3'072'000);

  for(uint n = 0x006a; n <= 0x006b; n++) iomap[n] = this;
  for(uint n = 0x0080; n <= 0x0094; n++) iomap[n] = this;

  s.clock = 0;
  r.waveBase = 0;
  r.speakerEnable = 0;
  r.speakerShift = 0;
  r.headphoneEnable = 0;

  channel0.o.left = 0;
  channel0.o.right = 0;
  channel0.s.period = 0;
  channel0.s.sampleOffset = 0;
  channel0.r.pitch = 0;
  channel0.r.volumeLeft = 0;
  channel0.r.volumeRight = 0;
  channel0.r.enable = 0;

  channel1.o.left = 0;
  channel1.o.right = 0;
  channel1.s.period = 0;
  channel1.s.sampleOffset = 0;
  channel1.r.pitch = 0;
  channel1.r.volumeLeft = 0;
  channel1.r.volumeRight = 0;
  channel1.r.enable = 0;
  channel1.r.voice = 0;
  channel1.r.voiceEnableLeft = 0;
  channel1.r.voiceEnableRight = 0;

  channel2.o.left = 0;
  channel2.o.right = 0;
  channel2.s.period = 0;
  channel2.s.sampleOffset = 0;
  channel2.s.sweepCounter = 0;
  channel2.r.pitch = 0;
  channel2.r.volumeLeft = 0;
  channel2.r.volumeRight = 0;
  channel2.r.sweepValue = 0;
  channel2.r.sweepTime = 0;
  channel2.r.enable = 0;
  channel2.r.sweep = 0;

  channel3.o.left = 0;
  channel3.o.right = 0;
  channel3.s.period = 0;
  channel3.s.sampleOffset = 0;
  channel3.s.noiseOutput = 0;
  channel3.s.noiseLFSR = 0;
  channel3.r.pitch = 0;
  channel3.r.volumeLeft = 0;
  channel3.r.volumeRight = 0;
  channel3.r.noiseMode = 0;
  channel3.r.noiseReset = 0;
  channel3.r.noiseUpdate = 0;
  channel3.r.enable = 0;
  channel3.r.noise = 0;

  channel4.o.left = 0;
  channel4.o.right = 0;
  channel4.s.data = 0;
  channel4.r.volume = 0;
  channel4.r.scale = 0;
  channel4.r.speed = 0;
  channel4.r.enable = 0;
  channel4.r.unknown = 0;
  channel4.r.leftEnable = 0;
  channel4.r.rightEnable = 0;
}

}
