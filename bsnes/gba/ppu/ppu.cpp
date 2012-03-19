#include <gba/gba.hpp>

namespace GBA {

PPU ppu;

void PPU::Enter() { ppu.enter(); }

void PPU::enter() {
  while(true) {
    frame();
    step(279620);
  }
}

void PPU::step(unsigned clocks) {
  clock += clocks;
  if(clock >= 0) co_switch(cpu.thread);
}

void PPU::power() {
  create(PPU::Enter, 16777216);

  for(unsigned n = 0; n < vram.size; n++) vram.data[n] = 0;
  for(unsigned n = 0; n <  oam.size; n++)  oam.data[n] = 0;
  for(unsigned n = 0; n < pram.size; n++) pram.data[n] = 0;
}

void PPU::frame() {
  static uint16_t output[240 * 160];
  static bool once = true;

  if(once) {
    once = false;
    for(signed y = 0; y < 160; y++) {
      uint16_t *dp = output + y * 240;
      for(signed x = 0; x < 240; x++) {
        uint16_t color = sin((x - 60) * 6.283 / 240) * 16 + cos((y - 80) * 6.283 / 160) * 16;
        if(color >= 16) color = 31 - color;
        *dp++ = color;
      }
    }
  }

  interface->videoRefresh(output);
  scheduler.exit(Scheduler::ExitReason::FrameEvent);
}

PPU::PPU() {
  vram.data = new uint8[vram.size = 96 * 1024];
  oam.data = new uint8[oam.size = 1024];
  pram.data = new uint8[pram.size = 1024];
}

}
