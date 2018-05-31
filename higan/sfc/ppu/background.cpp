#include "mode7.cpp"
uint4 PPU::Background::Mosaic::size;

auto PPU::Background::hires() const -> bool {
  return ppu.io.bgMode == 5 || ppu.io.bgMode == 6;
}

auto PPU::Background::voffset() const -> uint16 {
  return mosaic.enable ? latch.voffset : io.voffset;
}

auto PPU::Background::hoffset() const -> uint16 {
  return mosaic.enable ? latch.hoffset : io.hoffset;
}

//V = 0, H = 0
auto PPU::Background::frame() -> void {
}

//H = 0
auto PPU::Background::scanline() -> void {
}

//H = 28
auto PPU::Background::begin() -> void {
  x = -7;
  y = ppu.vcounter();

  tileCounter = 7 - (io.hoffset & 7) << hires();
  for(auto& word : data) word = 0;

  if(y == 1) {
    mosaic.vcounter = mosaic.size + 1;
    mosaic.voffset = 1;
    latch.hoffset = io.hoffset;
    latch.voffset = io.voffset;
  } else if(--mosaic.vcounter == 0) {
    mosaic.vcounter = mosaic.size + 1;
    mosaic.voffset += mosaic.size + 1;
    latch.hoffset = io.hoffset;
    latch.voffset = io.voffset;
  }

  mosaic.hcounter = mosaic.size + 1;
  mosaic.hoffset = 0;

  if(io.mode == Mode::Mode7) return beginMode7();
  if(mosaic.size == 0) {
    latch.hoffset = io.hoffset;
    latch.voffset = io.voffset;
  }
}

auto PPU::Background::getTile() -> void {
  uint paletteOffset = ppu.io.bgMode == 0 ? id << 5 : 0;
  uint paletteSize = 2 << io.mode;
  uint tileMask = ppu.vram.mask >> 3 + io.mode;
  uint tiledataIndex = io.tiledataAddress >> 3 + io.mode;

  uint tileHeight = 3 + io.tileSize;
  uint tileWidth = !hires() ? tileHeight : 4;

  uint width = 256 << hires();

  uint hmask = (width << io.tileSize << io.screenSize.bit(0)) - 1;
  uint vmask = (width << io.tileSize << io.screenSize.bit(1)) - 1;

  uint px = x << hires();
  uint py = mosaic.enable ? (uint)mosaic.voffset : y;

  uint hscroll = hoffset();
  uint vscroll = voffset();
  if(hires()) {
    hscroll <<= 1;
    if(ppu.io.interlace) py = py << 1 | (ppu.field() & !mosaic.enable);  //todo: temporary vmosaic hack
  }

  uint hoffset = hscroll + px;
  uint voffset = vscroll + py;

  if(ppu.io.bgMode == 2 || ppu.io.bgMode == 4 || ppu.io.bgMode == 6) {
    uint16 offsetX = px + (hscroll & 7);

    if(offsetX >= 8) {
      auto hlookup = ppu.bg3.getTile((offsetX - 8) + (ppu.bg3.hoffset() & ~7), ppu.bg3.voffset() + 0);
      auto vlookup = ppu.bg3.getTile((offsetX - 8) + (ppu.bg3.hoffset() & ~7), ppu.bg3.voffset() + 8);
      uint valid = 13 + id;

      if(ppu.io.bgMode == 4) {
        if(hlookup.bit(valid)) {
          if(!hlookup.bit(15)) {
            hoffset = offsetX + (hlookup & ~7);
          } else {
            voffset = py + hlookup;
          }
        }
      } else {
        if(hlookup.bit(valid)) hoffset = offsetX + (hlookup & ~7);
        if(vlookup.bit(valid)) voffset = py + vlookup;
      }
    }
  }

  hoffset &= hmask;
  voffset &= vmask;

  uint screenX = io.screenSize.bit(0) ? 32 << 5 : 0;
  uint screenY = io.screenSize.bit(1) ? 32 << 5 + io.screenSize.bit(0) : 0;

  uint tileX = hoffset >> tileWidth;
  uint tileY = voffset >> tileHeight;

  uint16 offset = (tileY & 0x1f) << 5 | (tileX & 0x1f);
  if(tileX & 0x20) offset += screenX;
  if(tileY & 0x20) offset += screenY;

  uint16 address = io.screenAddress + offset;
  tile = ppu.vram[address];
  bool mirrorY = tile.bit(15);
  bool mirrorX = tile.bit(14);
  priority = io.priority[tile.bit(13)];
  paletteNumber = tile.bits(10,12);
  paletteIndex = paletteOffset + (paletteNumber << paletteSize);

  if(tileWidth  == 4 && (bool)(hoffset & 8) != mirrorX) tile +=  1;
  if(tileHeight == 4 && (bool)(voffset & 8) != mirrorY) tile += 16;
  uint16 character = tile.bits(0,9) + tiledataIndex & tileMask;

  if(mirrorY) voffset ^= 7;
  offset = (character << 3 + io.mode) + (voffset & 7);

  switch(io.mode) {
  case Mode::BPP8:
    data[1].bits(16,31) = ppu.vram[offset + 24];
    data[1].bits( 0,15) = ppu.vram[offset + 16];
  case Mode::BPP4:
    data[0].bits(16,31) = ppu.vram[offset +  8];
  case Mode::BPP2:
    data[0].bits( 0,15) = ppu.vram[offset +  0];
  }

  if(mirrorX) for(auto n : range(2)) {
    data[n] = (data[n] >> 4 & 0x0f0f0f0f) | (data[n] << 4 & 0xf0f0f0f0);
    data[n] = (data[n] >> 2 & 0x33333333) | (data[n] << 2 & 0xcccccccc);
    data[n] = (data[n] >> 1 & 0x55555555) | (data[n] << 1 & 0xaaaaaaaa);
  }
}

auto PPU::Background::run(bool screen) -> void {
  if(ppu.vcounter() == 0) return;

  if(screen == Screen::Below) {
    output.above.priority = 0;
    output.below.priority = 0;
    if(!hires()) return;
  }

  if(tileCounter-- == 0) {
    tileCounter = 7;
    getTile();
  }

  if(io.mode == Mode::Mode7) return runMode7();

  uint8 color = getTileColor();
  Pixel pixel;
  pixel.priority = priority;
  pixel.palette = color ? paletteIndex + color : 0;
  pixel.tile = tile;

  if(x == 0) {
    mosaic.hcounter = mosaic.size + 1;
    mosaic.pixel = pixel;
  } else if((!hires() || screen == Screen::Below) && --mosaic.hcounter == 0) {
    mosaic.hcounter = mosaic.size + 1;
    mosaic.pixel = pixel;
  } else if(mosaic.enable) {
    pixel = mosaic.pixel;
  }
  if(screen == Screen::Above) x++;
  if(pixel.palette == 0) return;

  if(!hires() || screen == Screen::Above) if(io.aboveEnable) output.above = pixel;
  if(!hires() || screen == Screen::Below) if(io.belowEnable) output.below = pixel;
}

auto PPU::Background::getTileColor() -> uint {
  uint color = 0;

  switch(io.mode) {
  case Mode::BPP8:
    color += data[1] >> 24 & 0x80;
    color += data[1] >> 17 & 0x40;
    color += data[1] >> 10 & 0x20;
    color += data[1] >>  3 & 0x10;
    data[1] <<= 1;
  case Mode::BPP4:
    color += data[0] >> 28 & 0x08;
    color += data[0] >> 21 & 0x04;
  case Mode::BPP2:
    color += data[0] >> 14 & 0x02;
    color += data[0] >>  7 & 0x01;
    data[0] <<= 1;
  }

  return color;
}

auto PPU::Background::power() -> void {
  io = {};
  io.tiledataAddress = (random() & 0x0f) << 12;
  io.screenAddress = (random() & 0xfc) << 8;
  io.screenSize = random();
  io.tileSize = random();
  io.aboveEnable = random();
  io.belowEnable = random();
  io.hoffset = random();
  io.voffset = random();

  latch = {};

  output.above = {};
  output.below = {};

  mosaic = {};
  mosaic.size = random();
  mosaic.enable = random();

  x = 0;
  y = 0;

  tileCounter = 0;
  tile = 0;
  priority = 0;
  paletteNumber = 0;
  paletteIndex = 0;
  for(auto& word : data) word = 0;
}

auto PPU::Background::getTile(uint x, uint y) -> uint16 {
  uint tileHeight = 3 + io.tileSize;
  uint tileWidth = !hires() ? tileHeight : 4;
  uint screenX = io.screenSize.bit(0) ? 32 << 5 : 0;
  uint screenY = io.screenSize.bit(1) ? 32 << 5 + io.screenSize.bit(0) : 0;
  uint tileX = x >> tileWidth;
  uint tileY = y >> tileHeight;
  uint16 offset = (tileY & 0x1f) << 5 | (tileX & 0x1f);
  if(tileX & 0x20) offset += screenX;
  if(tileY & 0x20) offset += screenY;
  uint16 address = io.screenAddress + offset;
  return ppu.vram[address];
}
