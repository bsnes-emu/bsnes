#ifdef PPU_CPP

void PPU::Background::render() {
  if(regs.mode == Mode::Inactive) return;
  if(regs.mode == Mode::Mode7) return;
  if(regs.main_enable == false && regs.sub_enable == false) return;

  const unsigned opt_valid_bit = (id == ID::BG1 ? 0x2000 : id == ID::BG2 ? 0x4000 : 0x0000);
  const unsigned bgpal_index = (self.regs.bgmode == 0 ? id << 5 : 0);

  const unsigned pal_size = 2 << regs.mode;
  const unsigned tile_mask = 0x0fff >> regs.mode;
  const unsigned tiledata_index = regs.tiledata_addr >> (4 + regs.mode);

  const bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  const signed width = !hires ? 256 : 512;

  const unsigned tile_height = regs.tile_size ? 4 : 3;
  const unsigned tile_width = hires ? 4 : tile_height;

  unsigned mask_x = (tile_height == 4 ? width << 1 : width);
  unsigned mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  unsigned scy = (regs.screen_size & 2 ? 32 << 5 : 0);
  unsigned scx = (regs.screen_size & 1 ? 32 << 5 : 0);
  if(regs.screen_size == 3) scy <<= 1;

  unsigned y = self.vcounter();
  unsigned hscroll = regs.hoffset;
  unsigned vscroll = regs.voffset;

  if(hires) {
    hscroll <<= 1;
    if(self.regs.interlace) y = (y << 1) + self.field();
  }

  unsigned hval, vval;
  unsigned tile_pri, tile_num;
  unsigned pal_index, pal_num;
  unsigned hoffset, voffset, opt_x, col;
  bool mirror_x, mirror_y;

  const bool is_opt_mode = (self.regs.bgmode == 2 || self.regs.bgmode == 4 || self.regs.bgmode == 6);
  const bool is_direct_color_mode = (self.screen.regs.direct_color == true && id == ID::BG1 && (self.regs.bgmode == 3 || self.regs.bgmode == 4));

  signed x = 0 - (hscroll & 7);
  while(x < width) {
    hoffset = x + hscroll;
    voffset = y + vscroll;

    hoffset &= mask_x;
    voffset &= mask_y;

    unsigned tile_x = hoffset >> tile_width;
    unsigned tile_y = voffset >> tile_height;
    unsigned tile_pos = ((tile_y & 0x1f) << 5) + (tile_x & 0x1f);
    if(tile_y & 0x20) tile_pos += scy;
    if(tile_x & 0x20) tile_pos += scx;
    const unsigned tiledata_addr = regs.screen_addr + (tile_pos << 1);

    tile_num = (memory::vram[tiledata_addr + 0] << 0) + (memory::vram[tiledata_addr + 1] << 8);
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
      unsigned col = *tiledata++;
      if(col) {
        unsigned color = self.screen.get_palette(pal_index + col);
        self.screen.output.plot_main(plot_x, color, tile_pri);
      }
      plot_x += step;
    }

    x += 8;
  }
}

PPU::Background::Background(PPU &self, unsigned id) : self(self), id(id) {
}

#endif
