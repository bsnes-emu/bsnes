#include <ms/ms.hpp>

namespace MasterSystem {

VDP vdp;
#include "io.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  if(io.vcounter <= vlines()) {
    if(io.lcounter-- == 0) {
      io.lcounter = io.lineCounter;
      io.intLine = 1;
    }
  }

  if(io.vcounter == vlines() + 1) {
    io.lcounter = io.lineCounter;
    io.intFrame = 1;
  }

  for(uint x : range(684)) {
    step(1);
  }

  if(io.vcounter == 240) scheduler.exit(Scheduler::Event::Frame);
}

auto VDP::step(uint clocks) -> void {
  while(clocks--) {
    if(++io.hcounter == 684) {
      io.hcounter = 0;
      if(++io.vcounter == 262) {
        io.vcounter = 0;
      }
    }

    cpu.setINT((io.lineInterrupts && io.intLine) || (io.frameInterrupts && io.intFrame));
    Thread::step(1);
    synchronize(cpu);
  }
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 240);
}

auto VDP::vlines() -> uint {
  if(io.lines240) return 240;
  if(io.lines224) return 224;
  return 192;
}

auto VDP::vblank() -> bool {
  return io.vcounter >= vlines();
}

auto VDP::power() -> void {
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 5.0);

  memory::fill(&io, sizeof(IO));
}

}
