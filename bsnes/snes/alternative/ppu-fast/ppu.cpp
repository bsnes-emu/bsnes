#include <snes.hpp>

#define PPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "debugger/debugger.cpp"
  PPUDebugger ppu;
#else
  PPU ppu;
#endif

#include "background.cpp"
#include "memory.cpp"
#include "mmio.cpp"
#include "screen.cpp"

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
    add_clocks(512);
    if(vcounter() < regs.height) render_scanline();
    add_clocks(lineclocks() - 512);
  }
}

void PPU::add_clocks(unsigned clocks) {
  tick(clocks);
  step(clocks);
  synchronize_cpu();
}

void PPU::scanline() {
  if(vcounter() == 0) frame();
  regs.width = !hires() ? 256 : 512;
  regs.height = !overscan() ? 225 : 240;
}

void PPU::frame() {
  system.frame();
}

void PPU::render_scanline() {
  screen_render();
}

void PPU::power() {
  for(unsigned i = 0; i < memory::vram.size(); i++) memory::vram[i] = 0x00;
  for(unsigned i = 0; i < memory::oam.size(); i++) memory::oam[i] = 0x00;
  for(unsigned i = 0; i < memory::cgram.size(); i++) memory::cgram[i] = 0x00;
  reset();
}

void PPU::reset() {
  create(Enter, system.cpu_frequency());
  PPUcounter::reset();
  memset(surface, 0, 512 * 512 * sizeof(uint16));

  mmio_reset();
}

PPU::PPU() {
  surface = new uint16[512 * 512];
  output = surface + 16 * 512;

  light_table = new uint16*[16];
  for(unsigned l = 0; l < 16; l++) {
    light_table[l] = new uint16[32768];
    for(unsigned r = 0; r < 32; r++) {
      for(unsigned g = 0; g < 32; g++) {
        for(unsigned b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          unsigned ar = (luma * r + 0.5);
          unsigned ag = (luma * g + 0.5);
          unsigned ab = (luma * b + 0.5);
          light_table[l][(r << 10) + (g << 5) + (b << 0)] = (ab << 10) + (ag << 5) + (ar << 0);
        }
      }
    }
  }
}

PPU::~PPU() {
  delete[] surface;
}

bool PPU::interlace() const { return false; }
bool PPU::overscan() const { return false; }
bool PPU::hires() const { return false; }

void PPUcounter::serialize(serializer &s) {
  s.integer(status.interlace);
  s.integer(status.field);
  s.integer(status.vcounter);
  s.integer(status.hcounter);

  s.array(history.field);
  s.array(history.vcounter);
  s.array(history.hcounter);
  s.integer(history.index);
}

void PPU::serialize(serializer &s) {
}

}
