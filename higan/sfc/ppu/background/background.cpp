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
  uint colorDepth = io.mode == Mode::BPP2 ? 0 : io.mode == Mode::BPP4 ? 1 : 2;
  uint paletteOffset = ppu.io.bgMode == 0 ? id << 5 : 0;
  uint paletteSize = 2 << colorDepth;
  uint tileMask = ppu.vram.mask >> 3 + colorDepth;
  uint tiledataIndex = io.tiledataAddress >> 3 + colorDepth;

  uint tileHeight = io.tileSize == TileSize::Size8x8 ? 3 : 4;
  uint tileWidth = !hires() ? tileHeight : 4;

  uint width = 256 << hires();

  uint hmask = tileHeight == 3 ? width : width << 1;
  uint vmask = hmask;
  if(io.screenSize & 1) hmask <<= 1;
  if(io.screenSize & 2) vmask <<= 1;
  hmask--;
  vmask--;

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
    uint16 offsetX = x + (hscroll & 7);

    if(offsetX >= 8) {
      uint hval = ppu.bg3.getTile((offsetX - 8) + (ppu.bg3.hoffset() & ~7), ppu.bg3.voffset() + 0);
      uint vval = ppu.bg3.getTile((offsetX - 8) + (ppu.bg3.hoffset() & ~7), ppu.bg3.voffset() + 8);
      uint validMask = id == ID::BG1 ? 0x2000 : 0x4000;

      if(ppu.io.bgMode == 4) {
        if(hval & validMask) {
          if((hval & 0x8000) == 0) {
            hoffset = offsetX + (hval & ~7);
          } else {
            voffset = y + hval;
          }
        }
      } else {
        if(hval & validMask) hoffset = offsetX + (hval & ~7);
        if(vval & validMask) voffset = y + vval;
      }
    }
  }

  hoffset &= hmask;
  voffset &= vmask;

  uint screenX = io.screenSize & 1 ? 32 << 5 : 0;
  uint screenY = io.screenSize & 2 ? 32 << 5 : 0;
  if(io.screenSize == 3) screenY <<= 1;

  uint tx = hoffset >> tileWidth;
  uint ty = voffset >> tileHeight;

  uint16 offset = (ty & 0x1f) << 5 | (tx & 0x1f);
  if(tx & 0x20) offset += screenX;
  if(ty & 0x20) offset += screenY;

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
  offset = (character << 3 + colorDepth) + (voffset & 7);

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
    mosaic.hcounter = 1;
    mosaic.pixel = pixel;
  } else if(x >= 1 && screen == Screen::Above && --mosaic.hcounter == 0) {
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

auto PPU::Background::getTile(uint x, uint y) -> uint {
  uint tileHeight = io.tileSize == TileSize::Size8x8 ? 3 : 4;
  uint tileWidth = !hires() ? tileHeight : 4;
  uint width = !hires() ? 256 : 512;
  uint maskX = tileHeight == 3 ? width : width << 1;
  uint maskY = maskX;
  if(io.screenSize & 1) maskX <<= 1;
  if(io.screenSize & 2) maskY <<= 1;
  maskX--;
  maskY--;

  uint screenX = io.screenSize & 1 ? 32 << 5 : 0;
  uint screenY = io.screenSize & 2 ? 32 << 5 : 0;
  if(io.screenSize == 3) screenY <<= 1;

  x = (x & maskX) >> tileWidth;
  y = (y & maskY) >> tileHeight;

  uint16 offset = (y & 0x1f) << 5 | (x & 0x1f);
  if(x & 0x20) offset += screenX;
  if(y & 0x20) offset += screenY;

  uint16 address = io.screenAddress + offset;
  return ppu.vram[address];
}
