#include <pce/pce.hpp>

namespace PCEngine {

VDC vdc;
#include "io.cpp"
#include "irq.cpp"
#include "dma.cpp"
#include "background.cpp"
#include "sprite.cpp"

auto VDC::Enter() -> void {
  while(true) scheduler.synchronize(), vdc.main();
}

auto VDC::main() -> void {
  //1365 cycles/scanline
  state.x = 0;
  uint y = state.y;
  auto output = buffer + y * 512;
  background.scanline(y);
  sprite.scanline(y);
  for(uint x : range(256)) {
    if(y < 240) {
      background.run(x, y);
      sprite.run(x, y);

      if(sprite.color && sprite.priority) {
        *output++ = sprite.color();
      } else if(background.color) {
        *output++ = background.color();
      } else if(sprite.color) {
        *output++ = sprite.color();
      } else {
        *output++ = cram[0];
      }
    }
    step(4);
  }
  step(341);

  if(++state.y == 262) {
    state.y = 0;
  }
  if(state.y == (io.lineCoincidence - 64)) irq.raise(IRQ::Line::LineCoincidence);
  if(state.y == 240) {
    irq.raise(IRQ::Line::Vblank);
    dma.satbStart();
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto VDC::step(uint clocks) -> void {
  state.x += clocks;
  Thread::step(clocks);
  dma.step(clocks);
  synchronize(cpu);
}

auto VDC::refresh() -> void {
  Emulator::video.refresh(buffer, 512 * sizeof(uint32), 256, 240);
}

auto VDC::power() -> void {
  create(VDC::Enter, system.colorburst() * 6.0);

  for(auto& pixel : buffer) pixel = 0;
  for(auto& word : vram) word = 0x0000;
  for(auto& word : satb) word = 0x0000;
  for(auto& word : cram) word = 0x0000;
  memory::fill(&state, sizeof(State));
  memory::fill(&irq, sizeof(IRQ));
  memory::fill(&dma, sizeof(DMA));
  memory::fill(&io, sizeof(IO));
  memory::fill(&background, sizeof(Background));
  memory::fill(&sprite, sizeof(Sprite));
}

}
