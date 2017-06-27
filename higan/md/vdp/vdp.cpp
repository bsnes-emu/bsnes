#include <md/md.hpp>

namespace MegaDrive {

VDP vdp;
#include "memory.cpp"
#include "io.cpp"
#include "dma.cpp"
#include "render.cpp"
#include "background.cpp"
#include "sprite.cpp"
#include "serialization.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  scanline();

  cpu.lower(CPU::Interrupt::HorizontalBlank);
  apu.setINT(false);

  if(state.vcounter == 0) {
    latch.horizontalInterruptCounter = io.horizontalInterruptCounter;
    cpu.lower(CPU::Interrupt::VerticalBlank);
  }

  if(state.vcounter == screenHeight()) {
    if(io.verticalBlankInterruptEnable) {
      cpu.raise(CPU::Interrupt::VerticalBlank);
    }
    //todo: should only stay high for ~2573/2 clocks
    apu.setINT(true);
  }

  if(state.vcounter < screenHeight()) {
    while(state.hcounter < 1280) {
      run();
      step(pixelWidth());
    }

    if(latch.horizontalInterruptCounter-- == 0) {
      latch.horizontalInterruptCounter = io.horizontalInterruptCounter;
      if(io.horizontalBlankInterruptEnable) {
        cpu.raise(CPU::Interrupt::HorizontalBlank);
      }
    }

    step(430);
  } else {
    step(1710);
  }
}

auto VDP::step(uint clocks) -> void {
  state.hcounter += clocks;
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
  create(VDP::Enter, system.frequency() / 2.0);

  memory::fill(&io, sizeof(IO));
  memory::fill(&latch, sizeof(Latch));
  memory::fill(&state, sizeof(State));

  planeA.power();
  window.power();
  planeB.power();
  sprite.power();
  dma.power();
}

}
