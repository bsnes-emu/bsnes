#include <snes/snes.hpp>

#define SPPU_CPP
namespace SNES {

#include "background/background.cpp"
#include "mmio/mmio.cpp"
#include "screen/screen.cpp"
#include "sprite/sprite.cpp"
#include "window/window.cpp"

#if defined(DEBUGGER)
  #include "debugger/debugger.cpp"
  sPPUDebugger ppu;
#else
  sPPU ppu;
#endif

void sPPU::enter() {
  while(true) {
    scanline();

    add_clocks(88);

    if(vcounter() >= 1 && vcounter() <= (!regs.overscan ? 224 : 239)) {
      for(unsigned n = 0; n < 256; n++) {
        bg1.run();
        bg2.run();
        bg3.run();
        bg4.run();
        add_clocks(2);

        bg1.run();
        bg2.run();
        bg3.run();
        bg4.run();
        oam.run();
        window.run();
        screen.run();
        add_clocks(2);
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
  memset(memory::vram.data(), 0x00, memory::vram.size());
  memset(memory::oam.data(), 0x00, memory::oam.size());
  memset(memory::cgram.data(), 0x00, memory::cgram.size());

  reset();
}

void sPPU::reset() {
  PPU::reset();
  PPU::frame();

  memset(output, 0x00, 512 * 480 * sizeof(uint16));
  mmio_reset();
  bg1.reset();
  bg2.reset();
  bg3.reset();
  bg4.reset();
  oam.reset();
  window.reset();
  screen.reset();
}

void sPPU::scanline() {
  if(vcounter() == 0) frame();
  bg1.scanline();
  bg2.scanline();
  bg3.scanline();
  bg4.scanline();
  oam.scanline();
  window.scanline();
  screen.scanline();
}

void sPPU::frame() {
  PPU::frame();
  system.frame();
  oam.frame();

  display.interlace = regs.interlace;
  display.overscan = regs.overscan;
}

sPPU::sPPU() :
bg1(*this, Background::ID::BG1),
bg2(*this, Background::ID::BG2),
bg3(*this, Background::ID::BG3),
bg4(*this, Background::ID::BG4),
oam(*this),
window(*this),
screen(*this) {
}

}
