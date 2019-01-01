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
  if(s.vtime == 142) {
    latchOAM();
  }

  if(s.vtime < 144) {
    uint y = s.vtime % (r.vtotal + 1);
    auto output = this->output + y * 224;
    latchRegisters();
    latchSprites(y);
    for(uint x : range(224)) {
      s.pixel = {Pixel::Source::Back, 0x000};
      if(r.lcdEnable) {
        renderBack();
        if(l.screenOneEnable) renderScreenOne(x, y);
        if(l.screenTwoEnable) renderScreenTwo(x, y);
        if(l.spriteEnable) renderSprite(x, y);
      }
      *output++ = s.pixel.color;
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

//vtotal+1 = scanlines per frame
//vtotal<143 inhibits vblank and repeats the screen image until vtime=144
//todo: unknown how votal<143 interferes with line compare interrupts
auto PPU::scanline() -> void {
  s.vtime++;
  if(s.vtime >= max(144, r.vtotal + 1)) return frame();
  if(s.vtime == r.lineCompare) cpu.raise(CPU::Interrupt::LineCompare);
  if(s.vtime == 144) {
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
  s.vtime = 0;
  scheduler.exit(Scheduler::Event::Frame);
}

auto PPU::refresh() -> void {
  Emulator::video.refresh(output, 224 * sizeof(uint32), 224, 144);
}

auto PPU::step(uint clocks) -> void {
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
