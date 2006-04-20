#include "../../base.h"
#include "bppu_mmio.cpp"
#include "bppu_render.cpp"

void bPPU::run() {}

void bPPU::scanline() {
  line.y               = r_cpu->vcounter();
  line.width           = (regs.hires) ? 512 : 256;
  line.interlace       = r_cpu->interlace();
  line.interlace_field = r_cpu->interlace_field();

//this should probably only be done once per frame (at the
//start of vblank), however it is currently done every scanline
//as an attempt to emulate FirstSprite+Y priority mode
  if(regs.oam_priority == true) {
    if((regs.oam_addr & 3) == 3) {
    //FirstSprite+Y priority (untested)
      regs.oam_firstsprite = (regs.oam_addr >> 2) + (line.y - 1);
    } else {
      regs.oam_firstsprite = (regs.oam_addr >> 2);
    }
    regs.oam_firstsprite &= 127;
  } else {
    regs.oam_firstsprite = 0;
  }

  if(line.y == 0) {
  //RTO flag reset
    regs.time_over  = false;
    regs.range_over = false;
  }

  if(line.y == 1) {
  //mosaic reset
    for(int32 bg = BG1; bg <= BG4; bg++) {
      regs.bg_y[bg] = 1;
    }

    regs.mosaic_countdown = regs.mosaic_size + 1;
    regs.mosaic_countdown--;
  } else {
    for(int32 bg = BG1; bg <= BG4; bg++) {
      if(!regs.mosaic_enabled[bg] || !regs.mosaic_countdown) {
        regs.bg_y[bg] = line.y;
      }
    }

    if(!regs.mosaic_countdown) {
      regs.mosaic_countdown = regs.mosaic_size + 1;
    }
    regs.mosaic_countdown--;
  }

//TODO: line.y position needs to be verified
  if(line.y == (r_cpu->overscan() ? 240 : 225) && regs.display_disabled == false) {
  //OAM address reset
    regs.oam_addr = regs.oam_baseaddr << 1;
  }

  if(line.y == 241 && line.interlace_field == 1) {
    if(regs.interlace != r_cpu->interlace()) {
    //clear entire frame so that odd scanlines are empty
    //this should be handled better, but one blank frame looks
    //better than one improperly interlaced frame...
      memset(output, 0, 512 * 480 * sizeof(uint16));
    }
  //r_cpu->set_overscan(regs.overscan);
    r_cpu->set_interlace(regs.interlace);
    regs.scanlines = (regs.overscan == false) ? 224 : 239;
  }
}

void bPPU::render_scanline() {
  if(status.render_output == false)return;

  if(line.y > 0 && line.y < (r_cpu->overscan() ? 240 : 225)) {
    render_line();
  }
}

void bPPU::frame() {
  PPU::frame();
}

void bPPU::power() {
  PPU::power();

  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);

  region = snes->region();

  reset();
}

void bPPU::reset() {
  PPU::reset();
  frame();

  memset(sprite_list, 0, sizeof(sprite_list));

//open bus support
  regs.ppu1_mdr = 0xff;
  regs.ppu2_mdr = 0xff;

//bg line counters
  regs.bg_y[0] = 0;
  regs.bg_y[1] = 0;
  regs.bg_y[2] = 0;
  regs.bg_y[3] = 0;

//$2100
  regs.display_disabled   = 0;
  regs.display_brightness = 0;

//$2101
  regs.oam_basesize   = 0;
  regs.oam_nameselect = 0;
  regs.oam_tdaddr     = 0x0000;

//$2102-$2103
  regs.oam_baseaddr    = 0x0000;
  regs.oam_addr        = 0x0000;
  regs.oam_latchdata   = 0x00;
  regs.oam_priority    = false;
  regs.oam_firstsprite = 0x00;

//$2105
  regs.bg_tilesize[BG1] = 0;
  regs.bg_tilesize[BG2] = 0;
  regs.bg_tilesize[BG3] = 0;
  regs.bg_tilesize[BG4] = 0;
  regs.bg3_priority     = 0;
  regs.bg_mode          = 0;
  regs.hires            = false;

//$2106
  regs.mosaic_size          = 0;
  regs.mosaic_enabled[BG1]  = false;
  regs.mosaic_enabled[BG2]  = false;
  regs.mosaic_enabled[BG3]  = false;
  regs.mosaic_enabled[BG4]  = false;
  regs.mosaic_countdown     = 0;

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
  regs.bg_ofslatch  = 0x00;
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
  regs.window1_left  = 0;
  regs.window1_right = 0;
  regs.window2_left  = 0;
  regs.window2_right = 0;

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
  regs.addsub_mode   = 0;

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

  line.width = 256; //needed for clear_window_cache()
  clear_tiledata_cache();
}

uint8 bPPU::vram_read(uint16 addr) {
uint8 r;
  r = vram[addr];
#ifdef DEBUGGER
  snes->notify(SNES::VRAM_READ, addr, r);
#endif
  return r;
}

void bPPU::vram_write(uint16 addr, uint8 value) {
  vram[addr] = value;
#ifdef DEBUGGER
  snes->notify(SNES::VRAM_WRITE, addr, value);
#endif
}

uint8 bPPU::oam_read(uint16 addr) {
uint8 r;
  if(addr >= 0x0200)addr = 0x0200 | (addr & 31);
  r = oam[addr];
#ifdef DEBUGGER
  snes->notify(SNES::OAM_READ, addr, r);
#endif
  return r;
}

void bPPU::oam_write(uint16 addr, uint8 value) {
  if(addr >= 0x0200)addr = 0x0200 | (addr & 31);
  oam[addr] = value;
#ifdef DEBUGGER
  snes->notify(SNES::OAM_WRITE, addr, value);
#endif
}

uint8 bPPU::cgram_read(uint16 addr) {
uint8 r;
  addr &= 511;
  r = cgram[addr];
  if(addr & 1) {
    r &= 0x7f;
  }
#ifdef DEBUGGER
  snes->notify(SNES::CGRAM_READ, addr, r);
#endif
  return r;
}

void bPPU::cgram_write(uint16 addr, uint8 value) {
  addr &= 511;
  if(addr & 1) {
    value &= 0x7f;
  }
  cgram[addr] = value;
#ifdef DEBUGGER
  snes->notify(SNES::CGRAM_WRITE, addr, value);
#endif
}

bPPU::bPPU() {
  vram  = (uint8*)malloc(65536);
  oam   = (uint8*)malloc(  544);
  cgram = (uint8*)malloc(  512);
  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);

  init_tiledata_cache();

  for(int32 l = 0; l < 16; l++) {
    mosaic_table[l] = (uint16*)malloc(4096 * 2);
    for(int32 i = 0; i < 4096; i++) {
      mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
    }
  }

  light_table = (uint16*)malloc(16 * 32768 * 2);
uint16 *ptr = (uint16*)light_table;
  for(int32 l = 0; l < 16; l++) {
  int32  r, g, b;
  #if 0
  double y, cb, cr;
  double kr = 0.2126, kb = 0.0722, kg = (1.0 - kr - kb);
    for(int32 i = 0; i < 32768; i++) {
      if(l ==  0) { *ptr++ = 0; continue; }
      if(l == 15) { *ptr++ = i; continue; }

      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;

      y  = double(r) * kr + double(g) * kg + double(b) * kb;
      cb = (double(b) - y) / (2.0 - 2.0 * kb);
      cr = (double(r) - y) / (2.0 - 2.0 * kr);

      y  *= double(l) / 15.0;
      cb *= double(l) / 15.0;
      cr *= double(l) / 15.0;

      r = y + cr * (2.0 - 2.0 * kr);
      b = y + cb * (2.0 - 2.0 * kb);
      g = (y - b * kb - r * kr) / kg;

      r = (r > 31) ? 31 : ((r < 0) ? 0 : r);
      g = (g > 31) ? 31 : ((g < 0) ? 0 : g);
      b = (b > 31) ? 31 : ((b < 0) ? 0 : b);

      *ptr++ = (r) | (g << 5) | (b << 10);
    }
  #else
  double m = double(l) / 15.0;
    for(int32 i = 0; i < 32768; i++) {
      if(l ==  0) { *ptr++ = 0; continue; }
      if(l == 15) { *ptr++ = i; continue; }

      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;

      r = (int32)(double(r) * m);
      g = (int32)(double(g) * m);
      b = (int32)(double(b) * m);

      *ptr++ = (r) | (g << 5) | (b << 10);
    }
  #endif
  }
}

bPPU::~bPPU() {
  SafeFree(vram);
  SafeFree(oam);
  SafeFree(cgram);

  for(int i = 0; i < 16; i++) {
    SafeFree(mosaic_table[i]);
  }

  SafeFree(light_table);
}
