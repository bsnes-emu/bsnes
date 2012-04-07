#include <gba/gba.hpp>

//pixel:      4 cycles

//hdraw:    240 pixels ( 960 cycles)
//hblank:    68 pixels ( 272 cycles)
//scanline: 308 pixels (1232 cycles)

//vdraw:    160 scanlines (197120 cycles)
//vblank:    68 scanlines ( 83776 cycles)
//frame:    228 scanlines (280896 cycles)

namespace GBA {

#include "registers.cpp"
#include "background.cpp"
#include "object.cpp"
#include "screen.cpp"
#include "mmio.cpp"
PPU ppu;

void PPU::Enter() { ppu.enter(); }

void PPU::enter() {
  while(true) {
    scanline();
  }
}

void PPU::step(unsigned clocks) {
  clock += clocks;
  if(clock >= 0) co_switch(cpu.thread);
}

void PPU::power() {
  create(PPU::Enter, 16777216);

  for(unsigned n = 0; n < vram.size; n++) vram.data[n] = 0;
  for(unsigned n = 0; n <  oam.size; n++)  oam.data[n] = 0;
  for(unsigned n = 0; n < pram.size; n++) pram.data[n] = 0;
  for(unsigned n = 0; n < 240 * 160; n++) output[n] = 0;

  regs.control = 0;
  regs.greenswap = 0;
  regs.status = 0;
  regs.vcounter = 0;
  for(auto &bg : regs.bg) {
    bg.control = 0;
    bg.hoffset = 0;
    bg.voffset = 0;
    bg.pa = 0;
    bg.pb = 0;
    bg.pc = 0;
    bg.pd = 0;
    bg.x = 0;
    bg.y = 0;
    bg.lx = 0;
    bg.ly = 0;
  }
  for(auto &w : regs.window) {
    w.x1 = 0;
    w.x2 = 0;
    w.y1 = 0;
    w.y2 = 0;
    w.in = 0;
    w.out = 0;
  }
  regs.windowobj.in = 0;
  regs.mosaic.bghsize = 0;
  regs.mosaic.bgvsize = 0;
  regs.mosaic.objhsize = 0;
  regs.mosaic.objvsize = 0;
  regs.blend.control = 0;
  regs.blend.eva = 0;
  regs.blend.evb = 0;
  regs.blend.evy = 0;

  for(unsigned n = 0x000; n <= 0x055; n++) bus.mmio[n] = this;
}

void PPU::scanline() {
  regs.status.vblank = regs.vcounter >= 160 && regs.vcounter <= 226;
  regs.status.vcoincidence = regs.vcounter == regs.status.vcompare;

  if(regs.vcounter ==   0) {
    frame();

    regs.bg[2].lx = regs.bg[2].x;
    regs.bg[2].ly = regs.bg[2].y;

    regs.bg[3].lx = regs.bg[3].x;
    regs.bg[3].ly = regs.bg[3].y;
  }

  if(regs.vcounter == 160) {
    if(regs.status.irqvblank) cpu.regs.irq.flag.vblank = 1;
    cpu.pending.dma.vblank = true;
  }

  if(regs.status.irqvcoincidence) {
    if(regs.status.vcoincidence) cpu.regs.irq.flag.vcoincidence = 1;
  }

  if(regs.vcounter < 160) {
    for(unsigned x = 0; x < 240; x++) {
      layer[0][x].exists = false;
      layer[1][x].exists = false;
      layer[2][x].exists = false;
      layer[3][x].exists = false;
    }

    render_backgrounds();
    render_objects();
    render_screen();
  }

  step(960);
  regs.status.hblank = 1;
  if(regs.status.irqhblank) cpu.regs.irq.flag.hblank = 1;
  cpu.pending.dma.hblank = true;

  step(240);
  regs.status.hblank = 0;
  if(regs.vcounter < 160) cpu.pending.dma.hdma = true;

  step(32);
  if(++regs.vcounter == 228) regs.vcounter = 0;
}

void PPU::frame() {
  interface->videoRefresh(output);
  scheduler.exit(Scheduler::ExitReason::FrameEvent);
}

PPU::PPU() {
  vram.data = new uint8[vram.size = 96 * 1024];
  oam.data = new uint8[oam.size = 1024];
  pram.data = new uint8[pram.size = 1024];
  output = new uint16[240 * 160];
}

PPU::~PPU() {
  delete[] output;
}

}
