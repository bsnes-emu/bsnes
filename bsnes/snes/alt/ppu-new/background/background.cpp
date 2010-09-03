#ifdef PPU_CPP

#include "mode7.cpp"

unsigned PPU::Background::get_tile(unsigned hoffset, unsigned voffset) {
  unsigned tile_x = hoffset >> tile_width;
  unsigned tile_y = voffset >> tile_height;

  unsigned tile_pos = ((tile_y & 0x1f) << 5) + (tile_x & 0x1f);
  if(tile_y & 0x20) tile_pos += scy;
  if(tile_x & 0x20) tile_pos += scx;

  const unsigned tiledata_addr = regs.screen_addr + (tile_pos << 1);
  return (memory::vram[tiledata_addr + 0] << 0) + (memory::vram[tiledata_addr + 1] << 8);
}

void PPU::Background::offset_per_tile(unsigned x, unsigned &hoffset, unsigned &voffset) {
  unsigned opt_x = (x + (hscroll & 7)), hval, vval;
  if(opt_x >= 8) {
    hval = self.bg3.get_tile((opt_x - 8) + (self.bg3.regs.hoffset & ~7), self.bg3.regs.voffset + 0);
    if(self.regs.bgmode != 4)
    vval = self.bg3.get_tile((opt_x - 8) + (self.bg3.regs.hoffset & ~7), self.bg3.regs.voffset + 8);

    if(self.regs.bgmode == 4) {
      if(hval & opt_valid_bit) {
        hoffset = opt_x + (hval & ~7);
      } else {
        voffset = y + hval;
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

void PPU::Background::scanline() {
  y = mosaic_table[regs.mosaic][self.vcounter()] + (regs.mosaic > 0);
  hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  width = !hires ? 256 : 512;

  tile_height = regs.tile_size ? 4 : 3;
  tile_width = hires ? 4 : tile_height;

  mask_x = (tile_height == 4 ? width << 1 : width);
  mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  scx = (regs.screen_size & 1 ? 32 << 5 : 0);
  scy = (regs.screen_size & 2 ? 32 << 5 : 0);
  if(regs.screen_size == 3) scy <<= 1;
}

void PPU::Background::render() {
  if(regs.mode == Mode::Inactive) return;
  if(regs.main_enable == false && regs.sub_enable == false) return;

  if(regs.main_enable) window.render(0);
  if(regs.sub_enable) window.render(1);
  if(regs.mode == Mode::Mode7) return render_mode7();

  const unsigned bgpal_index = (self.regs.bgmode == 0 ? id << 5 : 0);
  const unsigned pal_size = 2 << regs.mode;
  const unsigned tile_mask = 0x0fff >> regs.mode;
  const unsigned tiledata_index = regs.tiledata_addr >> (4 + regs.mode);

  hscroll = regs.hoffset;
  vscroll = regs.voffset;

  if(hires) {
    hscroll <<= 1;
    if(self.regs.interlace) y = (y << 1) + self.field();
  }

  unsigned tile_pri, tile_num;
  unsigned pal_index, pal_num;
  unsigned hoffset, voffset, col;
  bool mirror_x, mirror_y;

  const bool is_opt_mode = (self.regs.bgmode == 2 || self.regs.bgmode == 4 || self.regs.bgmode == 6);
  const bool is_direct_color_mode = (self.screen.regs.direct_color == true && id == ID::BG1 && (self.regs.bgmode == 3 || self.regs.bgmode == 4));

  signed x = 0 - (hscroll & 7);
  while(x < width) {
    hoffset = x + hscroll;
    voffset = y + vscroll;
    if(is_opt_mode) offset_per_tile(x, hoffset, voffset);
    hoffset &= mask_x;
    voffset &= mask_y;

    tile_num = get_tile(hoffset, voffset);
    mirror_y = tile_num & 0x8000;
    mirror_x = tile_num & 0x4000;
    tile_pri = tile_num & 0x2000 ? regs.priority1 : regs.priority0;
    pal_num = (tile_num >> 10) & 7;
    pal_index = bgpal_index + (pal_num << pal_size);

    if(tile_width == 4) {
      if((bool)(hoffset & 8) != mirror_x) tile_num++;
    }

    if(tile_height == 4) {
      if((bool)(voffset & 8) != mirror_y) tile_num += 16;
    }

    tile_num &= 0x03ff;
    tile_num += tiledata_index;
    tile_num &= tile_mask;

    if(mirror_y) voffset ^= 7;
    signed step = mirror_x ? -1 : +1;
    unsigned plot_x = x + (mirror_x ? 7 : 0);

    uint8 *tiledata = self.cache.tile(regs.mode, tile_num);
    tiledata += ((voffset & 7) * 8);

    for(unsigned n = 0; n < 8; n++) {
      unsigned col = tiledata[n & regs.mosaic_mask];
      if(col && !(plot_x & width)) {
        unsigned color;
        if(is_direct_color_mode) {
          color = self.screen.get_direct_color(pal_num, col);
        } else {
          color = self.screen.get_palette(pal_index + col);
        }

        if(hires == false) {
          if(regs.main_enable && !window.main[plot_x]) self.screen.output.plot_main(plot_x, color, tile_pri, id);
          if(regs.sub_enable && !window.sub[plot_x]) self.screen.output.plot_sub(plot_x, color, tile_pri, id);
        } else {
          signed half_x = plot_x >> 1;
          if(plot_x & 1) {
            if(regs.main_enable && !window.main[half_x]) self.screen.output.plot_main(half_x, color, tile_pri, id);
          } else {
            if(regs.sub_enable && !window.sub[half_x]) self.screen.output.plot_sub(half_x, color, tile_pri, id);
          }
        }
      }
      plot_x += step;
    }

    x += 8;
  }
}

PPU::Background::Background(PPU &self, unsigned id) : self(self), id(id) {
  opt_valid_bit = (id == ID::BG1 ? 0x2000 : id == ID::BG2 ? 0x4000 : 0x0000);

  mosaic_table = new uint16*[16];
  for(unsigned m = 0; m < 16; m++) {
    mosaic_table[m] = new uint16[4096];
    for(unsigned x = 0; x < 4096; x++) {
      mosaic_table[m][x] = (x / (m + 1)) * (m + 1);
    }
  }
}

PPU::Background::~Background() {
  for(unsigned m = 0; m < 16; m++) delete[] mosaic_table[m];
  delete[] mosaic_table;
}

#endif
