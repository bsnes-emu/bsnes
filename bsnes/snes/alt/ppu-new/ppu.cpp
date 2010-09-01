#include <snes.hpp>

#define PPU_CPP
namespace SNES {

PPU ppu;

#include "mmio/mmio.cpp"
#include "cache/cache.cpp"
#include "background/background.cpp"
#include "sprite/sprite.cpp"
#include "window/window.cpp"
#include "screen/screen.cpp"
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
    if(vcounter() < display.height) {
      add_clocks(512);
      render_scanline();
      add_clocks(lineclocks() - 512);
    } else {
      add_clocks(lineclocks());
    }
  }
}

void PPU::add_clocks(unsigned clocks) {
  tick(clocks);
  step(clocks);
  synchronize_cpu();
}

void PPU::render_scanline() {
  if(regs.display_disable) return screen.render_black();
  screen.scanline();
  bg1.render();
  bg2.render();
  bg3.render();
  bg4.render();
  oam.render();
  screen.render();
}

void PPU::scanline() {
  if(vcounter() == 0) frame();
  display.width = !hires() ? 256 : 512;
  display.height = !overscan() ? 225 : 240;
}

void PPU::frame() {
  system.frame();
}

void PPU::power() {
  foreach(n, memory::vram) n = 0;
  foreach(n, memory::oam) n = 0;
  foreach(n, memory::cgram) n = 0;

  reset();
}

void PPU::reset() {
  create(Enter, system.cpu_frequency());
  PPUcounter::reset();
  memset(surface, 0, 512 * 512 * sizeof(uint16));

  mmio_reset();
}

PPU::PPU() :
cache(*this),
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
