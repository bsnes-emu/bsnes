#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;

#include "memory.cpp"
#include "mmio.cpp"
#include "background/background.cpp"
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
  output = new uint32[512 * 512];
  output += 16 * 512;  //overscan offset
}

PPU::~PPU() {
  output -= 16 * 512;
  delete[] output;
}

auto PPU::step(uint clocks) -> void {
  clock += clocks;
}

auto PPU::synchronizeCPU() -> void {
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  scanline();
  addClocks(28);
  bg1.begin();
  bg2.begin();
  bg3.begin();
  bg4.begin();

  if(vcounter() <= 239) {
    for(int pixel = -7; pixel <= 255; pixel++) {
      bg1.run(1);
      bg2.run(1);
      bg3.run(1);
      bg4.run(1);
      addClocks(2);

      bg1.run(0);
      bg2.run(0);
      bg3.run(0);
      bg4.run(0);
      if(pixel >= 0) {
        sprite.run();
        window.run();
        screen.run();
      }
      addClocks(2);
    }

    addClocks(14);
    sprite.tilefetch();
  } else {
    addClocks(1052 + 14 + 136);
  }

  addClocks(lineclocks() - 28 - 1052 - 14 - 136);
}

auto PPU::addClocks(uint clocks) -> void {
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    step(2);
    synchronizeCPU();
  }
}

auto PPU::power() -> void {
  for(auto& n : vram) n = random(0x00);
  for(auto& n : oam) n = random(0x00);
  for(auto& n : cgram) n = random(0x00);
}

auto PPU::reset() -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill(output, 512 * 480 * sizeof(uint32));

  function<auto (uint24, uint8) -> uint8> reader{&PPU::read, this};
  function<auto (uint24, uint8) -> void> writer{&PPU::write, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");

  regs.ppu1_mdr = random(0xff);
  regs.ppu2_mdr = random(0xff);

  regs.vram_readbuffer = random(0x0000);
  regs.oam_latchdata = random(0x00);
  regs.cgram_latchdata = random(0x00);
  regs.bgofs_latchdata = random(0x00);
  regs.mode7_latchdata = random(0x00);
  regs.counters_latched = false;
  regs.latch_hcounter = 0;
  regs.latch_vcounter = 0;

  regs.oam_iaddr = 0x0000;
  regs.cgram_iaddr = 0x00;

  //$2100  INIDISP
  regs.display_disable = true;
  regs.display_brightness = 0;

  //$2102  OAMADDL
  //$2103  OAMADDH
  regs.oam_baseaddr = random(0x0000);
  regs.oam_addr = random(0x0000);
  regs.oam_priority = random(false);

  //$2105  BGMODE
  regs.bg3_priority = false;
  regs.bgmode = 0;

  //$210d  BG1HOFS
  regs.mode7_hoffset = random(0x0000);

  //$210e  BG1VOFS
  regs.mode7_voffset = random(0x0000);

  //$2115  VMAIN
  regs.vram_incmode = random(1);
  regs.vram_mapping = random(0);
  regs.vram_incsize = 1;

  //$2116  VMADDL
  //$2117  VMADDH
  regs.vram_addr = random(0x0000);

  //$211a  M7SEL
  regs.mode7_repeat = random(0);
  regs.mode7_vflip = random(false);
  regs.mode7_hflip = random(false);

  //$211b  M7A
  regs.m7a = random(0x0000);

  //$211c  M7B
  regs.m7b = random(0x0000);

  //$211d  M7C
  regs.m7c = random(0x0000);

  //$211e  M7D
  regs.m7d = random(0x0000);

  //$211f  M7X
  regs.m7x = random(0x0000);

  //$2120  M7Y
  regs.m7y = random(0x0000);

  //$2121  CGADD
  regs.cgram_addr = random(0x0000);

  //$2133  SETINI
  regs.mode7_extbg = random(false);
  regs.pseudo_hires = random(false);
  regs.overscan = false;
  regs.interlace = false;

  //$213c  OPHCT
  regs.hcounter = 0;

  //$213d  OPVCT
  regs.vcounter = 0;

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

  if(vcounter() == 241) {
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto PPU::frame() -> void {
  sprite.frame();

  display.interlace = regs.interlace;
  display.overscan = regs.overscan;
}

auto PPU::refresh() -> void {
  auto output = this->output;
  if(!overscan()) output -= 14 * 512;
  auto pitch = 512;
  auto width = 512;
  auto height = 480;
  Emulator::video.refresh(output, pitch * sizeof(uint32), width, height);
}

}
