#include "../base.h"

PPUOutput::PPUOutput() {
  buffer = (uint16*)memalloc(512 * 478 * 2, "PPUOutput::buffer");
  memset(buffer, 0, 512 * 478 * 2);
  hires     = false;
  interlace = false;
  for(int i=0;i<239;i++) {
    line[i].hires     = false;
    line[i].interlace = false;
  }
}

PPUOutput::~PPUOutput() {
  if(buffer)memfree(buffer, "PPUOutput::buffer");
}

void PPU::set_frameskip(int fs) {}

PPU::PPU() {
  mmio = &mmio_unmapped;
  output = new PPUOutput();
}

PPU::~PPU() {
  if(output)delete(output);
}
