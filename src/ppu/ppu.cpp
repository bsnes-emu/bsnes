#include "../base.h"

PPUOutput::PPUOutput() {
  buffer = (uint16*)memalloc(512 * 478 * 2, "PPUOutput::buffer");
  memset(buffer, 0, 512 * 478 * 2);
  frame_mode = NORMAL;
  for(int i=0;i<239;i++) {
    scanline_mode[i] = NORMAL;
  }
}

PPUOutput::~PPUOutput() {
  if(buffer)memfree(buffer, "PPUOutput::buffer");
}

PPU::PPU() {
  mmio = &mmio_unmapped;
  output = new PPUOutput();
}

PPU::~PPU() {
  if(output)delete(output);
}
