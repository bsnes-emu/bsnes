#include <md/md.hpp>

//256-width = colorburst * 15 / 10
//320-width = colorburst * 15 /  8

namespace MegaDrive {

VDP vdp;
#include "io.cpp"
#include "dma.cpp"
#include "render.cpp"
#include "background.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  scanline();
  if(state.y < 240) {
    for(uint x : range(320)) {
      run();
      step(1);
    }
  } else {
    step(342);
  }
  step(22);
}

auto VDP::step(uint clocks) -> void {
  while(clocks--) {
    dmaRun();
    Thread::step(1);
    synchronize(cpu);
  }
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 1280 * sizeof(uint32), 1280, 480);
}

auto VDP::power() -> void {
  planeA.power();
  window.power();
  planeB.power();
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 10.0);

  memory::fill(&io, sizeof(IO));

  planeA.reset();
  window.reset();
  planeB.reset();
}

}
