#include "../base.h"

void PPU::get_scanline_info(scanline_info *info) {
  info->hires     = scanline_is_hires();
  info->interlace = cpu.interlace();
}

void PPU::enable_renderer(bool r) { status.render_output = r; }
bool PPU::renderer_enabled() { return status.render_output; }

void PPU::frame() {
  status.frame_executed = true;

static int32 fr = 0, fe = 0;
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

void PPU::power() {}

void PPU::reset() {
  memset(output, 0, 512 * 480 * sizeof(uint16));
}

PPU::PPU() {
  output = (uint16*)malloc(512 * 480 * sizeof(uint16));
  memset(output, 0, 512 * 480 * sizeof(uint16));

  status.render_output   = true;
  status.frames_updated  = false;
  status.frames_rendered = 0;
  status.frames_executed = 0;

  ppu1_version = 1;
  ppu2_version = 1;
}

PPU::~PPU() {
  safe_free(output);
}
