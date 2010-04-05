#ifdef SPPU_CPP

void sPPU::Background::scanline() {
  if(self.vcounter() == 1) {
    regs.mosaic_y = 1;
    regs.mosaic_countdown = 0;
  } else {
    if(!regs.mosaic || !regs.mosaic_countdown) regs.mosaic_y = self.vcounter();
    if(!regs.mosaic_countdown) regs.mosaic_countdown = regs.mosaic + 1;
    regs.mosaic_countdown--;
  }
}

void sPPU::Background::run() {
  output.main.valid = false;
  output.sub.valid = false;
  if(regs.mode == Mode::Inactive) return;
  if(regs.main_enabled == false && regs.sub_enabled == false) return;

  if(self.hcounter() < 88) return;
  if(self.hcounter() >= 1112) return;
  if(self.hcounter() & 2) return;

  unsigned x = (self.hcounter() - 88) >> 2;
  unsigned y = regs.mosaic_y;

  unsigned color_depth = (regs.mode == Mode::BPP2 ? 0 : regs.mode == Mode::BPP4 ? 1 : 2);
  unsigned palette_offset = (self.regs.bgmode == 0 ? (id << 5) : 0);
  unsigned palette_size = 2 << color_depth;
  unsigned tile_mask = 0x0fff >> color_depth;
  unsigned tiledata_index = regs.tiledata_addr >> (4 + color_depth);

  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  unsigned width = (!hires ? 256 : 512);

  unsigned tile_height = (regs.tile_size == TileSize::Size8x8 ? 3 : 4);
  unsigned tile_width  = (!hires ? tile_height : 4);

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
  }

  unsigned hoffset = hscroll + mosaic_table[regs.mosaic][x];
  unsigned voffset = vscroll + y;

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

  unsigned color = get_color(hoffset, voffset, tile_number);
  if(color == 0) return;

  unsigned palette_addr = (palette_index + color) << 1;
  unsigned output_color = memory::cgram[palette_addr + 0] + (memory::cgram[palette_addr + 1] << 8);

  if(regs.main_enabled) {
    output.main.valid = true;
    output.main.color = output_color;
    output.main.priority = priority;
  }

  if(regs.sub_enabled) {
    output.sub.valid = true;
    output.sub.color = output_color;
    output.sub.priority = priority;
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

sPPU::Background::Background(sPPU &self, unsigned id) : self(self), id(id) {
  //generate mosaic table
  for(unsigned m = 0; m < 16; m++) {
    for(unsigned x = 0; x < 4096; x++) {
      mosaic_table[m][x] = (x / (m + 1)) * (m + 1);
    }
  }

  regs.tiledata_addr = 0x0000;
  regs.screen_addr = 0x0000;
  regs.screen_size = ScreenSize::Size32x32;
  regs.mosaic = 0;
  regs.mosaic_y = 0;
  regs.tile_size = TileSize::Size8x8;

  regs.mode = Mode::BPP2;
  regs.priority0 = 0;
  regs.priority1 = 0;

  regs.main_enabled = false;
  regs.sub_enabled = false;
}

uint16 sPPU::Background::mosaic_table[16][4096];

#endif
