#include <ws/ws.hpp>

namespace WonderSwan {

PPU ppu;
#include "io.cpp"
#include "latch.cpp"
#include "render-mono.cpp"
#include "render-color.cpp"
#include "video.cpp"
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
      if(!r.lcdEnable) {
        s.pixel = {Pixel::Source::Back, 0x000};
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
      output[s.vclk * 224 + s.hclk] = s.pixel.color;
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
  video.refresh();
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::step(uint clocks) -> void {
  s.hclk += clocks;

  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto PPU::power() -> void {
  create(PPU::Enter, 3'072'000);

  bus.map(this, 0x0000, 0x0017);
  bus.map(this, 0x001c, 0x003f);
  bus.map(this, 0x00a2);
  bus.map(this, 0x00a4, 0x00ab);

  for(auto& n : output) n = 0;
  memory::fill(&s, sizeof(State));
  memory::fill(&l, sizeof(Latches));
  memory::fill(&r, sizeof(Registers));

  r.lcdEnable = 1;
  r.vtotal = 158;
  r.vblank = 155;

  video.power();
}

}
