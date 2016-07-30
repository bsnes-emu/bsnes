#include <gba/gba.hpp>

//pixel:      4 cycles

//hdraw:    240 pixels ( 960 cycles)
//hblank:    68 pixels ( 272 cycles)
//scanline: 308 pixels (1232 cycles)

//vdraw:    160 scanlines (197120 cycles)
//vblank:    68 scanlines ( 83776 cycles)
//frame:    228 scanlines (280896 cycles)

namespace GameBoyAdvance {

PPU ppu;
#include "background.cpp"
#include "object.cpp"
#include "mosaic.cpp"
#include "screen.cpp"
#include "io.cpp"
#include "memory.cpp"
#include "serialization.cpp"

PPU::PPU() {
  output = new uint32[240 * 160];

  regs.bg[0].id = BG0;
  regs.bg[1].id = BG1;
  regs.bg[2].id = BG2;
  regs.bg[3].id = BG3;
}

PPU::~PPU() {
  delete[] output;
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  scanline();
}

auto PPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PPU::power() -> void {
  create(PPU::Enter, 16'777'216);

  for(uint n = 0; n < 240 * 160; n++) output[n] = 0;

  for(uint n = 0; n < 1024; n += 2) writePRAM(n, Half, 0x0000);
  for(uint n = 0; n < 1024; n += 2) writeOAM(n, Half, 0x0000);

  regs.control.bgmode = 0;
  regs.control.cgbmode = 0;
  regs.control.frame = 0;
  regs.control.hblank = 0;
  regs.control.objmapping = 0;
  regs.control.forceblank = 0;
  for(auto& enable : regs.control.enable) enable = 0;
  for(auto& enablewindow : regs.control.enablewindow) enablewindow = 0;
  regs.greenswap = 0;
  regs.status.vblank = 0;
  regs.status.hblank = 0;
  regs.status.vcoincidence = 0;
  regs.status.irqvblank = 0;
  regs.status.irqhblank = 0;
  regs.status.irqvcoincidence = 0;
  regs.status.vcompare = 0;
  regs.vcounter = 0;
  for(auto& bg : regs.bg) {
    bg.control.priority = 0;
    bg.control.characterbaseblock = 0;
    bg.control.unused = 0;
    bg.control.mosaic = 0;
    bg.control.colormode = 0;
    bg.control.screenbaseblock = 0;
    bg.control.affinewrap = 0;
    bg.control.screensize = 0;
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
  for(auto& w : regs.window) {
    w.x1 = 0;
    w.x2 = 0;
    w.y1 = 0;
    w.y2 = 0;
  }
  for(auto& flags : regs.windowflags) {
    for(auto& enable : flags.enable) enable = 0;
  }
  regs.mosaic.bghsize = 0;
  regs.mosaic.bgvsize = 0;
  regs.mosaic.objhsize = 0;
  regs.mosaic.objvsize = 0;
  for(auto& above : regs.blend.control.above) above = 0;
  regs.blend.control.mode = 0;
  for(auto& below : regs.blend.control.below) below = 0;
  regs.blend.eva = 0;
  regs.blend.evb = 0;
  regs.blend.evy = 0;

  for(uint n = 0x000; n <= 0x055; n++) bus.io[n] = this;
}

auto PPU::scanline() -> void {
  cpu.keypadRun();

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
    if(regs.status.irqvblank) cpu.regs.irq.flag |= CPU::Interrupt::VBlank;
    cpu.dmaVblank();
  }

  if(regs.status.irqvcoincidence) {
    if(regs.status.vcoincidence) cpu.regs.irq.flag |= CPU::Interrupt::VCoincidence;
  }

  if(regs.vcounter < 160) {
    if(regs.control.forceblank || cpu.regs.mode == CPU::Registers::Mode::Stop) {
      renderForceBlank();
    } else {
      for(auto x : range(240)) {
        windowmask[0][x] = false;
        windowmask[1][x] = false;
        windowmask[2][x] = false;
        layer[OBJ][x].write(false);
        layer[BG0][x].write(false);
        layer[BG1][x].write(false);
        layer[BG2][x].write(false);
        layer[BG3][x].write(false);
        layer[SFX][x].write(true, 3, pram[0]);
      }
      renderWindow(0);
      renderWindow(1);
      renderObjects();
      renderBackgrounds();
      renderScreen();
    }
  }

  step(960);
  regs.status.hblank = 1;
  if(regs.status.irqhblank) cpu.regs.irq.flag |= CPU::Interrupt::HBlank;
  if(regs.vcounter < 160) cpu.dmaHblank();

  step(240);
  regs.status.hblank = 0;
  if(regs.vcounter < 160) cpu.dmaHDMA();

  step(32);
  if(++regs.vcounter == 228) regs.vcounter = 0;
}

auto PPU::frame() -> void {
  player.frame();
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::refresh() -> void {
  Emulator::video.refresh(output, 240 * sizeof(uint32), 240, 160);
}

}
