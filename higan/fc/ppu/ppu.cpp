#include <fc/fc.hpp>

namespace Famicom {

PPU ppu;
#include "memory.cpp"
#include "render.cpp"
#include "serialization.cpp"

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  renderScanline();
}

auto PPU::tick() -> void {
  if(r.ly == 240 && r.lx == 340) r.nmiHold = 1;
  if(r.ly == 241 && r.lx ==   0) r.nmiFlag = r.nmiHold;
  if(r.ly == 241 && r.lx ==   2) cpu.nmiLine(r.nmiEnable && r.nmiFlag);

  if(r.ly == 260 && r.lx == 340) r.spriteZeroHit = 0, r.spriteOverflow = 0;

  if(r.ly == 260 && r.lx == 340) r.nmiHold = 0;
  if(r.ly == 261 && r.lx ==   0) r.nmiFlag = r.nmiHold;
  if(r.ly == 261 && r.lx ==   2) cpu.nmiLine(r.nmiEnable && r.nmiFlag);

  clock += 4;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);

  r.lx++;
}

auto PPU::scanline() -> void {
  r.lx = 0;
  if(++r.ly == 262) {
    r.ly = 0;
    frame();
  }
  cartridge.scanline(r.ly);
}

auto PPU::frame() -> void {
  r.field ^= 1;
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 240);
}

auto PPU::power() -> void {
}

auto PPU::reset() -> void {
  create(PPU::Enter, 21'477'272);

  memory::fill(&r, sizeof(Registers));
  memory::fill(&l, sizeof(Latches));
  r.vramIncrement = 1;

  for(auto& n : ciram ) n = 0;
  for(auto& n : cgram ) n = 0;
  for(auto& n : oam   ) n = 0;

  for(auto& n : buffer) n = 0;
}

}
