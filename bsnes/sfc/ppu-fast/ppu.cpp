#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU& ppubase = ppu;

PPUfast ppufast;
#include "io.cpp"
#include "line.cpp"
#include "background.cpp"
#include "mode7.cpp"
#include "mode7hires.cpp"
#include "mode7hd.cpp"
#include "object.cpp"
#include "window.cpp"
#include "serialization.cpp"

auto PPUfast::interlace() const -> bool { return ppubase.display.interlace; }
auto PPUfast::overscan() const -> bool { return ppubase.display.overscan; }
auto PPUfast::vdisp() const -> uint { return ppubase.display.vdisp; }
auto PPUfast::hires() const -> bool { return latch.hires; }
auto PPUfast::hd() const -> bool { return latch.hd; }
auto PPUfast::hdScale() const -> uint { return configuration.hacks.ppu.mode7.scale; }
auto PPUfast::hdPerspective() const -> bool { return configuration.hacks.ppu.mode7.perspective; }

PPUfast::PPUfast() {
  output = new uint32[2304 * 2304] + 72 * 2304;  //overscan offset
  tilecache[TileMode::BPP2] = new uint8[4096 * 8 * 8];
  tilecache[TileMode::BPP4] = new uint8[2048 * 8 * 8];
  tilecache[TileMode::BPP8] = new uint8[1024 * 8 * 8];

  for(uint y : range(lines.size())) {
    lines[y].y = y;
  }
}

PPUfast::~PPUfast() {
  delete[] (output - 72 * 2304);  //overscan offset
  delete[] tilecache[TileMode::BPP2];
  delete[] tilecache[TileMode::BPP4];
  delete[] tilecache[TileMode::BPP8];
}

auto PPUfast::Enter() -> void {
  while(true) scheduler.synchronize(), ppufast.main();
}

auto PPUfast::step(uint clocks) -> void {
  tick(clocks);
  Thread::step(clocks);
  synchronize(cpu);
}

auto PPUfast::main() -> void {
  scanline();

  if(system.frameCounter == 0) {
    uint y = vcounter();
    step(512);
    if(y >= 1 && y <= 239) {
      if(io.displayDisable || y >= vdisp()) {
        lines[y].io.displayDisable = true;
      } else {
        memcpy(&lines[y].io, &io, sizeof(io));
        memcpy(&lines[y].cgram, &cgram, sizeof(cgram));
      }
      if(!Line::count) Line::start = y;
      Line::count++;
    }
  }

  step(lineclocks() - hcounter());
}

auto PPUfast::scanline() -> void {
  if(vcounter() == 0) {
    ppubase.display.interlace = io.interlace;
    ppubase.display.overscan = io.overscan;
    latch.hires = false;
    latch.hd = false;
    io.obj.timeOver = false;
    io.obj.rangeOver = false;
  }

  if(vcounter() > 0 && vcounter() < vdisp()) {
    latch.hires |= io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;
    latch.hires |= io.bgMode == 7 && configuration.hacks.ppu.mode7.hires;
    latch.hd |= io.bgMode == 7 && hdScale() > 1;
  }

  if(vcounter() == vdisp() && !io.displayDisable) {
    oamAddressReset();
  }

  if(vcounter() == 240) {
    Line::flush();
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto PPUfast::refresh() -> void {
  if(system.frameCounter == 0) {
    auto output = this->output;
    uint pitch, width, height;
    if(!hd()) {
      if(!overscan()) output -= 7 * 1024;
      pitch  = 512 << !interlace();
      width  = 256 << hires();
      height = 240 << interlace();
    } else {
      if(!overscan()) output -= 7 * 256 * hdScale() * hdScale();
      pitch  = 256 * hdScale();
      width  = 256 * hdScale();
      height = 240 * hdScale();
    }
    Emulator::video.setEffect(Emulator::Video::Effect::ColorBleed, configuration.video.blurEmulation && hires());
    Emulator::video.refresh(output, pitch * sizeof(uint32), width, height);
  }
  if(system.frameCounter++ >= system.frameSkip) system.frameCounter = 0;
}

auto PPUfast::load() -> bool {
  return true;
}

auto PPUfast::power(bool reset) -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill<uint32>(output, 1024 * 960);

  function<auto (uint24, uint8) -> uint8> reader{&PPUfast::readIO, this};
  function<auto (uint24, uint8) -> void> writer{&PPUfast::writeIO, this};
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
  updateVideoMode();

  ItemLimit = !configuration.hacks.ppu.noSpriteLimit ? 32 : 128;
  TileLimit = !configuration.hacks.ppu.noSpriteLimit ? 34 : 128;

  Line::start = 0;
  Line::count = 0;
}

}
