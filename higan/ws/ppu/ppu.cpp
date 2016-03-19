#include <ws/ws.hpp>

namespace WonderSwan {

PPU ppu;
#include "io.cpp"
#include "render-sprite.cpp"
#include "render-mono.cpp"
#include "render-color.cpp"
#include "video.cpp"

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  if(s.vclk < 144) {
    latchRegisters();
    renderSpriteFetch();
    renderSpriteDecode();
    for(auto x : range(224)) {
      if(!r.lcdEnable) {
        pixel = {Pixel::Source::Back, 0x000};
      } else if(!system.color()) {
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
      output[s.vclk * 224 + s.hclk] = pixel.color;
      step(1);
    }
    step(32);
  } else {
    step(256);
  }
  scanline();
  if(r.htimerEnable && r.htimerCounter < r.htimerFrequency) {
    if(++r.htimerCounter == r.htimerFrequency) {
      if(r.htimerRepeat) {
        r.htimerCounter = 0;
      } else {
        r.htimerEnable = false;
      }
      cpu.raise(CPU::Interrupt::HblankTimer);
    }
  }
}

auto PPU::scanline() -> void {
  s.hclk = 0;
  s.vclk++;
  if(s.vclk == r.lineCompare) {
    cpu.raise(CPU::Interrupt::LineCompare);
  }
  if(s.vclk == 144) {
    cpu.raise(CPU::Interrupt::Vblank);
    if(r.vtimerEnable && r.vtimerCounter < r.vtimerFrequency) {
      if(++r.vtimerCounter == r.vtimerFrequency) {
        if(r.vtimerRepeat) {
          r.vtimerCounter = 0;
        } else {
          r.vtimerEnable = false;
        }
        cpu.raise(CPU::Interrupt::VblankTimer);
      }
    }
  }
  if(s.vclk == 159) frame();
}

auto PPU::frame() -> void {
  s.field = !s.field;
  s.vclk = 0;
  video.refresh();
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::step(uint clocks) -> void {
  s.hclk += clocks;

  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto PPU::latchRegisters() -> void {
  l.backColor = r.backColor;

  l.screenOneEnable = r.screenOneEnable;
  l.screenOneMapBase = r.screenOneMapBase;
  l.scrollOneX = r.scrollOneX;
  l.scrollOneY = r.scrollOneY;

  l.screenTwoEnable = r.screenTwoEnable;
  l.screenTwoMapBase = r.screenTwoMapBase;
  l.scrollTwoX = r.scrollTwoX;
  l.scrollTwoY = r.scrollTwoY;
  l.screenTwoWindowEnable = r.screenTwoWindowEnable;
  l.screenTwoWindowInvert = r.screenTwoWindowInvert;
  l.screenTwoWindowX0 = r.screenTwoWindowX0;
  l.screenTwoWindowY0 = r.screenTwoWindowY0;
  l.screenTwoWindowX1 = r.screenTwoWindowX1;
  l.screenTwoWindowY1 = r.screenTwoWindowY1;

  l.spriteEnable = r.spriteEnable;
  l.spriteBase = r.spriteBase;
  l.spriteFirst = r.spriteFirst;
  l.spriteCount = r.spriteCount;
  l.spriteWindowEnable = r.spriteWindowEnable;
  l.spriteWindowX0 = r.spriteWindowX0;
  l.spriteWindowY0 = r.spriteWindowY0;
  l.spriteWindowX1 = r.spriteWindowX1;
  l.spriteWindowY1 = r.spriteWindowY1;
}

auto PPU::power() -> void {
  create(PPU::Enter, 3'072'000);

  bus.map(this, 0x0000, 0x0017);
  bus.map(this, 0x001c, 0x003f);
  bus.map(this, 0x00a2);
  bus.map(this, 0x00a4, 0x00ab);

  for(auto& n : output) n = 0;
  for(auto& n : oam[0]) n = 0;
  for(auto& n : oam[1]) n = 0;

  s.vclk = 0;
  s.hclk = 0;

  r.screenOneEnable = 0;
  r.screenTwoEnable = 0;
  r.spriteEnable = 0;
  r.spriteWindowEnable = 0;
  r.screenTwoWindowInvert = 0;
  r.screenTwoWindowEnable = 0;
  r.backColor = 0;
  r.lineCompare = 0xff;
  r.spriteBase = 0;
  r.spriteFirst = 0;
  r.spriteCount = 0;
  r.screenOneMapBase = 0;
  r.screenTwoMapBase = 0;
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
  r.lcdEnable = 1;
  r.lcdContrast = 0;
  r.lcdUnknown = 0;
  r.iconSleep = 0;
  r.iconVertical = 0;
  r.iconHorizontal = 0;
  r.iconAux1 = 0;
  r.iconAux2 = 0;
  r.iconAux3 = 0;
  r.vtotal = 158;
  r.vblank = 155;
  for(auto& color : r.pool) color = 0;
  for(auto& p : r.palette) for(auto& color : p.color) color = 0;
  r.htimerEnable = 0;
  r.htimerRepeat = 0;
  r.vtimerEnable = 0;
  r.vtimerRepeat = 0;
  r.htimerFrequency = 0;
  r.vtimerFrequency = 0;
  r.htimerCounter = 0;
  r.vtimerCounter = 0;

  video.power();
}

}
