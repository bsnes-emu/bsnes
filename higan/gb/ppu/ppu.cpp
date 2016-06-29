#include <gb/gb.hpp>

namespace GameBoy {

PPU ppu;
#include "io.cpp"
#include "dmg.cpp"
#include "cgb.cpp"
#include "serialization.cpp"

auto PPU::enabled() const -> bool { return status.displayEnable; }

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  status.lx = 0;
  interface->lcdScanline();  //Super Game Boy notification

  if(status.ly <= 143) {
    mode(2);
    scanline();
    step(92);

    mode(3);
    for(auto n : range(160)) {
      run();
      step(1);
    }

    mode(0);
    if(enabled()) cpu.hblank();
    step(204);
  } else {
    mode(1);
    step(456);
  }

  status.ly++;

  if(status.ly == 144) {
    if(enabled()) cpu.raise(CPU::Interrupt::Vblank);
    scheduler.exit(Scheduler::Event::Frame);
  }

  if(status.ly == 154) {
    status.ly = 0;
  }
}

auto PPU::mode(uint mode) -> void {
  status.mode = mode;
}

auto PPU::stat() -> void {
  bool irq = status.irq;

  status.irq  = status.interruptHblank && status.mode == 0;
  status.irq |= status.interruptVblank && status.mode == 1;
  status.irq |= status.interruptOAM    && status.mode == 2;
  status.irq |= status.interruptLYC    && coincidence();

  if(!irq && status.irq) cpu.raise(CPU::Interrupt::Stat);
}

auto PPU::coincidence() -> bool {
  uint ly = status.ly;
  if(ly == 153 && status.lx >= 92) ly = 0;  //LYC=0 triggers early during LY=153
  return ly == status.lyc;
}

auto PPU::refresh() -> void {
  if(!system.sgb()) Emulator::video.refresh(screen, 160 * sizeof(uint32), 160, 144);
}

auto PPU::step(uint clocks) -> void {
  while(clocks--) {
    stat();
    if(status.dmaActive) {
      uint hi = status.dmaClock++;
      uint lo = hi & (cpu.status.speedDouble ? 1 : 3);
      hi >>= cpu.status.speedDouble ? 1 : 2;
      if(lo == 0) {
        if(hi == 0) {
          //warm-up
        } else if(hi == 161) {
          //cool-down; disable
          status.dmaActive = false;
        } else {
          oam[hi - 1] = bus.read(status.dmaBank << 8 | hi - 1);
        }
      }
    }

    status.lx++;
    clock += cpu.frequency;
    if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
  }
}

auto PPU::hflip(uint data) const -> uint {
  return (data & 0x8080) >> 7 | (data & 0x4040) >> 5
       | (data & 0x2020) >> 3 | (data & 0x1010) >> 1
       | (data & 0x0808) << 1 | (data & 0x0404) << 3
       | (data & 0x0202) << 5 | (data & 0x0101) << 7;
}

auto PPU::power() -> void {
  create(Enter, 4 * 1024 * 1024);

  if(system.cgb()) {
    scanline = {&PPU::scanlineCGB, this};
    run = {&PPU::runCGB, this};
  } else {
    scanline = {&PPU::scanlineDMG, this};
    run = {&PPU::runDMG, this};
  }

  for(uint n = 0x8000; n <= 0x9fff; n++) bus.mmio[n] = this;  //VRAM
  for(uint n = 0xfe00; n <= 0xfe9f; n++) bus.mmio[n] = this;  //OAM

  bus.mmio[0xff40] = this;  //LCDC
  bus.mmio[0xff41] = this;  //STAT
  bus.mmio[0xff42] = this;  //SCY
  bus.mmio[0xff43] = this;  //SCX
  bus.mmio[0xff44] = this;  //LY
  bus.mmio[0xff45] = this;  //LYC
  bus.mmio[0xff46] = this;  //DMA
  bus.mmio[0xff47] = this;  //BGP
  bus.mmio[0xff48] = this;  //OBP0
  bus.mmio[0xff49] = this;  //OBP1
  bus.mmio[0xff4a] = this;  //WY
  bus.mmio[0xff4b] = this;  //WX

  if(system.cgb()) {
  bus.mmio[0xff4f] = this;  //VBK
  bus.mmio[0xff68] = this;  //BGPI
  bus.mmio[0xff69] = this;  //BGPD
  bus.mmio[0xff6a] = this;  //OBPI
  bus.mmio[0xff6b] = this;  //OBPD
  }

  for(auto& n : vram) n = 0x00;
  for(auto& n : oam) n = 0x00;
  for(auto& n : bgp) n = 0x00;
  for(auto& n : obp[0]) n = 3;
  for(auto& n : obp[1]) n = 3;
  for(auto& n : bgpd) n = 0x0000;
  for(auto& n : obpd) n = 0x0000;

  memory::fill(&status, sizeof(Status));

  for(auto& n : screen) n = 0;

  bg.color = 0;
  bg.palette = 0;
  bg.priority = 0;

  ob.color = 0;
  ob.palette = 0;
  ob.priority = 0;

  for(auto& s : sprite) {
    s.x = 0;
    s.y = 0;
    s.tile = 0;
    s.attr = 0;
    s.data = 0;
  }
  sprites = 0;

  background.attr = 0;
  background.data = 0;

  window.attr = 0;
  window.data = 0;
}

}
