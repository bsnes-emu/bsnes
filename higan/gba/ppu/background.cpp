uint3 PPU::Background::IO::mode;
uint1 PPU::Background::IO::frame;
uint5 PPU::Background::IO::mosaicWidth;
uint5 PPU::Background::IO::mosaicHeight;

auto PPU::Background::scanline(uint y) -> void {
}

auto PPU::Background::run(uint x, uint y) -> void {
  output = {};
  if(ppu.blank() || !io.enable) return;

  switch(id) {
  case PPU::BG0:
    if(io.mode <= 1) return linear(x, y);
    break;

  case PPU::BG1:
    if(io.mode <= 1) return linear(x, y);
    break;

  case PPU::BG2:
    if(io.mode == 0) return linear(x, y);
    if(io.mode <= 2) return affine(x, y);
    if(io.mode <= 5) return bitmap(x, y);
    break;

  case PPU::BG3:
    if(io.mode == 0) return linear(x, y);
    if(io.mode == 2) return affine(x, y);
    break;
  }
}

/*
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
*/

auto PPU::Background::linear(uint x, uint y) -> void {
  if(x == 0) {
    if(!io.mosaic || (y % (1 + io.mosaicHeight)) == 0) {
      vmosaic = y;
    }

    voffset = vmosaic + io.voffset;
    hoffset = io.hoffset;
  }

  uint px = hoffset & 7;
  uint py = voffset & 7;

  uint tx = hoffset >> 3;
  uint ty = voffset >> 3;

  uint offset = (ty & 31) * 32 + (tx & 31);
  if(io.screenSize.bit(0) && (tx & 32)) offset += 32 * 32;
  if(io.screenSize.bit(1) && (ty & 32)) offset += 32 * 32 * (1 + (io.screenSize.bit(0)));
  offset = (io.screenBase << 11) + offset * 2;

  uint16 tilemap = ppu.readVRAM(Half, offset);
  uint10 character = tilemap.bits( 0, 9);
  uint1  hflip     = tilemap.bit (10);
  uint1  vflip     = tilemap.bit (11);
  uint4  palette   = tilemap.bits(12,15);

  if(io.colorMode == 0) {
    offset  = (io.characterBase << 14) + character * 32;
    offset += (py ^ (vflip ? 7 : 0)) * 4;
    offset += (px ^ (hflip ? 7 : 0)) / 2;
    if(uint4 color = ppu.readVRAM(Byte, offset) >> (px & 1 ? 4 : 0)) {
      output.enable = true;
      output.priority = io.priority;
      output.color = ppu.pram[palette * 16 + color];
    }
  } else {
    offset  = (io.characterBase << 14) + character * 64;
    offset += (py ^ (vflip ? 7 : 0)) * 8;
    offset += (px ^ (hflip ? 7 : 0)) / 1;
    if(uint8 color = ppu.readVRAM(Byte, offset)) {
      output.enable = true;
      output.priority = io.priority;
      output.color = ppu.pram[color];
    }
  }

  hoffset++;
}

/*
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
*/

auto PPU::Background::affine(uint x, uint y) -> void {
  if(x == 0) {
    if(!io.mosaic || (y % (1 + io.mosaicHeight)) == 0) {
      hmosaic = io.lx;
      vmosaic = io.ly;
    }

    fx = hmosaic;
    fy = vmosaic;
  }

  uint screenSize = 16 << io.screenSize;
  uint screenWrap = (1 << (io.affineWrap ? 7 + io.screenSize : 20)) - 1;

  uint cx = (fx >> 8) & screenWrap, tx = cx >> 3, px = cx & 7;
  uint cy = (fy >> 8) & screenWrap, ty = cy >> 3, py = cy & 7;

  if(tx < screenSize && ty < screenSize) {
    uint8 character = ppu.vram[(io.screenBase << 11) + ty * screenSize + tx];
    if(uint8 color = ppu.vram[(io.characterBase << 14) + character * 64 + py * 8 + px]) {
      output.enable = true;
      output.priority = io.priority;
      output.color = ppu.pram[color];
    }
  }

  fx += io.pa;
  fy += io.pc;

  if(x == 239) {
    io.lx += io.pb;
    io.ly += io.pd;
  }
}

/*
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
*/

auto PPU::Background::bitmap(uint x, uint y) -> void {
  if(x == 0) {
    if(!io.mosaic || (y % (1 + io.mosaicHeight)) == 0) {
      hmosaic = io.lx;
      vmosaic = io.ly;
    }

    fx = hmosaic;
    fy = vmosaic;
  }

  uint1 depth = io.mode != 4;  //0 = 8-bit (mode 4); 1 = 15-bit (mode 3, mode 5)
  uint width  = io.mode == 5 ? 160 : 240;
  uint height = io.mode == 5 ? 128 : 160;
  uint mode   = depth ? Half : Byte;

  uint baseAddress = io.mode == 3 ? 0 : 0xa000 * io.frame;

  uint px = fx >> 8;
  uint py = fy >> 8;

  if(px < width && py < height) {
    uint offset = py * width + px;
    uint15 color = ppu.readVRAM(mode, baseAddress + (offset << depth));

    if(depth || color) {  //8bpp color 0 is transparent; 15bpp color is always opaque
      if(depth == 0) color = ppu.pram[color];
      output.enable = true;
      output.priority = io.priority;
      output.color = color;
    }
  }

  fx += io.pa;
  fy += io.pc;

  if(x == 239) {
    io.lx += io.pb;
    io.ly += io.pd;
  }
}

auto PPU::Background::power(uint id) -> void {
  this->id = id;

  memory::fill(&io, sizeof(IO));
}
