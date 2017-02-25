#include <md/md.hpp>

namespace MegaDrive {

VDP vdp;
#include "memory.cpp"
#include "io.cpp"
#include "dma.cpp"
#include "render.cpp"
#include "background.cpp"
#include "sprite.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  scanline();
  if(state.y < screenHeight()) {
    if(state.y == 0) {
      cpu.lower(CPU::Interrupt::VerticalBlank);
    }
    cpu.lower(CPU::Interrupt::HorizontalBlank);
    for(uint x : range(320)) {
      run();
      step(4);
    }
    if(io.horizontalBlankInterruptEnable) {
      cpu.raise(CPU::Interrupt::HorizontalBlank);
    }
    step(430);
  } else {
    if(state.y == screenHeight()) {
      if(io.verticalBlankInterruptEnable) {
        cpu.raise(CPU::Interrupt::VerticalBlank);
      }
      apu.setINT(true);
    }
    step(1710);
    apu.setINT(false);
  }
}

auto VDP::step(uint clocks) -> void {
  while(clocks--) {
    dma.run();
    Thread::step(1);
    synchronize(cpu);
    synchronize(apu);
  }
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 1280 * sizeof(uint32), 1280, 480);
}

auto VDP::power() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 2.0);

  memory::fill(&io, sizeof(IO));
  memory::fill(&state, sizeof(State));

  planeA.power();
  window.power();
  planeB.power();
  sprite.power();
  dma.power();
}

}
