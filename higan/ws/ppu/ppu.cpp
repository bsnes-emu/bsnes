#include <ws/ws.hpp>

namespace WonderSwan {

PPU ppu;
#include "io.cpp"
#include "latch.cpp"
#include "render.cpp"
#include "serialization.cpp"

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  if(s.vclk == 142) {
    latchOAM();
  }

  if(s.vclk < 144) {
    latchRegisters();
    latchSprites();
    for(auto x : range(224)) {
      s.pixel = {Pixel::Source::Back, 0x000};
      if(r.lcdEnable) {
        renderBack();
        if(l.screenOneEnable) renderScreenOne();
        if(l.screenTwoEnable) renderScreenTwo();
        if(l.spriteEnable) renderSprite();
      }
      output[s.vclk * 224 + x] = s.pixel.color;
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
  if(++s.vclk == 159) frame();
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
}

auto PPU::frame() -> void {
  s.field = !s.field;
  s.vclk = 0;
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::refresh() -> void {
  Emulator::video.refresh(output, 224 * sizeof(uint32), 224, 144);
}

auto PPU::step(uint clocks) -> void {
  s.hclk += clocks;

  Thread::step(clocks);
  synchronize(cpu);
}

auto PPU::power() -> void {
  create(PPU::Enter, 3'072'000);

  bus.map(this, 0x0000, 0x0017);
  bus.map(this, 0x001c, 0x003f);
  bus.map(this, 0x00a2);
  bus.map(this, 0x00a4, 0x00ab);

  for(auto& n : output) n = 0;
  s = {};
  l = {};
  r = {};
}

}
