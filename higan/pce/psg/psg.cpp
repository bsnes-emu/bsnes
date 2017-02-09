#include <pce/pce.hpp>

namespace PCEngine {

PSG psg;
#include "io.cpp"
#include "channel.cpp"

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  uint left = 0, right = 0;

  for(auto C : range(6)) {
    channel[C].run();
    if(C == 1 && io.lfoEnable) {
      //todo: frequency modulation of channel 0 using channel 1's output
    } else {
      left += channel[C].output.left;
      right += channel[C].output.right;
    }
  }

  stream->sample(left / 32768.0, right / 32768.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  create(PSG::Enter, system.colorburst());
  stream = Emulator::audio.createStream(2, system.colorburst());

  memory::fill(&io, sizeof(IO));
  for(auto C : range(6)) channel[C].power();
}

}
