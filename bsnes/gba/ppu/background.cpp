void PPU::render_backgrounds() {
  if(regs.control.bgmode == 0) {
    render_background_linear(0);
    render_background_linear(1);
    render_background_linear(2);
    render_background_linear(3);
  }

  if(regs.control.bgmode == 1) {
    render_background_linear(0);
    render_background_linear(1);
  //render_background_affine(2);
  }

  if(regs.control.bgmode == 2) {
  //render_background_affine(2);
  //render_background_affine(3);
  }
}

void PPU::render_background_linear(unsigned bgnumber) {
  for(unsigned n = 0; n < 240; n++) pixel[bgnumber][n].exists = false;
  if(regs.control.enablebg[bgnumber] == false) return;

  auto &bg = regs.bg[bgnumber];
  uint9 voffset = regs.vcounter + bg.voffset;
  uint9 hoffset = bg.hoffset;

  unsigned basemap = bg.control.screenbaseblock    << 11;
  unsigned basechr = bg.control.characterbaseblock << 14;
  unsigned px = hoffset & 7, py = voffset & 7;

  Tile tile;
  uint8 data[8];

  for(unsigned x = 0; x < 240; x++) {
    if(x == 0 || px & 8) {
      px &= 7;

      unsigned tx = hoffset / 8, ty = voffset / 8;
      unsigned offset = (ty & 31) * 32 + (tx & 31);
      if(bg.control.screensize & 1) if(tx & 32) offset += 32 * 32;
      if(bg.control.screensize & 2) if(ty & 32) offset += 32 * 32 * (1 + (bg.control.screensize & 1));
      offset = basemap + offset * 2;
      uint16 mapdata = vram.read(offset, Half);

      tile.character = mapdata >>  0;
      tile.hflip     = mapdata >> 10;
      tile.vflip     = mapdata >> 11;
      tile.palette   = mapdata >> 12;

      if(bg.control.colormode == 0) {
        offset = basechr + tile.character * 32 + (py ^ (tile.vflip ? 7 : 0)) * 4;
        uint32 word = vram.read(offset, Word);
        for(unsigned n = 0; n < 8; n++) data[n] = (word >> (n * 4)) & 15;
      } else {
        offset = basechr + tile.character * 64 + (py ^ (tile.vflip ? 7 : 0)) * 8;
        uint32 wordlo = vram.read(offset + 0, Word);
        uint32 wordhi = vram.read(offset + 4, Word);
        for(unsigned n = 0; n < 4; n++) data[0 + n] = (wordlo >> (n * 8)) & 255;
        for(unsigned n = 0; n < 4; n++) data[4 + n] = (wordhi >> (n * 8)) & 255;
      }
    }

    hoffset++;
    uint8 color = data[px++ ^ (tile.hflip ? 7 : 0)];
    if(color == 0) continue;  //transparent

    if(bg.control.colormode == 0) pixel[bgnumber][x] = { true, palette(tile.palette * 16 + color), bg.control.priority };
    if(bg.control.colormode == 1) pixel[bgnumber][x] = { true, palette(color), bg.control.priority };
  }
}
