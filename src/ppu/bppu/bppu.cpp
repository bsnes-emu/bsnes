#include "../../base.h"
#define BPPU_CPP

#include "bppu_mmio.cpp"
#include "bppu_render.cpp"

void bPPU::scanline() {
  line.y = cpu.vcounter();

  if(line.y == 0) {
    //RTO flag reset
    regs.time_over  = false;
    regs.range_over = false;
  }

  if(line.y == 1) {
    //mosaic reset
    for(int bg = BG1; bg <= BG4; bg++) regs.bg_y[bg] = 1;
    regs.mosaic_countdown = regs.mosaic_size + 1;
    regs.mosaic_countdown--;
  } else {
    for(int bg = BG1; bg <= BG4; bg++) {
      if(!regs.mosaic_enabled[bg] || !regs.mosaic_countdown) regs.bg_y[bg] = line.y;
    }
    if(!regs.mosaic_countdown) regs.mosaic_countdown = regs.mosaic_size + 1;
    regs.mosaic_countdown--;
  }

  //note: this should actually occur at V=225,HC=10.
  //this is a limitation of the scanline-based renderer.
  if(line.y == (!overscan() ? 225 : 240)) {
    if(regs.display_disabled == false) {
      //OAM address reset
      regs.oam_addr = regs.oam_baseaddr << 1;
      regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;
    }
  }
}

void bPPU::render_scanline() {
  #ifdef FAST_FRAMESKIP
  //bypass RTO status flag calculations
  if(status.render_output == false) return;
  #endif

  if(line.y >= 0 && line.y < (!overscan() ? 225 : 240)) {
    if(config::ppu.hack.obj_cache == false) {
      if(line.y != 0) {
        render_line_oam_rto();
        render_line();
      }
    } else {
      if(line.y != 0) render_line();
      render_line_oam_rto();
    }
  }
}

void bPPU::frame() {
  PPU::frame();

  display.field ^= 1;
  if(display.field == 0) {
    display.interlace = regs.interlace;
    regs.scanlines = (regs.overscan == false) ? 224 : 239;
  }
}

void bPPU::power() {
  PPU::power();

  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);

  region = snes.region();

  //$2100
  regs.display_disabled   = 1;
  regs.display_brightness = 15;

  //$2101
  regs.oam_basesize   = 0;
  regs.oam_nameselect = 0;
  regs.oam_tdaddr     = 0x0000;

  //$2102-$2103
  regs.oam_baseaddr    = 0x0000;
  regs.oam_addr        = 0x0000;
  regs.oam_priority    = false;
  regs.oam_firstsprite = 0;

  //$2104
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
  regs.mosaic_countdown    = 0;

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
  regs.bg_ofslatch = 0x00;
  regs.m7_hofs = regs.m7_vofs = 0x0000;
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
  regs.m7_latch = 0x00;
  regs.m7a = 0x0000;
  regs.m7b = 0x0000;
  regs.m7c = 0x0000;
  regs.m7d = 0x0000;
  regs.m7x = 0x0000;
  regs.m7y = 0x0000;

  //$2121
  regs.cgram_addr = 0x0000;

  //$2122
  regs.cgram_latchdata = 0x00;

  //$2123-$2125
  regs.window1_enabled[BG1] = false;
  regs.window1_enabled[BG2] = false;
  regs.window1_enabled[BG3] = false;
  regs.window1_enabled[BG4] = false;
  regs.window1_enabled[OAM] = false;
  regs.window1_enabled[COL] = false;

  regs.window1_invert [BG1] = false;
  regs.window1_invert [BG2] = false;
  regs.window1_invert [BG3] = false;
  regs.window1_invert [BG4] = false;
  regs.window1_invert [OAM] = false;
  regs.window1_invert [COL] = false;

  regs.window2_enabled[BG1] = false;
  regs.window2_enabled[BG2] = false;
  regs.window2_enabled[BG3] = false;
  regs.window2_enabled[BG4] = false;
  regs.window2_enabled[OAM] = false;
  regs.window2_enabled[COL] = false;

  regs.window2_invert [BG1] = false;
  regs.window2_invert [BG2] = false;
  regs.window2_invert [BG3] = false;
  regs.window2_invert [BG4] = false;
  regs.window2_invert [OAM] = false;
  regs.window2_invert [COL] = false;

  //$2126-$2129
  regs.window1_left  = 0x00;
  regs.window1_right = 0x00;
  regs.window2_left  = 0x00;
  regs.window2_right = 0x00;

  //$212a-$212b
  regs.window_mask[BG1] = 0;
  regs.window_mask[BG2] = 0;
  regs.window_mask[BG3] = 0;
  regs.window_mask[BG4] = 0;
  regs.window_mask[OAM] = 0;
  regs.window_mask[COL] = 0;

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
  regs.window_enabled[BG1]     = false;
  regs.window_enabled[BG2]     = false;
  regs.window_enabled[BG3]     = false;
  regs.window_enabled[BG4]     = false;
  regs.window_enabled[OAM]     = false;
  regs.sub_window_enabled[BG1] = false;
  regs.sub_window_enabled[BG2] = false;
  regs.sub_window_enabled[BG3] = false;
  regs.sub_window_enabled[BG4] = false;
  regs.sub_window_enabled[OAM] = false;

  //$2130
  regs.color_mask    = 0;
  regs.colorsub_mask = 0;
  regs.addsub_mode   = false;
  regs.direct_color  = false;

  //$2131
  regs.color_mode          = 0;
  regs.color_halve         = false;
  regs.color_enabled[BACK] = false;
  regs.color_enabled[OAM]  = false;
  regs.color_enabled[BG4]  = false;
  regs.color_enabled[BG3]  = false;
  regs.color_enabled[BG2]  = false;
  regs.color_enabled[BG1]  = false;

  //$2132
  regs.color_r   = 0x00;
  regs.color_g   = 0x00;
  regs.color_b   = 0x00;
  regs.color_rgb = 0x0000;

  //$2133
  regs.mode7_extbg   = false;
  regs.pseudo_hires  = false;
  regs.overscan      = false;
  regs.scanlines     = 224;
  regs.oam_interlace = false;
  regs.interlace     = false;

  //$2137
  regs.hcounter         = 0;
  regs.vcounter         = 0;
  regs.latch_hcounter   = 0;
  regs.latch_vcounter   = 0;
  regs.counters_latched = false;

  //$2139-$213a
  regs.vram_readbuffer = 0x0000;

  //$213e
  regs.time_over  = false;
  regs.range_over = false;

  reset();
}

void bPPU::reset() {
  PPU::reset();
  frame();

  display.field = 0;
  display.interlace = false;
  display.overscan = false;

  memset(sprite_list, 0, sizeof(sprite_list));

  //open bus support
  regs.ppu1_mdr = 0xff;
  regs.ppu2_mdr = 0xff;

  //bg line counters
  regs.bg_y[0] = 0;
  regs.bg_y[1] = 0;
  regs.bg_y[2] = 0;
  regs.bg_y[3] = 0;

  clear_tiledata_cache();
}

uint8 bPPU::vram_read(uint16 addr) {
  return vram[addr];
}

void bPPU::vram_write(uint16 addr, uint8 value) {
  vram[addr] = value;
}

uint8 bPPU::oam_read(uint16 addr) {
  if(addr >= 0x0200) { addr = 0x0200 | (addr & 31); }
  return oam[addr];
}

void bPPU::oam_write(uint16 addr, uint8 value) {
  if(addr >= 0x0200) { addr = 0x0200 | (addr & 31); }
  oam[addr] = value;
}

uint8 bPPU::cgram_read(uint16 addr) {
  addr &= 511;
  uint8 r = cgram[addr];
  if(addr & 1) { r &= 0x7f; }
  return r;
}

void bPPU::cgram_write(uint16 addr, uint8 value) {
  addr &= 511;
  if(addr & 1) { value &= 0x7f; }
  cgram[addr] = value;
}

bPPU::bPPU() {
  vram  = new(zeromemory) uint8_t[65536];
  oam   = new(zeromemory) uint8_t[  544];
  cgram = new(zeromemory) uint8_t[  512];

  init_tiledata_cache();

  for(int l = 0; l < 16; l++) {
    for(int i = 0; i < 4096; i++) {
      mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
    }
  }

  for(int l = 0; l < 16; l++) {
    double m = (double)l / 15.0;
    for(int i = 0; i < 32 * 32; i++) {
      int r = (int)((double)((i)      & 31) * m + 0.5);
      int g = (int)((double)((i >> 5) & 31) * m + 0.5);
      r = max(0, min(31, r));
      g = max(0, min(31, g));
      if(i < 32) light_table_b[l][i]  = (r << 10);
      light_table_gr[l][i] = (g << 5) | (r);
    }
  }
}

bPPU::~bPPU() {
  delete[] vram;
  delete[] oam;
  delete[] cgram;
}
