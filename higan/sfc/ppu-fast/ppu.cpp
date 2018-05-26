#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;
#include "io.cpp"
#include "object.cpp"
#include "line.cpp"
#include "serialization.cpp"
#include <sfc/ppu/counter/serialization.cpp>

PPU::PPU() {
  output = new uint32[512 * 512];
  output += 16 * 512;  //overscan offset
  for(uint y : range(240)) {
    lines[y].y = y;
    lines[y].outputLo = output + (y * 2 + 0) * 512;
    lines[y].outputHi = output + (y * 2 + 1) * 512;
  }
}

PPU::~PPU() {
  output -= 16 * 512;  //overscan offset
  delete[] output;
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
  uint y = vcounter();

  step(512);
  if(y >= 1 && y <= vdisp()) {
    memory::copy(&lines[y].cgram, &cgram, sizeof(cgram));
    memory::copy(&lines[y].io, &io, sizeof(io));
  }

  step(624);

  step(lineclocks() - 512 - 624);
}

auto PPU::scanline() -> void {
  if(vcounter() == 0) {
    frame();
  }

  if(vcounter() == 241) {
    #pragma omp parallel for
    for(uint y = 1; y < vdisp(); y++) {
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
  memory::fill(output, 512 * 480 * sizeof(uint32));

  function<auto (uint24, uint8) -> uint8> reader{&PPU::readIO, this};
  function<auto (uint24, uint8) -> void> writer{&PPU::writeIO, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");
}

}
