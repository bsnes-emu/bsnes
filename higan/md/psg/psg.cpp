#include <md/md.hpp>

namespace MegaDrive {

PSG psg;
#include "io.cpp"
#include "tone.cpp"
#include "noise.cpp"

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  tone0.run();
  tone1.run();
  tone2.run();
  noise.run();

  int output = 0;
  if(tone0.output) output += levels[tone0.volume];
  if(tone1.output) output += levels[tone1.volume];
  if(tone2.output) output += levels[tone2.volume];
  if(noise.output) output += levels[noise.volume];

  lowpass += (output - lowpass) * 20.0 / 256.0;
  output = output * 2.0 / 6.0 + lowpass * 3.0 / 4.0;
  output = sclamp<16>(output - 32768);

  stream->sample(output / 32768.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  create(PSG::Enter, system.colorburst() / 16.0);
  stream = Emulator::audio.createStream(1, frequency());

  select = 0;
  lowpass = 0;
  for(auto n : range(15)) {
    levels[n] = 0x3fff * pow(2, n * -2.0 / 6.0) + 0.5;
  }
  levels[15] = 0;

  tone0.power();
  tone1.power();
  tone2.power();
  noise.power();
}

}
