#include <md/md.hpp>

namespace MegaDrive {

YM2612 ym2612;
#include "io.cpp"
#include "timer.cpp"

auto YM2612::Enter() -> void {
  while(true) scheduler.synchronize(), ym2612.main();
}

auto YM2612::main() -> void {
  timerA.run();
  timerB.run();

  int output = 0;
  if(dac.enable) output += dac.sample;
  output <<= 5;

  stream->sample(output / 32768.0, output / 32768.0);
  step(1);
}

auto YM2612::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto YM2612::power() -> void {
  create(YM2612::Enter, system.colorburst() * 15.0 / 7.0 / 144.0);
  stream = Emulator::audio.createStream(2, frequency());

  memory::fill(&io, sizeof(IO));

  timerA.power();
  timerB.power();

  dac.enable = 0;
  dac.sample = 0;
}

}
