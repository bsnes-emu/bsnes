#include <snes/snes.hpp>

#define PPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "ppu-debugger.cpp"
#endif

#include "serialization.cpp"

void PPU::Enter() { ppu.enter(); }

void PPU::frame() {
  static signed framecount = 0;
  static time_t prev, curr;
  framecount++;

  time(&curr);
  if(curr != prev) {
    status.frames_updated = true;
    status.frames_executed = framecount;
    framecount = 0;
    prev = curr;
  }
}

void PPU::power() {
  create(PPU::Enter, system.cpu_frequency());

  ppu1_version = config.ppu1.version;
  ppu2_version = config.ppu2.version;
}

void PPU::reset() {
  create(PPU::Enter, system.cpu_frequency());

  PPUCounter::reset();
  memset(surface, 0, 512 * 512 * sizeof(uint16));
}

PPU::PPU() {
  surface = new uint16[512 * 512];
  output = surface + 16 * 512;

  status.frames_updated = false;
  status.frames_executed = 0;
}

PPU::~PPU() {
  delete[] surface;
}

}
