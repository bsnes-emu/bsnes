#include <ws/ws.hpp>

namespace WonderSwan {

PPU ppu;
#include "io.cpp"
#include "render-mono.cpp"
#include "render-color.cpp"
#include "video.cpp"

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  if(status.vclk < 144) {
    for(uint x = 0; x < 224; x++) {
      if(!system.color()) {
        renderMonoBack();
        renderMonoScreenOne();
        renderMonoScreenTwo();
        renderMonoSprite();
      } else {
        renderColorBack();
        renderColorScreenOne();
        renderColorScreenTwo();
        renderColorSprite();
      }
      output[status.vclk * 224 + status.hclk] = pixel.color;
      step(1);
    }
    for(uint x = 224; x < 256; x++) {
      step(1);
    }
  } else {
    step(256);
  }
  scanline();
}

auto PPU::scanline() -> void {
  status.hclk = 0;
  status.vclk++;
  if(status.vclk == r.lineCompare) {
    cpu.raise(CPU::Interrupt::LineCompare);
  }
  if(status.vclk == 144) {
    cpu.raise(CPU::Interrupt::Vblank);
  }
  if(status.vclk == 159) frame();
}

auto PPU::frame() -> void {
  status.vclk = 0;
  video.refresh();
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::step(uint clocks) -> void {
  status.hclk += clocks;

  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto PPU::power() -> void {
  create(PPU::Enter, 3'072'000);

  for(uint n = 0x0000; n <= 0x0017; n++) iomap[n] = this;
  for(uint n = 0x001c; n <= 0x003f; n++) iomap[n] = this;

  for(auto& n : output) n = 0;

  status.vclk = 0;
  status.hclk = 0;

  r.screenTwoWindowEnable = 0;
  r.screenTwoWindowInvert = 0;
  r.spriteWindowEnable = 0;
  r.spriteEnable = 0;
  r.screenTwoEnable = 0;
  r.screenOneEnable = 0;
  r.backColor = 0;
  r.lineCompare = 0xff;
  r.spriteBase = 0;
  r.spriteFirst = 0;
  r.spriteCount = 0;
  r.screenTwoMapBase = 0;
  r.screenOneMapBase = 0;
  r.screenTwoWindowX0 = 0;
  r.screenTwoWindowY0 = 0;
  r.screenTwoWindowX1 = 0;
  r.screenTwoWindowY1 = 0;
  r.spriteWindowX0 = 0;
  r.spriteWindowY0 = 0;
  r.spriteWindowX1 = 0;
  r.spriteWindowY1 = 0;
  r.scrollOneX = 0;
  r.scrollOneY = 0;
  r.scrollTwoX = 0;
  r.scrollTwoY = 0;
  r.control = 0;
  r.iconAux3 = 0;
  r.iconAux2 = 0;
  r.iconAux1 = 0;
  r.iconHorizontal = 0;
  r.iconVertical = 0;
  r.iconSleep = 0;
  r.vtotal = 158;
  r.vblank = 155;
  for(auto& color : r.pool) color = 0;
  for(auto& p : r.palette) for(auto& color : p.color) color = 0;

  video.power();
}

}
