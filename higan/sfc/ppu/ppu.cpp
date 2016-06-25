#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;

#include "memory.cpp"
#include "mmio.cpp"
#include "background/background.cpp"
#include "object/object.cpp"
#include "window/window.cpp"
#include "screen/screen.cpp"
#include "serialization.cpp"

PPU::PPU() :
bg1(Background::ID::BG1),
bg2(Background::ID::BG2),
bg3(Background::ID::BG3),
bg4(Background::ID::BG4) {
  ppu1.version = 1;  //allowed values: 1
  ppu2.version = 3;  //allowed values: 1, 2, 3

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
        obj.run();
        window.run();
        screen.run();
      }
      addClocks(2);
    }

    addClocks(14);
    obj.tilefetch();
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

auto PPU::load(Markup::Node node) -> bool {
  ppu1.version = max(1, min(1, node["ppu1/version"].natural()));
  ppu2.version = max(1, min(3, node["ppu2/version"].natural()));
  ppu.vram.mask = node["ppu1/ram/size"].natural() - 1;
  if(ppu.vram.mask != 0xffff) ppu.vram.mask = 0x7fff;
  return true;
}

auto PPU::power() -> void {
  for(auto& n : vram.data) n = random(0x0000);
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

  ppu1.mdr = random(0xff);
  ppu2.mdr = random(0xff);

  latch.vram = random(0x0000);
  latch.oam = random(0x00);
  latch.cgram = random(0x00);
  latch.bgofs = random(0x00);
  latch.mode7 = random(0x00);
  latch.counters = false;
  latch.hcounter = 0;
  latch.vcounter = 0;

  latch.oamAddress = 0x0000;
  latch.cgramAddress = 0x00;

  //$2100  INIDISP
  r.displayDisable = true;
  r.displayBrightness = 0;

  //$2102  OAMADDL
  //$2103  OAMADDH
  r.oamBaseAddress = random(0x0000);
  r.oamAddress = random(0x0000);
  r.oamPriority = random(false);

  //$2105  BGMODE
  r.bgPriority = false;
  r.bgMode = 0;

  //$210d  BG1HOFS
  r.hoffsetMode7 = random(0x0000);

  //$210e  BG1VOFS
  r.voffsetMode7 = random(0x0000);

  //$2115  VMAIN
  r.vramIncrementMode = random(1);
  r.vramMapping = random(0);
  r.vramIncrementSize = 1;

  //$2116  VMADDL
  //$2117  VMADDH
  r.vramAddress = random(0x0000);

  //$211a  M7SEL
  r.repeatMode7 = random(0);
  r.vflipMode7 = random(false);
  r.hflipMode7 = random(false);

  //$211b  M7A
  r.m7a = random(0x0000);

  //$211c  M7B
  r.m7b = random(0x0000);

  //$211d  M7C
  r.m7c = random(0x0000);

  //$211e  M7D
  r.m7d = random(0x0000);

  //$211f  M7X
  r.m7x = random(0x0000);

  //$2120  M7Y
  r.m7y = random(0x0000);

  //$2121  CGADD
  r.cgramAddress = random(0x0000);

  //$2133  SETINI
  r.extbg = random(false);
  r.pseudoHires = random(false);
  r.overscan = false;
  r.interlace = false;

  //$213c  OPHCT
  r.hcounter = 0;

  //$213d  OPVCT
  r.vcounter = 0;

  bg1.reset();
  bg2.reset();
  bg3.reset();
  bg4.reset();
  obj.reset();
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
  obj.scanline();
  window.scanline();
  screen.scanline();

  if(vcounter() == 241) {
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto PPU::frame() -> void {
  obj.frame();
  display.interlace = r.interlace;
  display.overscan = r.overscan;
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
