#include "../base.h"

PPU::PPU() {
  mmio = &mmio_unmapped;
  output = (uint16*)memalloc(512 * 478 * 2, "PPU::output");
  memset(output, 0, 512 * 478 * 2);
}

PPU::~PPU() {
  if(output)memfree(output, "PPU::output");
}
