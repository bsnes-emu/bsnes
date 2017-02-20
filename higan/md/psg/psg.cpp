#include <md/md.hpp>

namespace MegaDrive {

PSG psg;
#include "io.cpp"

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
  create(PSG::Enter, system.colorburst() / 16.0);
  stream = Emulator::audio.createStream(1, system.colorburst() / 16.0);
}

}
