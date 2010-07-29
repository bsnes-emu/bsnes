#include <snes/snes.hpp>

#define PPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "ppu-debugger.cpp"
#endif

#include "serialization.cpp"

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
  create();
  ppu1_version = config.ppu1.version;
  ppu2_version = config.ppu2.version;
}

void PPU::reset() {
  create();
  PPUCounter::reset();
  memset(output, 0, 512 * 480 * sizeof(uint16));
}

PPU::PPU() {
  output = new uint16[512 * 480];

  status.frames_updated = false;
  status.frames_executed = 0;
}

PPU::~PPU() {
  delete[] output;
}

}
