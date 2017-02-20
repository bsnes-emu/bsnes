#include <pce/pce.hpp>

namespace PCEngine {

VDC vdc0;
VDC vdc1;
#include "memory.cpp"
#include "io.cpp"
#include "irq.cpp"
#include "dma.cpp"
#include "background.cpp"
#include "sprite.cpp"
#include "serialization.cpp"

auto VDC::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(vdc0.active()) vdc0.main();
    if(vdc1.active()) vdc1.main();
  }
}

auto VDC::main() -> void {
  if(Model::PCEngine() && vdc1.active()) return step(frequency());

  if(timing.vclock == 0) {
    timing.voffset = 0;
    timing.vstart = max((uint8)2, timing.verticalDisplayStart) - 2;
    timing.vlength = min(242, timing.verticalDisplayLength + 1);
  }

  timing.hclock = 0;
  timing.hoffset = 0;
  timing.hstart = timing.horizontalDisplayStart;
  timing.hlength = (timing.horizontalDisplayLength + 1) << 3;

  if(timing.vclock >= timing.vstart && timing.voffset < timing.vlength) {
    background.scanline(timing.voffset);
    sprite.scanline(timing.voffset);

    step(timing.hstart);

    while(timing.hclock < 1360 && timing.hoffset < timing.hlength) {
      data = 0;

      background.run(timing.hoffset, timing.voffset);
      sprite.run(timing.hoffset, timing.voffset);

      if(sprite.color && sprite.priority) {
        data = 1 << 8 | sprite.palette << 4 | sprite.color << 0;
      } else if(background.color) {
        data = 0 << 8 | background.palette << 4 | background.color << 0;
      } else if(sprite.color) {
        data = 1 << 8 | sprite.palette << 4 | sprite.color << 0;
      }

      step(vce.clock());
      timing.hoffset++;
    }

    if(timing.voffset == io.lineCoincidence - 64) {
      irq.raise(IRQ::Line::LineCoincidence);
    }

    timing.voffset++;
  }

  data = 0;
  step(1365 - timing.hclock);

  if(timing.vclock == timing.vstart + timing.vlength) {
    irq.raise(IRQ::Line::Vblank);
    dma.satbStart();
  }

  if(++timing.vclock == 262) {
    timing.vclock = 0;
  }
}

auto VDC::scanline() -> void {
  timing.hpulse = true;
}

auto VDC::frame() -> void {
  timing.vpulse = true;
}

auto VDC::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(vce);

  timing.hclock += clocks;
  dma.step(clocks);
}

auto VDC::power() -> void {
  create(VDC::Enter, system.colorburst() * 6.0);

  memory::fill(&vram, sizeof(VRAM));
  memory::fill(&satb, sizeof(SATB));
  memory::fill(&timing, sizeof(Timing));
  memory::fill(&irq, sizeof(IRQ));
  memory::fill(&dma, sizeof(DMA));
  memory::fill(&io, sizeof(IO));
  memory::fill(&background, sizeof(Background));
  memory::fill(&sprite, sizeof(Sprite));

  dma.vdc = this;
  background.vdc = this;
  sprite.vdc = this;
}

}
