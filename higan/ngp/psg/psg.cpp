#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

PSG psg;
#include "serialization.cpp"

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  stream->sample(0.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  create(PSG::Enter, system.frequency() / 2.0);
  stream = Emulator::audio.createStream(1, frequency());
  stream->addHighPassFilter(20.0, Emulator::Filter::Order::First);
  stream->addDCRemovalFilter();
}

}
