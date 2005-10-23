#include "../base.h"

void PPU::get_scanline_info(scanline_info *info) {
  info->hires     = scanline_is_hires();
  info->interlace = cpu->interlace();
}

void PPU::enable_renderer(bool r) { status.render_output = r; }
bool PPU::renderer_enabled() { return status.render_output; }

void PPU::frame() {
static fr = 0, fe = 0;
static time_t prev, curr;
  fe++;
  if(status.render_output)fr++;

  time(&curr);
  if(curr != prev) {
    status.frames_updated  = true;
    status.frames_rendered = fr;
    status.frames_executed = fe;
    fr = fe = 0;
  }
  prev = curr;
}

PPU::PPU() {
  status.render_output   = true;
  status.frames_updated  = false;
  status.frames_rendered = 0;
  status.frames_executed = 0;

  ppu1_version = 1;
  ppu2_version = 1;
  mmio = &mmio_unmapped;
}

PPU::~PPU() {}
