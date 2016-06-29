auto PPU::renderBackgrounds() -> void {
  switch(regs.control.bgmode) {
  case 0:
    renderBackgroundLinear(regs.bg[3]);
    renderBackgroundLinear(regs.bg[2]);
    renderBackgroundLinear(regs.bg[1]);
    renderBackgroundLinear(regs.bg[0]);
    break;
  case 1:
    renderBackgroundAffine(regs.bg[2]);
    renderBackgroundLinear(regs.bg[1]);
    renderBackgroundLinear(regs.bg[0]);
    break;
  case 2:
    renderBackgroundAffine(regs.bg[3]);
    renderBackgroundAffine(regs.bg[2]);
    break;
  case 3: case 4: case 5:
    renderBackgroundBitmap(regs.bg[2]);
    break;
  }
}

auto PPU::renderBackgroundLinear(Registers::Background& bg) -> void {
  if(regs.control.enable[bg.id] == false) return;
  auto& output = layer[bg.id];

  if(bg.control.mosaic == false || (regs.vcounter % (1 + regs.mosaic.bgvsize)) == 0) {
    bg.vmosaic = regs.vcounter;
  }

  uint9 voffset = bg.vmosaic + bg.voffset;
  uint9 hoffset = bg.hoffset;

  uint basemap = bg.control.screenbaseblock    << 11;
  uint basechr = bg.control.characterbaseblock << 14;
  uint px = hoffset & 7, py = voffset & 7;

  Tile tile;
  uint8 data[8];

  for(auto x : range(240)) {
    if(x == 0 || px & 8) {
      px &= 7;

      uint tx = hoffset / 8, ty = voffset / 8;
      uint offset = (ty & 31) * 32 + (tx & 31);
      if(bg.control.screensize & 1) if(tx & 32) offset += 32 * 32;
      if(bg.control.screensize & 2) if(ty & 32) offset += 32 * 32 * (1 + (bg.control.screensize & 1));
      offset = basemap + offset * 2;
      uint16 mapdata = readVRAM(Half, offset);

      tile.character = mapdata >>  0;
      tile.hflip     = mapdata >> 10;
      tile.vflip     = mapdata >> 11;
      tile.palette   = mapdata >> 12;

      if(bg.control.colormode == 0) {
        offset = basechr + tile.character * 32 + (py ^ (tile.vflip ? 7 : 0)) * 4;
        uint32 word = readVRAM(Word, offset);
        for(auto n : range(8)) data[n] = (word >> (n * 4)) & 15;
      } else {
        offset = basechr + tile.character * 64 + (py ^ (tile.vflip ? 7 : 0)) * 8;
        uint32 wordlo = readVRAM(Word, offset + 0);
        uint32 wordhi = readVRAM(Word, offset + 4);
        for(auto n : range(4)) data[0 + n] = (wordlo >> (n * 8)) & 255;
        for(auto n : range(4)) data[4 + n] = (wordhi >> (n * 8)) & 255;
      }
    }

    hoffset++;
    uint8 color = data[px++ ^ (tile.hflip ? 7 : 0)];

    if(color) {
      if(bg.control.colormode == 0) output[x].write(true, bg.control.priority, pram[tile.palette * 16 + color]);
      if(bg.control.colormode == 1) output[x].write(true, bg.control.priority, pram[color]);
    }
  }
}

auto PPU::renderBackgroundAffine(Registers::Background& bg) -> void {
  if(regs.control.enable[bg.id] == false) return;
  auto& output = layer[bg.id];

  uint basemap = bg.control.screenbaseblock    << 11;
  uint basechr = bg.control.characterbaseblock << 14;
  uint screensize = 16 << bg.control.screensize;
  uint screenwrap = (1 << (bg.control.affinewrap ? 7 + bg.control.screensize : 20)) - 1;

  if(bg.control.mosaic == false || (regs.vcounter % (1 + regs.mosaic.bgvsize)) == 0) {
    bg.hmosaic = bg.lx;
    bg.vmosaic = bg.ly;
  }

  int28 fx = bg.hmosaic;
  int28 fy = bg.vmosaic;

  for(auto x : range(240)) {
    uint cx = (fx >> 8) & screenwrap, tx = cx / 8, px = cx & 7;
    uint cy = (fy >> 8) & screenwrap, ty = cy / 8, py = cy & 7;

    if(tx < screensize && ty < screensize) {
      uint8 character = vram[basemap + ty * screensize + tx];
      uint8 color = vram[basechr + (character * 64) + py * 8 + px];
      if(color) output[x].write(true, bg.control.priority, pram[color]);
    }

    fx += bg.pa;
    fy += bg.pc;
  }

  bg.lx += bg.pb;
  bg.ly += bg.pd;
}

auto PPU::renderBackgroundBitmap(Registers::Background& bg) -> void {
  if(regs.control.enable[bg.id] == false) return;
  auto& output = layer[bg.id];

  uint1 depth = regs.control.bgmode != 4;  //0 = 8-bit (Mode 4), 1 = 15-bit (Mode 3, Mode 5)
  uint basemap = regs.control.bgmode == 3 ? 0 : 0xa000 * regs.control.frame;

  uint width  = regs.control.bgmode == 5 ? 160 : 240;
  uint height = regs.control.bgmode == 5 ? 128 : 160;
  uint mode   = depth ? Half : Byte;

  if(bg.control.mosaic == false || (regs.vcounter % (1 + regs.mosaic.bgvsize)) == 0) {
    bg.hmosaic = bg.lx;
    bg.vmosaic = bg.ly;
  }

  int28 fx = bg.hmosaic;
  int28 fy = bg.vmosaic;

  for(auto x : range(240)) {
    uint px = fx >> 8;
    uint py = fy >> 8;

    if(px < width && py < height) {
      uint offset = py * width + px;
      uint color  = readVRAM(mode, basemap + (offset << depth));

      if(depth || color) {  //8bpp color 0 is transparent; 15bpp color is always opaque
        if(depth == 0) color = pram[color];
        if(depth == 1) color = color & 0x7fff;
        output[x].write(true, bg.control.priority, color);
      }
    }

    fx += bg.pa;
    fy += bg.pc;
  }

  bg.lx += bg.pb;
  bg.ly += bg.pd;
}
