#include "../base.h"

void PPU::get_scanline_info(scanline_info *info) {
  info->hires     = scanline_is_hires();
  info->interlace = cpu->interlace();
}

void PPU::set_frameskip(int fs) {}
bool PPU::render_frame() { return true; }

PPU::PPU() {
  ppu1_version = 1;
  ppu2_version = 3;
  mmio = &mmio_unmapped;
}

PPU::~PPU() {}
