#include <snes/snes.hpp>

#define SPPU_CPP
namespace SNES {

#include "mmio/mmio.cpp"

#if !defined(DEBUGGER)
  sPPU ppu;
#else
  sPPUDebugger ppu;
#endif

void sPPU::enter() {
  while(true) {
    scanline();

    add_clocks(88);

    if(vcounter() >= 1 && vcounter() <= 224) {
      uint16_t *buffer = output + vcounter() * 1024;
      for(unsigned n = 0; n < 256; n++) {
        uint16 color = (memory::cgram[1] << 8) + (memory::cgram[0] << 0);
        color = light_table[regs.display_brightness][color];
        if(regs.display_disabled) color = 0;
        *buffer++ = color;
        *buffer++ = color;
        add_clocks(4);
      }
    } else {
      add_clocks(1024);
    }

    add_clocks(lineclocks() - 88 - 1024);
  }
}

void sPPU::add_clocks(unsigned clocks) {
  #if 0
  //asynchronous execution
  tick(clocks);
  scheduler.addclocks_ppu(clocks);
  scheduler.sync_ppucpu();
  #else
  //synchronous execution
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    scheduler.addclocks_ppu(2);
    scheduler.sync_ppucpu();
  }
  #endif
}

void sPPU::power() {
  PPU::power();
  reset();
}

void sPPU::reset() {
  PPU::reset();
  PPU::frame();

  memset(output, 0x00, 512 * 480 * sizeof(uint16));
  mmio_reset();
}

void sPPU::scanline() {
  if(vcounter() == 0) frame();
}

void sPPU::frame() {
  PPU::frame();
  system.frame();
}

sPPU::sPPU() {
  //generate light table for INIDISP::d3-d0
  for(unsigned l = 0; l < 16; l++) {
    for(unsigned r = 0; r < 32; r++) {
      for(unsigned g = 0; g < 32; g++) {
        for(unsigned b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          unsigned ar = (luma * r + 0.5);
          unsigned ag = (luma * g + 0.5);
          unsigned ab = (luma * b + 0.5);
          light_table[l][(b << 10) + (g << 5) + r] = (ab << 10) + (ag << 5) + ar;
        }
      }
    }
  }
}

}
