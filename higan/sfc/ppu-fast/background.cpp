auto PPU::Line::renderBackground(PPU::IO::Background& self, uint source) -> void {
  if(io.displayDisable) return;
  if(!self.aboveEnable && !self.belowEnable) return;
  if(self.tileMode == TileMode::Mode7) return renderMode7(self, source);
  if(self.tileMode == TileMode::Inactive) return;

  bool windowAbove[256];
  bool windowBelow[256];
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  bool hires = io.bgMode == 5 || io.bgMode == 6;
  bool offsetPerTileMode = io.bgMode == 2 || io.bgMode == 4 || io.bgMode == 6;
  bool directColorMode = io.col.directColor && source == Source::BG1 && (io.bgMode == 3 || io.bgMode == 4);
  int width = !hires ? 256 : 512;

  uint tileHeight = self.tileSize ? 4 : 3;
  uint tileWidth = hires ? 4 : tileHeight;
  uint tileMask = 0x0fff >> self.tileMode;
  uint tiledataIndex = self.tiledataAddress >> 3 + self.tileMode;

  uint paletteBase = io.bgMode == 0 ? source << 5 : 0;
  uint paletteShift = 2 << self.tileMode;

  uint hscroll = self.hoffset;
  uint vscroll = self.voffset;
  uint hmask = (width << (tileHeight == 4) << self.screenSize.bit(0)) - 1;
  uint vmask = (width << (tileHeight == 4) << self.screenSize.bit(1)) - 1;

  uint y = this->y;  //todo: vmosaic
  if(hires) {
    hscroll <<= 1;
    if(io.interlace) y = y << 1 | ppu.PPUcounter::field();
  }

  uint mosaicCounter = 1;
  uint mosaicPalette = 0;
  uint mosaicPriority = 0;
  uint mosaicColor = 0;

  int x = 0 - (hscroll & 7);
  while(x < width) {
    uint hoffset = x + hscroll;
    uint voffset = y + vscroll;
    if(offsetPerTileMode) {
      uint validBit = source == Source::BG1 ? 0x2000 : source == Source::BG2 ? 0x4000 : 0x0000;
      uint offsetX = x + (hscroll & 7);
      if(offsetX >= 8) {  //first column is exempt
        uint hvalue = getTile(io.bg3, (offsetX - 8) + (io.bg3.hoffset & ~7), io.bg3.voffset + 0);
        if(io.bgMode == 4) {
          if(hvalue & validBit) {
            if(!(hvalue & 0x8000)) {
              hoffset = offsetX + (hvalue & ~7);
            } else {
              voffset = y + hvalue;
            }
          }
        } else {
          uint vvalue = getTile(io.bg3, (offsetX - 8) + (io.bg3.hoffset & ~7), io.bg3.voffset + 8);
          if(hvalue & validBit) {
            hoffset = offsetX + (hvalue & ~7);
          }
          if(vvalue & validBit) {
            voffset = y + vvalue;
          }
        }
      }
    }
    hoffset &= hmask;
    voffset &= vmask;

    uint tileNumber = getTile(self, hoffset, voffset);
    uint mirrorY = tileNumber & 0x8000 ? 7 : 0;
    uint mirrorX = tileNumber & 0x4000 ? 7 : 0;
    uint tilePriority = tileNumber & 0x2000 ? self.priority[1] : self.priority[0];
    uint paletteNumber = tileNumber >> 10 & 7;
    uint paletteIndex = (paletteBase + (paletteNumber << paletteShift)) & 0xff;

    if(tileWidth  == 4 && (hoffset & 8) - 1 != mirrorX) tileNumber +=  1;
    if(tileHeight == 4 && (voffset & 8) - 1 != mirrorY) tileNumber += 16;
    tileNumber = ((tileNumber & 0x03ff) + tiledataIndex) & tileMask;

    auto tiledata = ppu.tilecache[self.tileMode] + (tileNumber << 6);
    tiledata += ((voffset & 7) ^ mirrorY) << 3;

    for(uint tileX = 0; tileX < 8; tileX++, x++) {
      if(x & width) continue;  //x < 0 || x >= width
      if(--mosaicCounter == 0) {
        mosaicCounter = 1 + io.mosaicSize;
        mosaicPalette = tiledata[tileX ^ mirrorX];
        mosaicPriority = tilePriority;
        if(directColorMode) {
          mosaicColor = directColor(paletteNumber, mosaicPalette);
        } else {
          mosaicColor = cgram[paletteIndex + mosaicPalette];
        }
      }
      if(!mosaicPalette) continue;

      if(!hires) {
        if(self.aboveEnable && !windowAbove[x]) plotAbove(x, source, mosaicPriority, mosaicColor);
        if(self.belowEnable && !windowBelow[x]) plotBelow(x, source, mosaicPriority, mosaicColor);
      } else {
        uint X = x >> 1;
        if(x & 1) {
          if(self.aboveEnable && !windowAbove[X]) plotAbove(X, source, mosaicPriority, mosaicColor);
        } else {
          if(self.belowEnable && !windowBelow[X]) plotBelow(X, source, mosaicPriority, mosaicColor);
        }
      }
    }
  }
}

auto PPU::Line::getTile(PPU::IO::Background& self, uint hoffset, uint voffset) -> uint {
  bool hires = io.bgMode == 5 || io.bgMode == 6;
  uint width = !hires ? 256 : 512;
  uint tileHeight = self.tileSize ? 4 : 3;
  uint tileWidth = hires ? 4 : tileHeight;
  uint hmask = (width << (tileHeight == 4) << self.screenSize.bit(0)) - 1;
  uint vmask = (width << (tileHeight == 4) << self.screenSize.bit(1)) - 1;
  uint screenX = self.screenSize.bit(0) ? 32 << 5 : 0;
  uint screenY = self.screenSize.bit(1) ? screenX + (32 << 5) : 0;
  uint tileX = (hoffset & hmask) >> tileWidth;
  uint tileY = (voffset & vmask) >> tileHeight;
  uint tilePosition = (tileY & 0x1f) << 5 | (tileX & 0x1f);
  if(tileX & 0x20) tilePosition += screenX;
  if(tileY & 0x20) tilePosition += screenY;
  uint15 tiledataAddress = self.screenAddress + tilePosition;
  return ppu.vram[tiledataAddress];
}
