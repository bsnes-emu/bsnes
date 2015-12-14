#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;

#include "background/background.cpp"
#include "mmio/mmio.cpp"
#include "screen/screen.cpp"
#include "sprite/sprite.cpp"
#include "window/window.cpp"
#include "serialization.cpp"

PPU::PPU() :
bg1(*this, Background::ID::BG1),
bg2(*this, Background::ID::BG2),
bg3(*this, Background::ID::BG3),
bg4(*this, Background::ID::BG4),
sprite(*this),
window(*this),
screen(*this) {
  surface = new uint32[512 * 512];
  output = surface + 16 * 512;
}

PPU::~PPU() {
  delete[] surface;
}

auto PPU::step(uint clocks) -> void {
  clock += clocks;
}

auto PPU::synchronizeCPU() -> void {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.enter();
  }
}

auto PPU::Enter() -> void { ppu.enter(); }

auto PPU::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    scanline();
    add_clocks(28);
    bg1.begin();
    bg2.begin();
    bg3.begin();
    bg4.begin();

    if(vcounter() <= 239) {
      for(signed pixel = -7; pixel <= 255; pixel++) {
        bg1.run(1);
        bg2.run(1);
        bg3.run(1);
        bg4.run(1);
        add_clocks(2);

        bg1.run(0);
        bg2.run(0);
        bg3.run(0);
        bg4.run(0);
        if(pixel >= 0) {
          sprite.run();
          window.run();
          screen.run();
        }
        add_clocks(2);
      }

      add_clocks(14);
      sprite.tilefetch();
    } else {
      add_clocks(1052 + 14 + 136);
    }

    add_clocks(lineclocks() - 28 - 1052 - 14 - 136);
  }
}

auto PPU::add_clocks(uint clocks) -> void {
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    step(2);
    synchronizeCPU();
  }
}

auto PPU::enable() -> void {
  function<auto (uint, uint8) -> uint8> reader{&PPU::mmio_read, (PPU*)&ppu};
  function<auto (uint, uint8) -> void> writer{&PPU::mmio_write, (PPU*)&ppu};

  bus.map(reader, writer, 0x00, 0x3f, 0x2100, 0x213f);
  bus.map(reader, writer, 0x80, 0xbf, 0x2100, 0x213f);
}

auto PPU::power() -> void {
  for(auto& n : vram) n = random(0x00);
  for(auto& n : oam) n = random(0x00);
  for(auto& n : cgram) n = random(0x00);
}

auto PPU::reset() -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill(surface, 512 * 512 * sizeof(uint32));

  mmio_reset();
  bg1.reset();
  bg2.reset();
  bg3.reset();
  bg4.reset();
  sprite.reset();
  window.reset();
  screen.reset();

  frame();
}

auto PPU::scanline() -> void {
  if(vcounter() == 0) {
    frame();
    bg1.frame();
    bg2.frame();
    bg3.frame();
    bg4.frame();
  }

  bg1.scanline();
  bg2.scanline();
  bg3.scanline();
  bg4.scanline();
  sprite.scanline();
  window.scanline();
  screen.scanline();
}

auto PPU::frame() -> void {
  system.frame();
  sprite.frame();

  display.interlace = regs.interlace;
  display.overscan = regs.overscan;
}

}
