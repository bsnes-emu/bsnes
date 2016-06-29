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

auto PPU::step(uint clocks) -> void {
  while(clocks--) {
    if(io.ly == 240 && io.lx == 340) io.nmiHold = 1;
    if(io.ly == 241 && io.lx ==   0) io.nmiFlag = io.nmiHold;
    if(io.ly == 241 && io.lx ==   2) cpu.nmiLine(io.nmiEnable && io.nmiFlag);

    if(io.ly == 260 && io.lx == 340) io.spriteZeroHit = 0, io.spriteOverflow = 0;

    if(io.ly == 260 && io.lx == 340) io.nmiHold = 0;
    if(io.ly == 261 && io.lx ==   0) io.nmiFlag = io.nmiHold;
    if(io.ly == 261 && io.lx ==   2) cpu.nmiLine(io.nmiEnable && io.nmiFlag);

    clock += 4;
    if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);

    io.lx++;
  }
}

auto PPU::scanline() -> void {
  io.lx = 0;
  if(++io.ly == 262) {
    io.ly = 0;
    frame();
  }
  cartridge.scanline(io.ly);
}

auto PPU::frame() -> void {
  io.field++;
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 240);
}

auto PPU::power() -> void {
}

auto PPU::reset() -> void {
  create(PPU::Enter, 21'477'272);

  memory::fill(&io, sizeof(IO));
  memory::fill(&latch, sizeof(Latches));
  io.vramIncrement = 1;

  for(auto& n : ciram ) n = 0;
  for(auto& n : cgram ) n = 0;
  for(auto& n : oam   ) n = 0;

  for(auto& n : buffer) n = 0;
}

}
