#include "../../base.h"
#include "bppu_mmio.cpp"
#include "bppu_render.cpp"

void bPPU::run() {}

void bPPU::scanline() {
uint16 v = clock->vcounter();
  if(v > 0 && v < clock->visible_scanlines()) {
    if(clock->interlace() || regs.oam_halve == true) {
      output->frame_mode       |= PPUOutput::INTERLACE;
      output->scanline_mode[v] |= PPUOutput::INTERLACE;
    }
    if(regs.bg_mode == 5 || regs.bg_mode == 6) {
      output->frame_mode       |= PPUOutput::DOUBLEWIDTH;
      output->scanline_mode[v] |= PPUOutput::DOUBLEWIDTH;
    }
    render_line(v);
  }
}

void bPPU::frame() {
  snes->notify(SNES::RENDER_FRAME);
  output->frame_mode = PPUOutput::NORMAL;
  for(int i=0;i<239;i++) {
    output->scanline_mode[i] = PPUOutput::NORMAL;
  }
}

void bPPU::power() {
  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);
  reset();
}

void bPPU::reset() {
  memset(output->buffer, 0, 512 * 478 * 2);
  frame();

//$2100
  regs.display_disabled   = 0;
  regs.display_brightness = 0;

//$2101
  regs.oam_basesize   = 0;
  regs.oam_nameselect = 0;
  regs.oam_tdaddr     = 0x0000;

//$2102-$2103
  regs.oam_addrl     = 0x00;
  regs.oam_addrh     = 0x00;
  regs.oam_addr      = 0x0000;
  regs.oam_latchdata = 0x00;

//$2105
  regs.bg_tilesize[BG1] = 0;
  regs.bg_tilesize[BG2] = 0;
  regs.bg_tilesize[BG3] = 0;
  regs.bg_tilesize[BG4] = 0;
  regs.bg3_priority     = 0;
  regs.bg_mode          = 0;

//$2106
  regs.mosaic_size         = 0;
  regs.mosaic_enabled[BG1] = false;
  regs.mosaic_enabled[BG2] = false;
  regs.mosaic_enabled[BG3] = false;
  regs.mosaic_enabled[BG4] = false;

//$2107-$210a
  regs.bg_scaddr[BG1] = 0x0000;
  regs.bg_scaddr[BG2] = 0x0000;
  regs.bg_scaddr[BG3] = 0x0000;
  regs.bg_scaddr[BG4] = 0x0000;
  regs.bg_scsize[BG1] = SC_32x32;
  regs.bg_scsize[BG2] = SC_32x32;
  regs.bg_scsize[BG3] = SC_32x32;
  regs.bg_scsize[BG4] = SC_32x32;

//$210b-$210c
  regs.bg_tdaddr[BG1] = 0x0000;
  regs.bg_tdaddr[BG2] = 0x0000;
  regs.bg_tdaddr[BG3] = 0x0000;
  regs.bg_tdaddr[BG4] = 0x0000;

//$210d-$2114
  regs.bg_hofs[BG1] = regs.bg_vofs[BG1] = 0x0000;
  regs.bg_hofs[BG2] = regs.bg_vofs[BG2] = 0x0000;
  regs.bg_hofs[BG3] = regs.bg_vofs[BG3] = 0x0000;
  regs.bg_hofs[BG4] = regs.bg_vofs[BG4] = 0x0000;

//$2115
  regs.vram_incmode = 1;
  regs.vram_mapping = 0;
  regs.vram_incsize = 1;

//$2116-$2117
  regs.vram_addr = 0x0000;

//$211a
  regs.mode7_repeat = 0;
  regs.mode7_vflip  = false;
  regs.mode7_hflip  = false;

//$211b-$2120
  regs.m7a = 0x0000;
  regs.m7b = 0x0000;
  regs.m7c = 0x0000;
  regs.m7d = 0x0000;
  regs.m7x = 0x0000;
  regs.m7y = 0x0000;

//$2121
  regs.cgram_addr = 0x0000;

//$2123-$2125
  regs.bg_window1_enabled[BG1] = false;
  regs.bg_window1_enabled[BG2] = false;
  regs.bg_window1_enabled[BG3] = false;
  regs.bg_window1_enabled[BG4] = false;
  regs.bg_window1_enabled[OAM] = false;
  regs.bg_window1_invert [BG1] = false;
  regs.bg_window1_invert [BG2] = false;
  regs.bg_window1_invert [BG3] = false;
  regs.bg_window1_invert [BG4] = false;
  regs.bg_window1_invert [OAM] = false;
  regs.bg_window2_enabled[BG1] = false;
  regs.bg_window2_enabled[BG2] = false;
  regs.bg_window2_enabled[BG3] = false;
  regs.bg_window2_enabled[BG4] = false;
  regs.bg_window2_enabled[OAM] = false;
  regs.bg_window2_invert [BG1] = false;
  regs.bg_window2_invert [BG2] = false;
  regs.bg_window2_invert [BG3] = false;
  regs.bg_window2_invert [BG4] = false;
  regs.bg_window2_invert [OAM] = false;
  regs.color_window1_enabled   = false;
  regs.color_window1_invert    = false;
  regs.color_window2_enabled   = false;
  regs.color_window2_invert    = false;

//$2126-$2129
  regs.window1_left  = 0;
  regs.window1_right = 0;
  regs.window2_left  = 0;
  regs.window2_right = 0;

//$212a-$212b
  regs.bg_window_mask[BG1] = 0;
  regs.bg_window_mask[BG2] = 0;
  regs.bg_window_mask[BG3] = 0;
  regs.bg_window_mask[BG4] = 0;
  regs.bg_window_mask[OAM] = 0;
  regs.color_window_mask   = 0;

//$212c-$212d
  regs.bg_enabled[BG1]    = false;
  regs.bg_enabled[BG2]    = false;
  regs.bg_enabled[BG3]    = false;
  regs.bg_enabled[BG4]    = false;
  regs.bg_enabled[OAM]    = false;
  regs.bgsub_enabled[BG1] = false;
  regs.bgsub_enabled[BG2] = false;
  regs.bgsub_enabled[BG3] = false;
  regs.bgsub_enabled[BG4] = false;
  regs.bgsub_enabled[OAM] = false;

//$212e-$212f
  regs.bg_window_enabled[BG1]    = false;
  regs.bg_window_enabled[BG2]    = false;
  regs.bg_window_enabled[BG3]    = false;
  regs.bg_window_enabled[BG4]    = false;
  regs.bg_window_enabled[OAM]    = false;
  regs.bgsub_window_enabled[BG1] = false;
  regs.bgsub_window_enabled[BG2] = false;
  regs.bgsub_window_enabled[BG3] = false;
  regs.bgsub_window_enabled[BG4] = false;
  regs.bgsub_window_enabled[OAM] = false;

//$2130
  regs.color_mask    = 0;
  regs.colorsub_mask = 0;
  regs.addsub_mode   = 0;

//$2131
  regs.color_mode             = 0;
  regs.color_halve            = false;
  regs.bg_color_enabled[BACK] = false;
  regs.bg_color_enabled[OAM]  = false;
  regs.bg_color_enabled[BG4]  = false;
  regs.bg_color_enabled[BG3]  = false;
  regs.bg_color_enabled[BG2]  = false;
  regs.bg_color_enabled[BG1]  = false;

//$2132
  regs.color_r = 0x00;
  regs.color_g = 0x00;
  regs.color_b = 0x00;

//$2133
  regs.mode7_extbg = false;
  regs.overscan    = false;
  regs.scanlines   = 224;
  regs.oam_halve   = false;
  regs.interlace   = false;

//$2137
  regs.hcounter         = 0;
  regs.vcounter         = 0;
  regs.latch_hcounter   = 0;
  regs.latch_vcounter   = 0;
  regs.counters_latched = false;

//$2139-$213a
  regs.vram_readbuffer = 0x0000;

  clear_tiledata_cache();
}

uint8 bPPU::vram_read(uint16 addr) {
uint8 r;
  r = vram[addr];
  snes->notify(SNES::VRAM_READ, addr, r);
  return r;
}

void bPPU::vram_write(uint16 addr, uint8 value) {
  vram[addr] = value;
  snes->notify(SNES::VRAM_WRITE, addr, value);
}

uint8 bPPU::oam_read(uint16 addr) {
uint8 r;
  if(addr >= 0x0200)addr = 0x0200 | (addr & 31);
  r = oam[addr];
  snes->notify(SNES::OAM_READ, addr, r);
  return r;
}

void bPPU::oam_write(uint16 addr, uint8 value) {
  if(addr >= 0x0200)addr = 0x0200 | (addr & 31);
  oam[addr] = value;
  snes->notify(SNES::OAM_WRITE, addr, value);
}

uint8 bPPU::cgram_read(uint16 addr) {
uint8 r;
  r = cgram[addr & 511];
  snes->notify(SNES::CGRAM_READ, addr, r);
  return r;
}

void bPPU::cgram_write(uint16 addr, uint8 value) {
  cgram[addr & 511] = value;
  snes->notify(SNES::CGRAM_WRITE, addr, value);
}

bPPU::bPPU() {
  mmio = new bPPUMMIO(this);

  vram  = (uint8*)memalloc(65536, "bPPU::vram");
  oam   = (uint8*)memalloc(  544, "bPPU::oam");
  cgram = (uint8*)memalloc(  512, "bPPU::cgram");
  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);

  init_tiledata_cache();

int i, l;
byte r, g, b;
double m;
uint16 *ptr;
  for(l=0;l<16;l++) {
    mosaic_table[l] = (uint16*)memalloc(4096 * 2, "bPPU::mosaic_table[%2d]", l);
    for(i=0;i<4096;i++) {
      mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
    }
  }

  light_table = (uint16*)memalloc(16 * 65536 * 2, "bPPU::light_table");
  ptr = (word*)light_table;
  for(l=0;l<16;l++) {
    m = (double)l / 15.0;
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      if(l == 0) { r = g = b = 0; }
      else if(l == 15);
      else {
        r = (double)r * m;
        g = (double)g * m;
        b = (double)b * m;
      }
      *ptr++ = (r) | (g << 5) | (b << 10);
    }
  }
}

bPPU::~bPPU() {
  delete(mmio);

  if(vram) memfree(vram,  "bPPU::vram");
  if(oam)  memfree(oam,   "bPPU::oam");
  if(cgram)memfree(cgram, "bPPU::cgram");

  for(int i=0;i<16;i++) {
    if(mosaic_table[i])memfree(mosaic_table[i], "bPPU::mosaic_table[%2d]", i);
  }
  if(light_table)memfree(light_table, "bPPU::light_table");
}

bPPUMMIO::bPPUMMIO(bPPU *_ppu) {
  ppu = _ppu;
}
