#ifdef PPU_CPP

signed PPU::Background::clip(signed n) {
  //13-bit sign extend: --s---nnnnnnnnnn -> ssssssnnnnnnnnnn
  return n & 0x2000 ? (n | ~1023) : (n & 1023);
}

void PPU::Background::run_mode7(unsigned x, unsigned y) {
  signed a = sclip<16>(self.regs.m7a);
  signed b = sclip<16>(self.regs.m7b);
  signed c = sclip<16>(self.regs.m7c);
  signed d = sclip<16>(self.regs.m7d);

  signed cx = sclip<13>(self.regs.m7x);
  signed cy = sclip<13>(self.regs.m7y);
  signed hoffset = sclip<13>(self.regs.mode7_hoffset);
  signed voffset = sclip<13>(self.regs.mode7_voffset);

  if(self.regs.mode7_hflip) x = 255 - x;
  if(self.regs.mode7_vflip) y = 255 - y;

  unsigned mosaic_x;
  unsigned mosaic_y;
  if(id == ID::BG1) {
    mosaic_x = mosaic_table[self.bg1.regs.mosaic][x];
    mosaic_y = mosaic_table[self.bg1.regs.mosaic][y];
  } else if(id == ID::BG2) {
    mosaic_x = mosaic_table[self.bg2.regs.mosaic][x];
    mosaic_y = mosaic_table[self.bg1.regs.mosaic][y];  //BG2 vertical mosaic uses BG1 mosaic size
  }

  signed psx = ((a * clip(hoffset - cx)) & ~63) + ((b * clip(voffset - cy)) & ~63) + ((b * mosaic_y) & ~63) + (cx << 8);
  signed psy = ((c * clip(hoffset - cx)) & ~63) + ((d * clip(voffset - cy)) & ~63) + ((d * mosaic_y) & ~63) + (cy << 8);

  signed px = psx + (a * mosaic_x);
  signed py = psy + (c * mosaic_x);

  //mask pseudo-FP bits
  px >>= 8;
  py >>= 8;

  unsigned tile;
  unsigned palette;
  switch(self.regs.mode7_repeat) {
    //screen repetition outside of screen area
    case 0:
    case 1: {
      px &= 1023;
      py &= 1023;
      tile = memory::vram[((py >> 3) * 128 + (px >> 3)) << 1];
      palette = memory::vram[(((tile << 6) + ((py & 7) << 3) + (px & 7)) << 1) + 1];
    } break;

    //palette color 0 outside of screen area
    case 2: {
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        palette = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tile = memory::vram[((py >> 3) * 128 + (px >> 3)) << 1];
        palette = memory::vram[(((tile << 6) + ((py & 7) << 3) + (px & 7)) << 1) + 1];
      }
    } break;

    //character 0 repetition outside of screen area
    case 3: {
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        tile = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tile = memory::vram[((py >> 3) * 128 + (px >> 3)) << 1];
      }
      palette = memory::vram[(((tile << 6) + ((py & 7) << 3) + (px & 7)) << 1) + 1];
    } break;
  }

  unsigned priority;
  if(id == ID::BG1) {
    priority = regs.priority0;
  } else if(id == ID::BG2) {
    priority = (palette & 0x80 ? regs.priority1 : regs.priority0);
    palette &= 0x7f;
  }

  if(palette == 0) return;

  if(regs.main_enabled) {
    output.main.palette = palette;
    output.main.priority = priority;
  }

  if(regs.sub_enabled) {
    output.sub.palette = palette;
    output.sub.priority = priority;
  }
}

#endif
