#ifdef PPU_CPP

#include "mode7.cpp"

void PPU::Background::frame() {
}

void PPU::Background::scanline() {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  x = -8 << hires;
  y = self.vcounter();
  edge = 7 - (regs.hoffset & 7);
  for(unsigned n = 0; n < 8; n++) data[n] = 0;

  if(self.vcounter() == 1) {
    mosaic_vcounter = regs.mosaic + 1;
    mosaic_voffset = 1;
  } else if(--mosaic_vcounter == 0) {
    mosaic_vcounter = regs.mosaic + 1;
    mosaic_voffset += regs.mosaic + 1;
  }

  mosaic_hcounter = 0;
  mosaic_hoffset = 0;
}

void PPU::Background::get_tile() {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);

  unsigned color_depth = (regs.mode == Mode::BPP2 ? 0 : regs.mode == Mode::BPP4 ? 1 : 2);
  unsigned palette_offset = (self.regs.bgmode == 0 ? (id << 5) : 0);
  unsigned palette_size = 2 << color_depth;
  unsigned tile_mask = 0x0fff >> color_depth;
  unsigned tiledata_index = regs.tiledata_addr >> (4 + color_depth);

  unsigned tile_height = (regs.tile_size == TileSize::Size8x8 ? 3 : 4);
  unsigned tile_width = (!hires ? tile_height : 4);

  unsigned width = (!hires ? 256 : 512);

  unsigned mask_x = (tile_height == 3 ? width : (width << 1));
  unsigned mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  unsigned px = x;
  unsigned py = mosaic_voffset;

  unsigned hscroll = regs.hoffset;
  unsigned vscroll = regs.voffset;
  if(hires) {
    hscroll <<= 1;
    if(self.regs.interlace) py = (py << 1) + self.field();
  }

  unsigned hoffset = hscroll + px;
  unsigned voffset = vscroll + py;

  if(self.regs.bgmode == 2 || self.regs.bgmode == 4 || self.regs.bgmode == 6) {
    uint16 opt_x = (x + (hscroll & 7));

    if(opt_x >= 8) {
      unsigned hval = self.bg3.get_tile((opt_x - 8) + (self.bg3.regs.hoffset & ~7), self.bg3.regs.voffset + 0);
      unsigned vval = self.bg3.get_tile((opt_x - 8) + (self.bg3.regs.hoffset & ~7), self.bg3.regs.voffset + 8);
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
        if(hval & opt_valid_bit) hoffset = opt_x + (hval & ~7);
        if(vval & opt_valid_bit) voffset = y + vval;
      }
    }
  }

  hoffset &= mask_x;
  voffset &= mask_y;

  unsigned screen_x = (regs.screen_size & 1 ? (32 << 5) : 0);
  unsigned screen_y = (regs.screen_size & 2 ? (32 << 5) : 0);
  if(regs.screen_size == 3) screen_y <<= 1;

  unsigned tx = hoffset >> tile_width;
  unsigned ty = voffset >> tile_height;

  uint16 offset = ((ty & 0x1f) << 5) + (tx & 0x1f);
  if(tx & 0x20) offset += screen_x;
  if(ty & 0x20) offset += screen_y;

  uint16 addr = regs.screen_addr + (offset << 1);
  tile = (memory::vram[addr + 0] << 0) + (memory::vram[addr + 1] << 8);
  bool mirror_y = tile & 0x8000;
  bool mirror_x = tile & 0x4000;
  priority = (tile & 0x2000 ? regs.priority1 : regs.priority0);
  palette_number = (tile >> 10) & 7;
  palette_index = palette_offset + (palette_number << palette_size);

  if(tile_width   == 4 && (bool)(hoffset & 8) != mirror_x) tile +=  1;
  if(tile_height  == 4 && (bool)(voffset & 8) != mirror_y) tile += 16;
  uint16 character = ((tile & 0x03ff) + tiledata_index) & tile_mask;

  if(mirror_y) voffset ^= 7;
  offset = (character << (4 + color_depth)) + ((voffset & 7) << 1);

  if(regs.mode >= Mode::BPP2) {
    data[0] = memory::vram[offset +  0];
    data[1] = memory::vram[offset +  1];
  }
  if(regs.mode >= Mode::BPP4) {
    data[2] = memory::vram[offset + 16];
    data[3] = memory::vram[offset + 17];
  }
  if(regs.mode >= Mode::BPP8) {
    data[4] = memory::vram[offset + 32];
    data[5] = memory::vram[offset + 33];
    data[6] = memory::vram[offset + 48];
    data[7] = memory::vram[offset + 49];
  }

  if(mirror_x) for(unsigned n = 0; n < 8; n++) {
    data[n] = ((data[n] >> 4) & 0x0f) | ((data[n] << 4) & 0xf0);
    data[n] = ((data[n] >> 2) & 0x33) | ((data[n] << 2) & 0xcc);
    data[n] = ((data[n] >> 1) & 0x55) | ((data[n] << 1) & 0xaa);
  }
}

void PPU::Background::run() {
  if(self.vcounter() == 0) return;
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);

  if((self.hcounter() & 2) == 0) {
    output.main.priority = 0;
    output.sub.priority = 0;
  } else if(hires == false) {
    return;
  }

  if(regs.mode == Mode::Inactive) return;
  if(regs.main_enable == false && regs.sub_enable == false) return;

  if(regs.mode == Mode::Mode7) return run_mode7();

  if((x++ & 7) == edge) get_tile();

  uint8 palette = get_tile_color();
  if(x >= 0 && mosaic_hcounter++ >= regs.mosaic) {
    mosaic_hcounter = 0;
    mosaic_palette = palette;
  }
  if(mosaic_palette == 0) return;

  if(hires == false) {
    if(regs.main_enable) {
      output.main.priority = priority;
      output.main.palette = palette_index + mosaic_palette;
      output.main.tile = tile;
    }

    if(regs.sub_enable) {
      output.sub.priority = priority;
      output.sub.palette = palette_index + mosaic_palette;
      output.sub.tile = tile;
    }
  } else {
    if(x & 1) {
      if(regs.main_enable) {
        output.main.priority = priority;
        output.main.palette = palette_index + mosaic_palette;
        output.main.tile = tile;
      }
    } else {
      if(regs.sub_enable) {
        output.sub.priority = priority;
        output.sub.palette = palette_index + mosaic_palette;
        output.sub.tile = tile;
      }
    }
  }
}

unsigned PPU::Background::get_tile_color() {
  unsigned color = 0;
  if(regs.mode >= Mode::BPP2) {
    color += (data[0] & 0x80) ? 0x01 : 0; data[0] <<= 1;
    color += (data[1] & 0x80) ? 0x02 : 0; data[1] <<= 1;
  }
  if(regs.mode >= Mode::BPP4) {
    color += (data[2] & 0x80) ? 0x04 : 0; data[2] <<= 1;
    color += (data[3] & 0x80) ? 0x08 : 0; data[3] <<= 1;
  }
  if(regs.mode >= Mode::BPP8) {
    color += (data[4] & 0x80) ? 0x10 : 0; data[4] <<= 1;
    color += (data[5] & 0x80) ? 0x20 : 0; data[5] <<= 1;
    color += (data[6] & 0x80) ? 0x40 : 0; data[6] <<= 1;
    color += (data[7] & 0x80) ? 0x80 : 0; data[7] <<= 1;
  }
  return color;
}

void PPU::Background::reset() {
  regs.tiledata_addr = 0;
  regs.screen_addr = 0;
  regs.screen_size = 0;
  regs.mosaic = 0;
  regs.tile_size = 0;
  regs.mode = 0;
  regs.priority0 = 0;
  regs.priority1 = 0;
  regs.main_enable = 0;
  regs.sub_enable = 0;
  regs.hoffset = 0;
  regs.voffset = 0;

  output.main.palette = 0;
  output.main.priority = 0;
  output.sub.palette = 0;
  output.sub.priority = 0;

  x = 0;
  y = 0;
  edge = 0;

  mosaic_vcounter = 0;
  mosaic_voffset = 0;
  mosaic_hcounter = 0;
  mosaic_hoffset = 0;
  mosaic_palette = 0;

  tile = 0;
  priority = 0;
  palette_number = 0;
  palette_index = 0;
  for(unsigned n = 0; n < 8; n++) data[n] = 0;
}

unsigned PPU::Background::get_tile(unsigned x, unsigned y) {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  unsigned tile_height = (regs.tile_size == TileSize::Size8x8 ? 3 : 4);
  unsigned tile_width = (!hires ? tile_height : 4);
  unsigned width = (!hires ? 256 : 512);
  unsigned mask_x = (tile_height == 3 ? width : (width << 1));
  unsigned mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  unsigned screen_x = (regs.screen_size & 1 ? (32 << 5) : 0);
  unsigned screen_y = (regs.screen_size & 2 ? (32 << 5) : 0);
  if(regs.screen_size == 3) screen_y <<= 1;

  x = (x & mask_x) >> tile_width;
  y = (y & mask_y) >> tile_height;

  uint16 offset = ((y & 0x1f) << 5) + (x & 0x1f);
  if(x & 0x20) offset += screen_x;
  if(y & 0x20) offset += screen_y;

  uint16 addr = regs.screen_addr + (offset << 1);
  return (memory::vram[addr + 0] << 0) + (memory::vram[addr + 1] << 8);
}

PPU::Background::Background(PPU &self, unsigned id) : self(self), id(id) {
}

#endif
