#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;

#include "io.cpp"
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
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    clock += 2;
    synchronizeCPU();
  }
}

auto PPU::synchronizeCPU() -> void {
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  scanline();
  step(28);
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
      step(2);

      bg1.run(0);
      bg2.run(0);
      bg3.run(0);
      bg4.run(0);
      if(pixel >= 0) {
        obj.run();
        window.run();
        screen.run();
      }
      step(2);
    }

    step(14);
    obj.tilefetch();
  } else {
    step(1052 + 14 + 136);
  }

  step(lineclocks() - 28 - 1052 - 14 - 136);
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
}

auto PPU::reset() -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill(output, 512 * 480 * sizeof(uint32));

  function<auto (uint24, uint8) -> uint8> reader{&PPU::readIO, this};
  function<auto (uint24, uint8) -> void> writer{&PPU::writeIO, this};
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
  io.displayDisable = true;
  io.displayBrightness = 0;

  //$2102  OAMADDL
  //$2103  OAMADDH
  io.oamBaseAddress = random(0x0000);
  io.oamAddress = random(0x0000);
  io.oamPriority = random(false);

  //$2105  BGMODE
  io.bgPriority = false;
  io.bgMode = 0;

  //$210d  BG1HOFS
  io.hoffsetMode7 = random(0x0000);

  //$210e  BG1VOFS
  io.voffsetMode7 = random(0x0000);

  //$2115  VMAIN
  io.vramIncrementMode = random(1);
  io.vramMapping = random(0);
  io.vramIncrementSize = 1;

  //$2116  VMADDL
  //$2117  VMADDH
  io.vramAddress = random(0x0000);

  //$211a  M7SEL
  io.repeatMode7 = random(0);
  io.vflipMode7 = random(false);
  io.hflipMode7 = random(false);

  //$211b  M7A
  io.m7a = random(0x0000);

  //$211c  M7B
  io.m7b = random(0x0000);

  //$211d  M7C
  io.m7c = random(0x0000);

  //$211e  M7D
  io.m7d = random(0x0000);

  //$211f  M7X
  io.m7x = random(0x0000);

  //$2120  M7Y
  io.m7y = random(0x0000);

  //$2121  CGADD
  io.cgramAddress = random(0x00);
  io.cgramAddressLatch = random(0);

  //$2133  SETINI
  io.extbg = random(false);
  io.pseudoHires = random(false);
  io.overscan = false;
  io.interlace = false;

  //$213c  OPHCT
  io.hcounter = 0;

  //$213d  OPVCT
  io.vcounter = 0;

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
  display.interlace = io.interlace;
  display.overscan = io.overscan;
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
