#include <pce/pce.hpp>

namespace PCEngine {

VDC vdc;
#include "memory.cpp"
#include "io.cpp"
#include "irq.cpp"
#include "dma.cpp"
#include "background.cpp"
#include "sprite.cpp"

auto VDC::Enter() -> void {
  while(true) scheduler.synchronize(), vdc.main();
}

auto VDC::main() -> void {
  auto output = buffer + 1140 * vce.vclock;

  //todo: if block breaks TV Sports Basketball
  //if(vce.vclock >= vce.vstart && vce.voffset < vce.vlength) {
    background.scanline(vce.voffset);
    sprite.scanline(vce.voffset);
  //}

  while(vce.hclock < 1140) {
    uint9 color = 0;

    if(vce.vclock >= vce.vstart && vce.voffset < vce.vlength
    && vce.hclock >= vce.hstart && vce.hoffset < vce.hlength
    ) {
      background.run(vce.hoffset, vce.voffset);
      sprite.run(vce.hoffset, vce.voffset);
      vce.hoffset++;

      if(sprite.color && sprite.priority) {
        color = sprite.color();
      } else if(background.color) {
        color = background.color();
      } else if(sprite.color) {
        color = sprite.color();
      } else {
        color = cram.read(0);
      }
    }

    if(vce.clock >= 2) *output++ = color;
    if(vce.clock >= 2) *output++ = color;
    if(vce.clock >= 3) *output++ = color;
    if(vce.clock >= 4) *output++ = color;

    step(vce.clock);
  }

  if(vce.vclock == io.lineCoincidence - (66 - vce.vstart)) {
    irq.raise(IRQ::Line::LineCoincidence);
  }

  step(1365 - vce.hclock);
  scanline();
}

auto VDC::scanline() -> void {
  vce.hclock = 0;
  vce.hoffset = 0;
  vce.hstart = vce.horizontalDisplayStart * vce.clock;
  vce.hlength = (vce.horizontalDisplayLength + 1) << 3;
  if(vce.clock == 2) vce.hstart += 0;
  if(vce.clock == 3) vce.hstart += 0;
  if(vce.clock == 4) vce.hstart += 0;

  vce.vclock++;
  if(vce.vclock >= vce.vstart) vce.voffset++;

  if(vce.vclock == 262) {
    frame();
  }

  if(vce.vclock == vce.vstart + vce.vlength) {
    irq.raise(IRQ::Line::Vblank);
    dma.satbStart();
  }
}

auto VDC::frame() -> void {
  scheduler.exit(Scheduler::Event::Frame);

  vce.vclock = 0;
  vce.voffset = 0;
  vce.vstart = vce.verticalDisplayStart - 2;
  vce.vlength = min(242, vce.verticalDisplayLength + 1);
}

auto VDC::step(uint clocks) -> void {
  vce.hclock += clocks;
  Thread::step(clocks);
  dma.step(clocks);
  synchronize(cpu);
}

auto VDC::refresh() -> void {
  Emulator::video.refresh(buffer + 1140 * vce.vstart, 1140 * sizeof(uint32), 1140, 242);
}

auto VDC::power() -> void {
  create(VDC::Enter, system.colorburst() * 6.0);

  for(auto& pixel : buffer) pixel = 0;
  memory::fill(&vram, sizeof(VRAM));
  memory::fill(&satb, sizeof(SATB));
  memory::fill(&cram, sizeof(CRAM));
  memory::fill(&irq, sizeof(IRQ));
  memory::fill(&dma, sizeof(DMA));
  memory::fill(&vce, sizeof(VCE));
  memory::fill(&io, sizeof(IO));
  memory::fill(&background, sizeof(Background));
  memory::fill(&sprite, sizeof(Sprite));

  vce.clock = 4;
}

}
