#include <md/md.hpp>

//256-width = colorburst * 15 / 10
//320-width = colorburst * 15 /  8

namespace MegaDrive {

VDP vdp;
#include "io.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  for(uint y : range(262)) {
    auto outputLo = buffer + (y * 2 + 0) * 1280;
    auto outputHi = buffer + (y * 2 + 1) * 1280;
    for(uint x : range(342)) {
      if(y < 240 && x < 320) {
        for(uint n : range(4)) {
          *outputLo++ = 511;
          *outputHi++ = 511;
        }
      }

      step(1);
    }
    if(y == 240) scheduler.exit(Scheduler::Event::Frame);
  }
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 1280 * sizeof(uint32), 1280, 480);
}

auto VDP::power() -> void {
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 10.0);

  memory::fill(&io, sizeof(IO));
}

}
