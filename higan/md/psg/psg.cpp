#include <md/md.hpp>

namespace MegaDrive {

PSG psg;
#include "io.cpp"
#include "tone.cpp"
#include "noise.cpp"
#include "serialization.cpp"

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  tone0.run();
  tone1.run();
  tone2.run();
  noise.run();

  int output = 0;
  output += levels[tone0.volume] * tone0.output;
  output += levels[tone1.volume] * tone1.output;
  output += levels[tone2.volume] * tone2.output;
  output += levels[noise.volume] * noise.output;

  stream->sample(sclamp<16>(output) / 32768.0);
  step(16);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto PSG::power(bool reset) -> void {
  create(PSG::Enter, system.frequency() / 15.0);
  stream = Emulator::audio.createStream(1, frequency() / 16.0);
  stream->addFilter(Emulator::Filter::Order::First, Emulator::Filter::Type::HighPass, 20.0);
  stream->addFilter(Emulator::Filter::Order::First, Emulator::Filter::Type::LowPass, 2840.0);

  select = 0;
  for(auto n : range(15)) {
    levels[n] = 0x1400 * pow(2, n * -2.0 / 6.0) + 0.5;
  }
  levels[15] = 0;

  tone0.power();
  tone1.power();
  tone2.power();
  noise.power();
}

}
