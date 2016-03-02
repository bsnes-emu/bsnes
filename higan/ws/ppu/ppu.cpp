#include <ws/ws.hpp>

namespace WonderSwan {

PPU ppu;
#include "io.cpp"
#include "render.cpp"
#include "video.cpp"

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  if(status.vclk < 144) {
    for(uint x = 0; x < 224; x++) {
      renderBack();
      renderScreenOne();
      renderScreenTwo();
      renderSprite();
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
  iomap[0x0060] = this;

  for(auto& n : output) n = 0;

  status.vclk = 0;
  status.hclk = 0;

  r.lineCompare = 0xff;

  video.power();
}

}
