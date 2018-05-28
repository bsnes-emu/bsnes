#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;
#include "io.cpp"
#include "line.cpp"
#include "background.cpp"
#include "object.cpp"
#include "window.cpp"
#include "serialization.cpp"
#include <sfc/ppu/counter/serialization.cpp>

PPU::PPU() {
  ppu1.version = 1;
  ppu2.version = 3;

  output = new uint32[512 * 512];
  output += 16 * 512;  //overscan offset

  tilecache[0] = new uint8[4096 * 8 * 8];
  tilecache[1] = new uint8[2048 * 8 * 8];
  tilecache[2] = new uint8[1024 * 8 * 8];

  for(uint y : range(240)) {
    lines[y].y = y;
    lines[y].outputLo = output + (y * 2 + 0) * 512;
    lines[y].outputHi = output + (y * 2 + 1) * 512;
  }
}

PPU::~PPU() {
  output -= 16 * 512;  //overscan offset
  delete[] output;

  delete[] tilecache[0];
  delete[] tilecache[1];
  delete[] tilecache[2];
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::step(uint clocks) -> void {
  tick(clocks);
  Thread::step(clocks);
  synchronize(cpu);
}

auto PPU::main() -> void {
  scanline();
  uint y = PPUcounter::vcounter();

  step(512);
  if(y >= 1 && y <= vdisp()) {
    memory::copy(&lines[y].cgram, &cgram, sizeof(cgram));
    memory::copy(&lines[y].io, &io, sizeof(io));
  }

  step(624);
  step(PPUcounter::lineclocks() - PPUcounter::hcounter());
}

auto PPU::scanline() -> void {
  if(PPUcounter::vcounter() == 0) {
    frame();
  }

  if(PPUcounter::vcounter() == 240) {
    const uint limit = vdisp();
    #pragma omp parallel for
    for(uint y = 1; y < limit; y++) {
      lines[y].render();
    }
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto PPU::frame() -> void {
}

auto PPU::refresh() -> void {
  auto output = this->output;
  if(!overscan()) output -= 14 * 512;
  auto pitch = 512;
  auto width = 512;
  auto height = 480;
  Emulator::video.refresh(output, pitch * sizeof(uint32), width, height);
}

auto PPU::load(Markup::Node node) -> bool {
  return true;
}

auto PPU::power(bool reset) -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill<uint32>(output, 512 * 480);

  function<auto (uint24, uint8) -> uint8> reader{&PPU::readIO, this};
  function<auto (uint24, uint8) -> void> writer{&PPU::writeIO, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");
}

}
