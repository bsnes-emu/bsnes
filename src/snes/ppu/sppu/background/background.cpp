#ifdef SPPU_CPP

#include "mode7.cpp"

void sPPU::Background::scanline() {
  if(self.vcounter() == 1) {
    state.mosaic_y = 1;
    state.mosaic_countdown = 0;
  } else {
    if(!regs.mosaic || !state.mosaic_countdown) state.mosaic_y = self.vcounter();
    if(!state.mosaic_countdown) state.mosaic_countdown = regs.mosaic + 1;
    state.mosaic_countdown--;
  }

  state.x = 0;
}

void sPPU::Background::run() {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);

  if((self.hcounter() & 2) == 0) {
    output.main.priority = 0;
    output.sub.priority = 0;
  } else if(hires == false) {
    return;
  }

  if(regs.mode == Mode::Inactive) return;
  if(regs.main_enabled == false && regs.sub_enabled == false) return;

  unsigned x = state.x++;
  unsigned y = state.mosaic_y;
  if(regs.mode == Mode::Mode7) return run_mode7(x, y);

  unsigned color_depth = (regs.mode == Mode::BPP2 ? 0 : regs.mode == Mode::BPP4 ? 1 : 2);
  unsigned palette_offset = (self.regs.bgmode == 0 ? (id << 5) : 0);
  unsigned palette_size = 2 << color_depth;
  unsigned tile_mask = 0x0fff >> color_depth;
  unsigned tiledata_index = regs.tiledata_addr >> (4 + color_depth);

  unsigned tile_height = (regs.tile_size == TileSize::Size8x8 ? 3 : 4);
  unsigned tile_width  = (!hires ? tile_height : 4);

  unsigned width = (!hires ? 256 : 512);
  unsigned mask_x = (tile_height == 3 ? width : (width << 1));
  unsigned mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  unsigned screen_y = (regs.screen_size & 2 ? (32 << 5) : 0);
  unsigned screen_x = (regs.screen_size & 1 ? (32 << 5) : 0);
  if(regs.screen_size == 3) screen_y <<= 1;

  unsigned hscroll = regs.hoffset;
  unsigned vscroll = regs.voffset;
  if(hires) {
    hscroll <<= 1;
    if(self.regs.interlace) y = (y << 1) + self.field();
  }

  unsigned hoffset = hscroll + mosaic_table[regs.mosaic][x];
  unsigned voffset = vscroll + y;

  bool offset_per_tile = (self.regs.bgmode == 2 || self.regs.bgmode == 4 || self.regs.bgmode == 6);
  if(id != ID::BG1 && id != ID::BG2) offset_per_tile = false;

  if(offset_per_tile) {
    uint16 opt_x = (x + (hscroll & 7));

    //tile 0 is unaffected by offset-per-tile mode
    if(opt_x >= 8) {
      uint16 hval; {
        unsigned px = (opt_x - 8) + (self.bg3.regs.hoffset & ~7);
        unsigned py = self.bg3.regs.voffset;
        unsigned tx = (px & (width - 1)) >> tile_width;
        unsigned ty = (py & (width - 1)) >> tile_height;
        uint16 pos = ((ty & 0x1f) << 5) + (tx & 0x1f);
        uint16 addr = self.bg3.regs.screen_addr + (pos << 1);
        hval = memory::vram[addr + 0] + (memory::vram[addr + 1] << 8);
      }

      uint16 vval; if(self.regs.bgmode != 4) {
        unsigned px = (opt_x - 8) + (self.bg3.regs.hoffset & ~7);
        unsigned py = self.bg3.regs.voffset + 8;
        unsigned tx = (px & (width - 1)) >> tile_width;
        unsigned ty = (py & (width - 1)) >> tile_height;
        uint16 pos = ((ty & 0x1f) << 5) + (tx & 0x1f);
        uint16 addr = self.bg3.regs.screen_addr + (pos << 1);
        vval = memory::vram[addr + 0] + (memory::vram[addr + 1] << 8);
      }

      unsigned opt_valid_bit = (id == ID::BG1 ? 0x2000 : 0x4000);

      if(self.regs.bgmode == 4) {
        if(hval & opt_valid_bit) {
          if(!(hval & 0x8000)) {
            hoffset = opt_x + (hval & ~7);
          } else {
            voffset = y + hval;
          }
        }
      } else {
        if(hval & opt_valid_bit) {
          hoffset = opt_x + (hval & ~7);
        }
        if(vval & opt_valid_bit) {
          voffset = y + vval;
        }
      }
    }
  }

  hoffset &= mask_x;
  voffset &= mask_y;

  unsigned tile_number; {
    unsigned tx = hoffset >> tile_width;
    unsigned ty = voffset >> tile_height;
    uint16 pos = ((ty & 0x1f) << 5) + (tx & 0x1f);
    if(ty & 0x20) pos += screen_y;
    if(tx & 0x20) pos += screen_x;
    uint16 addr = regs.screen_addr + (pos << 1);
    tile_number = memory::vram[addr + 0] + (memory::vram[addr + 1] << 8);
  }

  bool mirror_y = tile_number & 0x8000;
  bool mirror_x = tile_number & 0x4000;
  unsigned priority = (tile_number & 0x2000 ? regs.priority1 : regs.priority0);
  unsigned palette_number = (tile_number >> 10) & 7;
  unsigned palette_index = palette_offset + (palette_number << palette_size);

  if(tile_width  == 4 && (bool)(hoffset & 8) != mirror_x) tile_number +=  1;
  if(tile_height == 4 && (bool)(voffset & 8) != mirror_y) tile_number += 16;
  tile_number &= 0x03ff;
  tile_number += tiledata_index;
  tile_number &= tile_mask;

  if(mirror_y) voffset ^= 7;
  if(mirror_x) hoffset ^= 7;

  uint8 color = get_color(hoffset, voffset, tile_number);
  if(color == 0) return;

  color += palette_index;

  if(hires == false) {
    if(regs.main_enabled) {
      output.main.palette = color | (palette_number << 8);
      output.main.priority = priority;
    }

    if(regs.sub_enabled) {
      output.sub.palette = color | (palette_number << 8);
      output.sub.priority = priority;
    }
  } else {
    if(x & 1) {
      if(regs.main_enabled) {
        output.main.palette = color | (palette_number << 8);
        output.main.priority = priority;
      }
    } else {
      if(regs.sub_enabled) {
        output.sub.palette = color | (palette_number << 8);
        output.sub.priority = priority;
      }
    }
  }
}

unsigned sPPU::Background::get_color(unsigned x, unsigned y, uint16 offset) {
  unsigned mask = 0x80 >> (x & 7);

  switch(regs.mode) {
    case Background::Mode::BPP2: {
      offset = (offset * 16) + ((y & 7) * 2);

      unsigned d0 = memory::vram[offset +  0];
      unsigned d1 = memory::vram[offset +  1];

      return (((bool)(d0 & mask)) << 0)
           + (((bool)(d1 & mask)) << 1);
    }

    case Background::Mode::BPP4: {
      offset = (offset * 32) + ((y & 7) * 2);

      unsigned d0 = memory::vram[offset +  0];
      unsigned d1 = memory::vram[offset +  1];
      unsigned d2 = memory::vram[offset + 16];
      unsigned d3 = memory::vram[offset + 17];

      return (((bool)(d0 & mask)) << 0)
           + (((bool)(d1 & mask)) << 1)
           + (((bool)(d2 & mask)) << 2)
           + (((bool)(d3 & mask)) << 3);
    }

    case Background::Mode::BPP8: {
      offset = (offset * 64) + ((y & 7) * 2);

      unsigned d0 = memory::vram[offset +  0];
      unsigned d1 = memory::vram[offset +  1];
      unsigned d2 = memory::vram[offset + 16];
      unsigned d3 = memory::vram[offset + 17];
      unsigned d4 = memory::vram[offset + 32];
      unsigned d5 = memory::vram[offset + 33];
      unsigned d6 = memory::vram[offset + 48];
      unsigned d7 = memory::vram[offset + 49];

      return (((bool)(d0 & mask)) << 0)
           + (((bool)(d1 & mask)) << 1)
           + (((bool)(d2 & mask)) << 2)
           + (((bool)(d3 & mask)) << 3)
           + (((bool)(d4 & mask)) << 4)
           + (((bool)(d5 & mask)) << 5)
           + (((bool)(d6 & mask)) << 6)
           + (((bool)(d7 & mask)) << 7);
    }
  };
}

void sPPU::Background::reset() {
  state.x = 0;
  state.mosaic_y = 0;
  state.mosaic_countdown = 0;
  regs.tiledata_addr = 0;
  regs.screen_addr = 0;
  regs.screen_size = 0;
  regs.mosaic = 0;
  regs.tile_size = 0;
  regs.mode = 0;
  regs.priority0 = 0;
  regs.priority1 = 0;
  regs.main_enabled = 0;
  regs.sub_enabled = 0;
  regs.hoffset = 0;
  regs.voffset = 0;
  output.main.palette = 0;
  output.main.priority = 0;
  output.sub.palette = 0;
  output.sub.priority = 0;
}

sPPU::Background::Background(sPPU &self, unsigned id) : self(self), id(id) {
  //generate mosaic table
  for(unsigned m = 0; m < 16; m++) {
    for(unsigned x = 0; x < 4096; x++) {
      mosaic_table[m][x] = (x / (m + 1)) * (m + 1);
    }
  }
}

uint16 sPPU::Background::mosaic_table[16][4096];

#endif
