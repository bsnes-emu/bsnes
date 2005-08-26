#include "../../base.h"
#include "bppu_mmio.cpp"
#include "bppu_render.cpp"

void bPPU::run() {}

void bPPU::scanline() {
  _y               = cpu->vcounter();
  _screen_width    = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
  _interlace       = cpu->interlace();
  _interlace_field = cpu->interlace_field();

  if(_y == 0) {
  //RTO flag reset
    regs.time_over  = false;
    regs.range_over = false;
  }

  if(_y == 1) {
  //OAM FirstSprite priority set
    if(regs.oam_priority == true) {
      regs.oam_firstsprite = (regs.oam_addr & 0xfe) >> 1;
    } else {
      regs.oam_firstsprite = 0;
    }
  }

  if(_y == (cpu->overscan()?239:224) && regs.display_disabled == false) {
  //OAM address reset
    regs.oam_addr = ((regs.oam_addrh << 8) | regs.oam_addrl) << 1;
  }

//only allow frameskip setting to ignore actual rendering; not RTO, etc.
  if(settings.frameskip_pos != 0)return;

  if(_y > 0 && _y < (cpu->overscan()?239:224)) {
    if(regs.bg_mode == 5 || regs.bg_mode == 6) {
      output->hires = true;
      output->line[_y].hires = true;
    }
    if(_interlace == true) {
      output->interlace = true;
      output->line[_y].interlace = true;
    }
    render_line();
  }
}

void bPPU::frame() {
  if(settings.frameskip_changed == true) {
    settings.frameskip_changed = false;
    settings.frameskip_pos = 0;
  } else {
    settings.frameskip_pos++;
    settings.frameskip_pos %= (settings.frameskip + 1);
  }

  if(settings.frameskip_pos != 0)return;

  snes->notify(SNES::RENDER_FRAME);
  output->hires     = false;
  output->interlace = false;
  for(int i=0;i<239;i++) {
    output->line[i].hires     = false;
    output->line[i].interlace = false;
  }
}

void bPPU::set_frameskip(int fs) {
  settings.frameskip = fs;
  settings.frameskip_changed = true;
}

void bPPU::power() {
  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);

  region = snes->region();

  reset();
}

void bPPU::reset() {
  memset(output->buffer, 0, 512 * 478 * 2);
  frame();

  memset(sprite_list, 0, sizeof(sprite_list));

//open bus support
  regs.ppu1_mdr = 0xff;
  regs.ppu2_mdr = 0xff;

//$2100
  regs.display_disabled   = 0;
  regs.display_brightness = 0;

//$2101
  regs.oam_basesize   = 0;
  regs.oam_nameselect = 0;
  regs.oam_tdaddr     = 0x0000;

//$2102-$2103
  regs.oam_addrl       = 0x00;
  regs.oam_addrh       = 0x00;
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

//$213e
  regs.time_over  = false;
  regs.range_over = false;

  update_sprite_list_sizes();
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
  update_sprite_list(addr);
  snes->notify(SNES::OAM_WRITE, addr, value);
}

uint8 bPPU::cgram_read(uint16 addr) {
uint8 r;
  addr &= 511;
  r = cgram[addr];
  if(addr & 1) {
    r &= 0x7f;
  }
  snes->notify(SNES::CGRAM_READ, addr, r);
  return r;
}

void bPPU::cgram_write(uint16 addr, uint8 value) {
  addr &= 511;
  if(addr & 1) {
    value &= 0x7f;
  }
  cgram[addr] = value;
  snes->notify(SNES::CGRAM_WRITE, addr, value);
}

bPPU::bPPU() {
  settings.frameskip         = 0;
  settings.frameskip_pos     = 0;
  settings.frameskip_changed = false;

  mmio = new bPPUMMIO(this);

  vram  = (uint8*)memalloc(65536, "bPPU::vram");
  oam   = (uint8*)memalloc(  544, "bPPU::oam");
  cgram = (uint8*)memalloc(  512, "bPPU::cgram");
  memset(vram,  0, 65536);
  memset(oam,   0,   544);
  memset(cgram, 0,   512);

  init_tiledata_cache();

int     i, l;
uint8   r, g, b;
float   m;
uint16 *ptr;
  for(l=0;l<16;l++) {
    mosaic_table[l] = (uint16*)malloc(4096 * 2);
    for(i=0;i<4096;i++) {
      mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
    }
  }

  light_table = (uint16*)malloc(16 * 32768 * 2);
  ptr = (uint16*)light_table;
  for(l=0;l<16;l++) {
    m = (float)l / 15.0;
    for(i=0;i<32768;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      if(l == 0) { r = g = b = 0; }
      else if(l == 15);
      else {
        r = (uint8)((float)r * m);
        g = (uint8)((float)g * m);
        b = (uint8)((float)b * m);
      }
      *ptr++ = (r) | (g << 5) | (b << 10);
    }
  }
}

bPPU::~bPPU() {
  delete(mmio);

  if(vram) {
    free(vram);
    vram = 0;
  }
  if(oam) {
    free(oam);
    oam = 0;
  }
  if(cgram) {
    free(cgram);
    cgram = 0;
  }

  for(int i=0;i<16;i++) {
    if(mosaic_table[i]) {
      free(mosaic_table[i]);
      mosaic_table[i] = 0;
    }
  }

  if(light_table) {
    memfree(light_table);
    light_table = 0;
  }
}

bPPUMMIO::bPPUMMIO(bPPU *_ppu) {
  ppu = _ppu;
}
