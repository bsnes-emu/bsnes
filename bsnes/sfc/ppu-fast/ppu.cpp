#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU& ppubase = ppu;

#define PPU PPUfast
#define ppu ppufast

PPU ppu;
#include "io.cpp"
#include "line.cpp"
#include "background.cpp"
#include "mode7.cpp"
#include "mode7hd.cpp"
#include "object.cpp"
#include "window.cpp"
#include "serialization.cpp"

auto PPU::interlace() const -> bool { return ppubase.display.interlace; }
auto PPU::overscan() const -> bool { return ppubase.display.overscan; }
auto PPU::vdisp() const -> uint { return ppubase.display.vdisp; }
auto PPU::hires() const -> bool { return latch.hires; }
auto PPU::hd() const -> bool { return latch.hd; }
auto PPU::ss() const -> bool { return latch.ss; }
#undef ppu
auto PPU::hdScale() const -> uint { return configuration.hacks.ppu.mode7.scale; }
auto PPU::hdPerspective() const -> bool { return configuration.hacks.ppu.mode7.perspective; }
auto PPU::hdSupersample() const -> bool { return configuration.hacks.ppu.mode7.supersample; }
auto PPU::hdMosaic() const -> bool { return configuration.hacks.ppu.mode7.mosaic; }
auto PPU::deinterlace() const -> bool { return configuration.hacks.ppu.deinterlace; }
auto PPU::renderCycle() const -> uint { return configuration.hacks.ppu.renderCycle; }
auto PPU::noVRAMBlocking() const -> bool { return configuration.hacks.ppu.noVRAMBlocking; }
#define ppu ppufast

PPU::PPU() {
  output = new uint16_t[2304 * 2160]();

  for(uint l : range(16)) {
    lightTable[l] = new uint16_t[32768];
    for(uint r : range(32)) {
      for(uint g : range(32)) {
        for(uint b : range(32)) {
          double luma = (double)l / 15.0;
          uint ar = (luma * r + 0.5);
          uint ag = (luma * g + 0.5);
          uint ab = (luma * b + 0.5);
          lightTable[l][r << 10 | g << 5 | b << 0] = ab << 10 | ag << 5 | ar << 0;
        }
      }
    }
  }

  tilecache[TileMode::BPP2] = new uint8_t[4096 * 8 * 8]();
  tilecache[TileMode::BPP4] = new uint8_t[2048 * 8 * 8]();
  tilecache[TileMode::BPP8] = new uint8_t[1024 * 8 * 8]();

  for(uint y : range(240)) {
    lines[y].y = y;
  }
}

PPU::~PPU() {
  delete[] output;
  for(uint l : range(16)) delete[] lightTable[l];
  delete[] tilecache[TileMode::BPP2];
  delete[] tilecache[TileMode::BPP4];
  delete[] tilecache[TileMode::BPP8];
}

auto PPU::synchronizeCPU() -> void {
  if(scheduler.synchronizingAll()) return;
  if(ppubase.clock >= 0) co_switch(cpu.thread);
}

auto PPU::Enter() -> void {
  while(true) {
    scheduler.synchronizeAll();
    ppu.main();
  }
}

auto PPU::step(uint clocks) -> void {
  tick(clocks);
  ppubase.clock += clocks;
  synchronizeCPU();
}

auto PPU::main() -> void {
  scanline();

  if(system.frameCounter == 0) {
    uint y = vcounter();
    if(y >= 1 && y <= 239) {
      step(renderCycle());
      lines[y].cache();
    }
  }

  step(hperiod() - hcounter());
}

auto PPU::scanline() -> void {
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

  if(vcounter() == vdisp()) {
    if(auto device = controllerPort2.device) device->latch();  //light guns
    if(!io.displayDisable) oamAddressReset();
  }

  if(vcounter() == vdisp()) {  //240
    Line::flush();
    scheduler.leave(Scheduler::Event::Frame);
  }

  if(vcounter() == 240) {
    Line::flush();
  }
}

auto PPU::refresh() -> void {
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

    if(auto device = controllerPort2.device) device->draw(output, pitch * sizeof(uint16), width, height);
    platform->videoFrame(output, pitch * sizeof(uint16), width, height, hd() ? hdScale() : 1);

    frame.pitch  = pitch;
    frame.width  = width;
    frame.height = height;
  }
  if(system.frameCounter++ >= system.frameSkip) system.frameCounter = 0;
}

auto PPU::load() -> bool {
  return true;
}

auto PPU::power(bool reset) -> void {
  PPUcounter::reset();
  memory::fill<uint16>(output, 1024 * 960);

  function<uint8 (uint, uint8)> reader{&PPU::readIO, this};
  function<void  (uint, uint8)> writer{&PPU::writeIO, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");

  if(!reset) {
    for(uint address : range(32768)) {
      vram[address] = 0x0000;
      updateTiledata(address);
    }
    for(auto& color : cgram) color = 0x0000;
    for(auto& object : objects) object = {};
  }

  latch = {};
  io = {};
  updateVideoMode();

  #undef ppu
  ItemLimit = !configuration.hacks.ppu.noSpriteLimit ? 32 : 128;
  TileLimit = !configuration.hacks.ppu.noSpriteLimit ? 34 : 128;

  Line::start = 0;
  Line::count = 0;

  frame = {};
}

}
