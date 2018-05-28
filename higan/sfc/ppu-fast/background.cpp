auto PPU::Line::renderBackground(PPU::IO::Background& self, uint source) -> void {
  if(io.displayDisable) return;
  if(self.tileMode == TileMode::Inactive) return;
  if(self.tileMode == TileMode::Mode7) return;  //todo
  if(!self.aboveEnable && !self.belowEnable) return;

  bool hires = io.bgMode == 5 || io.bgMode == 6;
  bool offsetPerTile = io.bgMode == 2 || io.bgMode == 4 || io.bgMode == 6;
  bool directColor = io.col.directColor && source == Source::BG1 && (io.bgMode == 3 || io.bgMode == 4);
  uint width = !hires ? 256 : 512;

  uint tileHeight = self.tileSize ? 4 : 3;
  uint tileWidth = hires ? 4 : tileHeight;
  uint tileMask = 0x0fff >> self.tileMode;
  uint tiledataIndex = self.tiledataAddress >> 4 + self.tileMode;

  uint maskX = width << (tileHeight == 4);
  uint maskY = maskX;
  if(self.screenSize.bit(0)) maskX <<= 1;
  if(self.screenSize.bit(1)) maskY <<= 1;
  maskX--;
  maskY--;

  uint screenX = self.screenSize.bit(0) ? 32 << 5 : 0;
  uint screenY = self.screenSize.bit(1) ? 32 << 5 : 0;
  if(self.screenSize == 3) screenY <<= 1;

  uint paletteBase = io.bgMode == 0 ? source << 5 : 0;
  uint paletteShift = 2 << self.tileMode;

  uint hscroll = self.hoffset;
  uint vscroll = self.voffset;

  uint y = this->y;  //todo: vmosaic
  if(hires) {
    hscroll <<= 1;
    if(io.interlace) y = y << 1 | ppu.PPUcounter::field();
  }

  uint mosaicCounter = 1;
  uint mosaicPalette = 0;
  uint mosaicPriority = 0;
  uint mosaicColor = 0;

  auto getTile = [&](uint hoffset, uint voffset) -> uint {
    uint tileX = (hoffset & maskX) >> tileWidth;
    uint tileY = (voffset & maskY) >> tileHeight;
    uint tilePosition = (tileY & 0x1f) << 5 | (tileX & 0x1f);
    if(tileX & 0x20) tilePosition += screenX;
    if(tileY & 0x20) tilePosition += screenY;
    uint15 tiledataAddress = self.screenAddress + tilePosition;
    return ppu.vram[tiledataAddress];
  };

  int x = 0 - (hscroll & 7);
  while(x < width) {
    uint hoffset = x + hscroll;
    uint voffset = y + vscroll;
    if(offsetPerTile);  //todo
    hoffset &= maskX;
    voffset &= maskY;

    uint tileNumber = getTile(hoffset, voffset);
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
        if(directColor) {
          //todo
        } else {
          mosaicColor = cgram[paletteIndex + mosaicPalette];
        }
      }
      if(!mosaicPalette) continue;

      if(!hires) {
        if(self.aboveEnable) {  //todo: window
          plotAbove(x, source, mosaicPriority, mosaicColor);
        }
        if(self.belowEnable) {  //todo: window
          plotBelow(x, source, mosaicPriority, mosaicColor);
        }
      } else {
        //todo
      }
    }
  }
}
