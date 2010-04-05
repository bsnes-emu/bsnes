#include <snes/snes.hpp>

#define SPPU_CPP
namespace SNES {

#include "background/background.cpp"
#include "mmio/mmio.cpp"
#include "screen/screen.cpp"

#if !defined(DEBUGGER)
  sPPU ppu;
#else
  sPPUDebugger ppu;
#endif

void sPPU::enter() {
  while(true) {
    scanline();

    add_clocks(88);

    //mosaic
    if(vcounter() == 1) {
      bg1.regs.mosaic_y = 1;
      bg2.regs.mosaic_y = 1;
      bg3.regs.mosaic_y = 1;
      bg4.regs.mosaic_y = 1;
    } else {
      if(!bg1.regs.mosaic || !regs.mosaic_countdown) bg1.regs.mosaic_y = vcounter();
      if(!bg2.regs.mosaic || !regs.mosaic_countdown) bg2.regs.mosaic_y = vcounter();
      if(!bg3.regs.mosaic || !regs.mosaic_countdown) bg3.regs.mosaic_y = vcounter();
      if(!bg4.regs.mosaic || !regs.mosaic_countdown) bg4.regs.mosaic_y = vcounter();
      if(!regs.mosaic_countdown) regs.mosaic_countdown = regs.mosaic_size + 1;
      regs.mosaic_countdown--;
    }

    if(vcounter() >= 1 && vcounter() <= 224) {
      screen.scanline();
      for(unsigned n = 0; n < 256; n++) {
        bg1.run();
        bg2.run();
        bg3.run();
        bg4.run();
        screen.run();
        add_clocks(4);
      }
    } else {
      add_clocks(1024);
    }

    add_clocks(lineclocks() - 88 - 1024);
  }
}

void sPPU::add_clocks(unsigned clocks) {
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    scheduler.addclocks_ppu(2);
    scheduler.sync_ppucpu();
  }
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

sPPU::sPPU() :
bg1(*this, Background::ID::BG1),
bg2(*this, Background::ID::BG2),
bg3(*this, Background::ID::BG3),
bg4(*this, Background::ID::BG4),
screen(*this) {
}

}
