#include <snes.hpp>

#define PPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "debugger/debugger.cpp"
  PPUDebugger ppu;
#else
  PPU ppu;
#endif

#include "background/background.cpp"
#include "mmio/mmio.cpp"
#include "screen/screen.cpp"
#include "sprite/sprite.cpp"
#include "window/window.cpp"
#include "serialization.cpp"

void PPU::step(unsigned clocks) {
  clock += clocks;
}

void PPU::synchronize_cpu() {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.enter();
  }
}

void PPU::Enter() { ppu.enter(); }

void PPU::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    scanline();
    add_clocks(88);

    if(vcounter() <= (!regs.overscan ? 224 : 239)) {
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

      add_clocks(22);
      oam.tilefetch();
    } else {
      add_clocks(1024 + 22 + 136);
    }

    add_clocks(lineclocks() - 88 - 1024 - 22 - 136);
  }
}

void PPU::add_clocks(unsigned clocks) {
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    step(2);
    synchronize_cpu();
  }
}

void PPU::power() {
  ppu1_version = config.ppu1.version;
  ppu2_version = config.ppu2.version;

  memset(memory::vram.data(), 0x00, memory::vram.size());
  memset(memory::oam.data(), 0x00, memory::oam.size());
  memset(memory::cgram.data(), 0x00, memory::cgram.size());

  reset();
}

void PPU::reset() {
  create(Enter, system.cpu_frequency());
  PPUcounter::reset();
  memset(surface, 0, 512 * 512 * sizeof(uint16));

  mmio_reset();
  bg1.reset();
  bg2.reset();
  bg3.reset();
  bg4.reset();
  oam.reset();
  window.reset();
  screen.reset();

  frame();
}

void PPU::scanline() {
  if(vcounter() == 0) frame();
  bg1.scanline();
  bg2.scanline();
  bg3.scanline();
  bg4.scanline();
  oam.scanline();
  window.scanline();
  screen.scanline();
}

void PPU::frame() {
  system.frame();
  oam.frame();

  display.interlace = regs.interlace;
  display.overscan = regs.overscan;
}

PPU::PPU() :
bg1(*this, Background::ID::BG1),
bg2(*this, Background::ID::BG2),
bg3(*this, Background::ID::BG3),
bg4(*this, Background::ID::BG4),
oam(*this),
window(*this),
screen(*this) {
  surface = new uint16[512 * 512];
  output = surface + 16 * 512;
}

PPU::~PPU() {
  delete[] surface;
}

}
