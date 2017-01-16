#include <pce/pce.hpp>

namespace PCEngine {

VDC vdc;
#include "io.cpp"
#include "irq.cpp"
#include "dma.cpp"

auto VDC::Enter() -> void {
  while(true) scheduler.synchronize(), vdc.main();
}

auto VDC::main() -> void {
  //1365 cycles/scanline
  for(uint x : range(256)) {
    step(4);
  }
  step(341);
  scanline();
}

auto VDC::scanline() -> void {
  state.x = 0;
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
  Emulator::video.refresh(buffer, 512 * sizeof(uint32), 512, 484);
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
}

}
