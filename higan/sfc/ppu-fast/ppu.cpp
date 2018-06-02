#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;
#include "io.cpp"
#include "line.cpp"
#include "background.cpp"
#include "mode7.cpp"
#include "object.cpp"
#include "window.cpp"
#include "serialization.cpp"
#include <sfc/ppu/counter/serialization.cpp>

PPU::PPU() {
  output = new uint32[512 * 512];
  output += 16 * 512;  //overscan offset

  tilecache[TileMode::BPP2] = new uint8[4096 * 8 * 8];
  tilecache[TileMode::BPP4] = new uint8[2048 * 8 * 8];
  tilecache[TileMode::BPP8] = new uint8[1024 * 8 * 8];

  for(uint y : range(240)) {
    lines[y].y = y;
  }
}

PPU::~PPU() {
  output -= 16 * 512;  //overscan offset
  delete[] output;
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::step(uint clocks) -> void {
  tick(clocks);
  Thread::step(clocks);
  synchronize(cpu);
}

auto PPU::main() -> void {
  scanline();
  uint y = vcounter();
  step(512);
  if(y >= 1 && y <= vdisp()) {
    memcpy(&lines[y].io, &io, sizeof(io));
    memcpy(&lines[y].cgram, &cgram, sizeof(cgram));
    if(!Line::count) Line::start = y;
    Line::count++;
  }
  step(lineclocks() - hcounter());
}

auto PPU::scanline() -> void {
  if(vcounter() == 0) {
    latch.interlace = io.interlace;
    latch.overscan = io.overscan;
    latch.hires = false;
    io.obj.timeOver = false;
    io.obj.rangeOver = false;
  }

  if(vcounter() > 0 && vcounter() < vdisp()) {
    latch.hires |= io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;
  }

  if(vcounter() == vdisp() && !io.displayDisable) {
    oamAddressReset();
  }

  if(vcounter() == 240) {
    Line::flush();
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto PPU::refresh() -> void {
  auto output = this->output;
  if(!overscan()) output -= 14 * 512;
  auto pitch  = 512 << !interlace();
  auto width  = 256 << hires();
  auto height = 240 << interlace();
  if(!hires()) Emulator::video.setEffect(Emulator::Video::Effect::ColorBleed, false);
  Emulator::video.refresh(output, pitch * sizeof(uint32), width, height);
  if(!hires()) Emulator::video.setEffect(Emulator::Video::Effect::ColorBleed, settings.blurEmulation);
}

auto PPU::load(Markup::Node node) -> bool {
  return true;
}

auto PPU::power(bool reset) -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill<uint32>(output, 512 * 480);

  function<auto (uint24, uint8) -> uint8> reader{&PPU::readIO, this};
  function<auto (uint24, uint8) -> void> writer{&PPU::writeIO, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");

  if(!reset) {
    for(auto address : range(32768)) {
      vram[address] = 0x0000;
      updateTiledata(address);
    }
    for(auto& color : cgram) color = 0x0000;
    for(auto& object : objects) object = {};
  }

  latch = {};
  io = {};

  Line::start = 0;
  Line::count = 0;
}

}
