auto PPU::Line::renderMode7(PPU::IO::Background& self, uint source) -> void {
  int Y = this->y - self.mosaicEnable ? this->y % (1 + io.mosaicSize) : 0;
  int y = !io.mode7.vflip ? Y : 255 - Y;

  int a = (int16)io.mode7.a;
  int b = (int16)io.mode7.b;
  int c = (int16)io.mode7.c;
  int d = (int16)io.mode7.d;
  int hcenter = (int13)io.mode7.x;
  int vcenter = (int13)io.mode7.y;
  int hoffset = (int13)io.mode7.hoffset;
  int voffset = (int13)io.mode7.voffset;

  uint mosaicCounter = 1;
  uint mosaicPalette = 0;
  uint mosaicPriority = 0;
  uint mosaicColor = 0;

  auto clip = [](int n) -> int { return n & 0x2000 ? (n | ~1023) : (n & 1023); };
  int originX = (a * clip(hoffset - hcenter) & ~63) + (b * clip(voffset - vcenter) & ~63) + (b * y & ~63) + (hcenter << 8);
  int originY = (c * clip(hoffset - hcenter) & ~63) + (d * clip(voffset - vcenter) & ~63) + (d * y & ~63) + (vcenter << 8);

  array<bool[256]> windowAbove;
  array<bool[256]> windowBelow;
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  if(!configuration.hacks.ppuFast.hiresMode7) {
    for(int X : range(256)) {
      int x = !io.mode7.hflip ? X : 255 - X;
      int pixelX = originX + a * x >> 8;
      int pixelY = originY + c * x >> 8;
      int tileX = pixelX >> 3 & 127;
      int tileY = pixelY >> 3 & 127;
      bool outOfBounds = (pixelX | pixelY) & ~1023;
      uint15 tileAddress = tileY * 128 + tileX;
      uint15 paletteAddress = ((pixelY & 7) << 3) + (pixelX & 7);
      uint8 tile = io.mode7.repeat == 3 && outOfBounds ? 0 : ppu.vram[tileAddress].byte(0);
      uint8 palette = io.mode7.repeat == 2 && outOfBounds ? 0 : ppu.vram[paletteAddress + (tile << 6)].byte(1);

      uint priority;
      if(source == Source::BG1) {
        priority = self.priority[0];
      } else if(source == Source::BG2) {
        priority = self.priority[palette >> 7];
        palette &= 0x7f;
      }

      if(!self.mosaicEnable || --mosaicCounter == 0) {
        mosaicCounter = 1 + io.mosaicSize;
        mosaicPalette = palette;
        mosaicPriority = priority;
        if(io.col.directColor && source == Source::BG1) {
          mosaicColor = directColor(0, palette);
        } else {
          mosaicColor = cgram[palette];
        }
      }
      if(!mosaicPalette) continue;

      if(self.aboveEnable && !windowAbove[X]) plotAbove(X, source, mosaicPriority, mosaicColor);
      if(self.belowEnable && !windowBelow[X]) plotBelow(X, source, mosaicPriority, mosaicColor);
    }
  } else {
    //emulator enhancement option: render 512 pixels instead of 256 pixels of horizontal resolution
    //note: this mode is impossible on real hardware due to needing 512 above+below pixels
    for(int X : range(512)) {
      int x = !io.mode7.hflip ? X : 511 - X;
      int pixelX = 2 * originX + a * x >> 9;
      int pixelY = 2 * originY + c * x >> 9;
      int tileX = pixelX >> 3 & 127;
      int tileY = pixelY >> 3 & 127;
      bool outOfBounds = (pixelX | pixelY) & ~1023;
      uint15 tileAddress = tileY * 128 + tileX;
      uint15 paletteAddress = ((pixelY & 7) << 3) + (pixelX & 7);
      uint8 tile = io.mode7.repeat == 3 && outOfBounds ? 0 : ppu.vram[tileAddress].byte(0);
      uint8 palette = io.mode7.repeat == 2 && outOfBounds ? 0 : ppu.vram[paletteAddress + (tile << 6)].byte(1);

      uint priority;
      if(source == Source::BG1) {
        priority = self.priority[0];
      } else if(source == Source::BG2) {
        priority = self.priority[palette >> 7];
        palette &= 0x7f;
      }

      if(!self.mosaicEnable || !io.mosaicSize || --mosaicCounter == 0) {
        mosaicCounter = (1 + io.mosaicSize) << 1;
        mosaicPalette = palette;
        mosaicPriority = priority;
        if(io.col.directColor && source == Source::BG1) {
          mosaicColor = directColor(0, palette);
        } else {
          mosaicColor = cgram[palette];
        }
      }
      if(!mosaicPalette) continue;

      if(self.aboveEnable && !windowAbove[X >> 1]) plotAbove(X >> 1 | (X & 1 ? 256 : 0), source, mosaicPriority, mosaicColor);
      if(self.belowEnable && !windowBelow[X >> 1]) plotBelow(X >> 1 | (X & 1 ? 256 : 0), source, mosaicPriority, mosaicColor);
    }
  }
}
