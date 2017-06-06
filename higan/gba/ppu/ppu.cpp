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
#include "window.cpp"
#include "screen.cpp"
#include "io.cpp"
#include "memory.cpp"
#include "serialization.cpp"

auto PPU::blank() -> bool {
  return io.forceBlank || cpu.regs.mode == CPU::Registers::Mode::Stop;
}

PPU::PPU() {
  output = new uint32[240 * 160];
}

PPU::~PPU() {
  delete[] output;
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PPU::main() -> void {
  cpu.keypadRun();

  io.vblank = io.vcounter >= 160 && io.vcounter <= 226;
  io.vcoincidence = io.vcounter == io.vcompare;

  if(io.vcounter == 0) {
    frame();

    bg2.io.lx = bg2.io.x;
    bg2.io.ly = bg2.io.y;

    bg3.io.lx = bg3.io.x;
    bg3.io.ly = bg3.io.y;
  }

  if(io.vcounter == 160) {
    if(io.irqvblank) cpu.regs.irq.flag |= CPU::Interrupt::VBlank;
    cpu.dmaVblank();
  }

  if(io.irqvcoincidence) {
    if(io.vcoincidence) cpu.regs.irq.flag |= CPU::Interrupt::VCoincidence;
  }

  if(io.vcounter < 160) {
    uint y = io.vcounter;
    bg0.scanline(y);
    bg1.scanline(y);
    bg2.scanline(y);
    bg3.scanline(y);
    objects.scanline(y);
    for(uint x : range(240)) {
      bg0.run(x, y);
      bg1.run(x, y);
      bg2.run(x, y);
      bg3.run(x, y);
      objects.run(x, y);
      window0.run(x, y);
      window1.run(x, y);
      window2.output = objects.output.window;
      window3.output = true;
      uint15 color = screen.run(x, y);
      output[y * 240 + x] = color;
      step(4);
    }
  } else {
    step(960);
  }

  io.hblank = 1;
  if(io.irqhblank) cpu.regs.irq.flag |= CPU::Interrupt::HBlank;
  if(io.vcounter < 160) cpu.dmaHblank();

  step(240);
  io.hblank = 0;
  if(io.vcounter < 160) cpu.dmaHDMA();

  step(32);
  if(++io.vcounter == 228) io.vcounter = 0;
}

auto PPU::frame() -> void {
  player.frame();
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::refresh() -> void {
  Emulator::video.refresh(output, 240 * sizeof(uint32), 240, 160);
}

auto PPU::power() -> void {
  create(PPU::Enter, 16'777'216);

  for(uint n = 0x000; n <= 0x055; n++) bus.io[n] = this;

  for(uint n = 0; n < 240 * 160; n++) output[n] = 0;

  for(uint n = 0; n < 96 * 1024; n++) vram[n] = 0x00;
  for(uint n = 0; n < 1024; n += 2) writePRAM(n, Half, 0x0000);
  for(uint n = 0; n < 1024; n += 2) writeOAM(n, Half, 0x0000);

  memory::fill(&io, sizeof(IO));
  for(auto& object : this->object) object = {};
  for(auto& param : this->objectParam) param = {};

  bg0.power(BG0);
  bg1.power(BG1);
  bg2.power(BG2);
  bg3.power(BG3);
  objects.power();
  window0.power(IN0);
  window1.power(IN1);
  window2.power(IN2);
  window3.power(OUT);
  screen.power();
}

}
