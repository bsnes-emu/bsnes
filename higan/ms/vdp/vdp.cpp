#include <ms/ms.hpp>

namespace MasterSystem {

VDP vdp;
#include "io.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  for(uint y : range(262)) {
    for(uint x : range(684)) {
      step(1);
    }
    if(y == 240) scheduler.exit(Scheduler::Event::Frame);
  }
}

auto VDP::step(uint clocks) -> void {
  if(++io.hcounter == 684) {
    io.hcounter = 0;
    if(++io.vcounter == 262) {
      io.vcounter = 0;
    }
  }

  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 240);
}

auto VDP::power() -> void {
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 5.0);

  memory::fill(&io, sizeof(IO));
}

}
