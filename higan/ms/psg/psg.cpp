#include <ms/ms.hpp>

namespace MasterSystem {

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

  int left = 0;
  left += levels[tone0.volume] * tone0.output * tone0.left;
  left += levels[tone1.volume] * tone1.output * tone1.left;
  left += levels[tone2.volume] * tone2.output * tone2.left;
  left += levels[noise.volume] * noise.output * noise.left;

  int right = 0;
  right += levels[tone0.volume] * tone0.output * tone0.right;
  right += levels[tone1.volume] * tone1.output * tone1.right;
  right += levels[tone2.volume] * tone2.output * tone2.right;
  right += levels[noise.volume] * noise.output * noise.right;

  stream->sample(sclamp<16>(left) / 32768.0, sclamp<16>(right) / 32768.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  //Master System is monaural; Game Gear is stereo
  //use stereo mode for both; output same sample to both channels for Master System
  create(PSG::Enter, system.colorburst() / 16.0);
  stream = Emulator::audio.createStream(2, frequency());
  stream->addHighPassFilter(20.0, Emulator::Filter::Order::First);
  stream->addDCRemovalFilter();

  select = 0;
  for(auto n : range(15)) {
    levels[n] = 0x2000 * pow(2, n * -2.0 / 6.0) + 0.5;
  }
  levels[15] = 0;

  tone0.power();
  tone1.power();
  tone2.power();
  noise.power();
}

}
