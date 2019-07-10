#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU& ppubase = ppu;

PPUfast ppufast;
#include "io.cpp"
#include "line.cpp"
#include "background.cpp"
#include "mode7.cpp"
#include "mode7hd.cpp"
#include "object.cpp"
#include "window.cpp"
#include "serialization.cpp"

auto PPUfast::interlace() const -> bool { return ppubase.display.interlace; }
auto PPUfast::overscan() const -> bool { return ppubase.display.overscan; }
auto PPUfast::vdisp() const -> uint { return ppubase.display.vdisp; }
auto PPUfast::hires() const -> bool { return latch.hires; }
auto PPUfast::hd() const -> bool { return latch.hd; }
auto PPUfast::ss() const -> bool { return latch.ss; }
auto PPUfast::hdScale() const -> uint { return configuration.hacks.ppu.mode7.scale; }
auto PPUfast::hdPerspective() const -> bool { return configuration.hacks.ppu.mode7.perspective; }
auto PPUfast::hdSupersample() const -> bool { return configuration.hacks.ppu.mode7.supersample; }
auto PPUfast::hdMosaic() const -> bool { return configuration.hacks.ppu.mode7.mosaic; }

PPUfast::PPUfast() {
  for(uint l : range(16)) {
    for(uint r : range(32)) {
      for(uint g : range(32)) {
        for(uint b : range(32)) {
          double luma = (double)l / 15.0;
          uint ar = (luma * r + 0.5);
          uint ag = (luma * g + 0.5);
          uint ab = (luma * b + 0.5);
          lightTable[l][(r << 10) + (g << 5) + b] = (ab << 10) + (ag << 5) + ar;
        }
      }
    }
  }

  tilecache[TileMode::BPP2] = new uint8_t[4096 * 8 * 8];
  tilecache[TileMode::BPP4] = new uint8_t[2048 * 8 * 8];
  tilecache[TileMode::BPP8] = new uint8_t[1024 * 8 * 8];

  for(uint y : range(lines.size())) {
    lines[y].y = y;
  }
}

PPUfast::~PPUfast() {
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
    latch.overscan = io.overscan;
    latch.hires = false;
    latch.hd = false;
    latch.ss = false;
    io.obj.timeOver = false;
    io.obj.rangeOver = false;
  }

  if(vcounter() > 0 && vcounter() < vdisp()) {
    latch.hires |= io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;
    latch.hd |= io.bgMode == 7 && hdScale() > 1 && hdSupersample() == 0;
    latch.ss |= io.bgMode == 7 && hdScale() > 1 && hdSupersample() == 1;
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
      pitch  = 512 << !interlace();
      width  = 256 << hires();
      height = 240 << interlace();
    } else {
      pitch  = 256 * hdScale();
      width  = 256 * hdScale();
      height = 240 * hdScale();
    }

    //clear the areas of the screen that won't be rendered:
    //previous video frames may have drawn data here that would now be stale otherwise.
    if(!latch.overscan && pitch != frame.pitch && width != frame.width && height != frame.height) {
      for(uint y : range(240)) {
        if(y >= 8 && y <= 230) continue;  //these scanlines are always rendered.
        auto output = this->output + (!hd() ? (y * 1024 + (interlace() && field() ? 512 : 0)) : (y * 256 * hdScale() * hdScale()));
        auto width = (!hd() ? (!hires() ? 256 : 512) : (256 * hdScale() * hdScale()));
        memory::fill<uint16>(output, width);
      }
    }

    platform->videoFrame(output, pitch * sizeof(uint16), width, height, hd() ? hdScale() : 1);

    frame.pitch  = pitch;
    frame.width  = width;
    frame.height = height;
  }
  if(system.frameCounter++ >= system.frameSkip) system.frameCounter = 0;
}

auto PPUfast::load() -> bool {
  return true;
}

auto PPUfast::power(bool reset) -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill<uint16_t>(output, 1024 * 960);

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

  frame = {};
}

}
