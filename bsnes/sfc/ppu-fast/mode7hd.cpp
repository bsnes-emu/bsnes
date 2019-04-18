auto PPUfast::Line::renderMode7HD(PPUfast::IO::Background& self, uint source) -> void {
  const bool extbg = source == Source::BG2;
  const uint scale = ppufast.hdScale();

  Pixel  pixel;
  Pixel* above = &this->above[-1];
  Pixel* below = &this->below[-1];

  //find the first and last scanline for interpolation
  int y_a = y;
  int y_b = y;
  #define isLineMode7(n) (ppufast.lines[n].io.bg1.tileMode == TileMode::Mode7 && ( \
    (ppufast.lines[n].io.bg1.aboveEnable || ppufast.lines[n].io.bg1.belowEnable) \
  ))
  if(ppufast.hdPerspective()) {
    while(y_a >   1 && isLineMode7(y_a)) y_a--; y_a += 1;
    while(y_b < 239 && isLineMode7(y_b)) y_b++; y_b -= 8;
  } else {
    if(y_a >   1 && isLineMode7(y_a)) y_a--;
    if(y_b < 239 && isLineMode7(y_b)) y_b++;
  }
  #undef isLineMode7

  int Y = y - (self.mosaicEnable ? y % (1 + io.mosaicSize) : 0);
  float yt = !io.mode7.vflip ? Y : 255 - Y;

  Line line_a = ppufast.lines[y_a];
  float a_a = (int16)line_a.io.mode7.a;
  float b_a = (int16)line_a.io.mode7.b;
  float c_a = (int16)line_a.io.mode7.c;
  float d_a = (int16)line_a.io.mode7.d;

  Line line_b = ppufast.lines[y_b];
  float a_b = (int16)line_b.io.mode7.a;
  float b_b = (int16)line_b.io.mode7.b;
  float c_b = (int16)line_b.io.mode7.c;
  float d_b = (int16)line_b.io.mode7.d;

  int hcenter = (int13)io.mode7.x;
  int vcenter = (int13)io.mode7.y;
  int hoffset = (int13)io.mode7.hoffset;
  int voffset = (int13)io.mode7.voffset;

  array<bool[256]> windowAbove;
  array<bool[256]> windowBelow;
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  int pixelYp = INT_MIN;
  for(int ys : range(scale)) {
    float y = yt + ys * 1.0 / scale - 0.5;
    float a = 1.0 / lerp(y_a, 1.0 / a_a, y_b, 1.0 / a_b, y);
    float b = 1.0 / lerp(y_a, 1.0 / b_a, y_b, 1.0 / b_b, y);
    float c = 1.0 / lerp(y_a, 1.0 / c_a, y_b, 1.0 / c_b, y);
    float d = 1.0 / lerp(y_a, 1.0 / d_a, y_b, 1.0 / d_b, y);

    int ht = (hoffset - hcenter) % 1024;
    float vty = ((voffset - vcenter) % 1024) + y;
    float originX = (a * ht) + (b * vty) + (hcenter << 8);
    float originY = (c * ht) + (d * vty) + (vcenter << 8);

    uint tile, palette, priority;
    //some games enable mosaic with a mosaic size of 0 (1x1)
    bool mosaicEnable = self.mosaicEnable && io.mosaicSize;
    uint mosaicCounter = 1;
    uint mosaicPalette = 0;
    uint mosaicPriority = 0;
    uint mosaicColor = 0;
    Pixel mosaicPixel;

    int pixelXp = INT_MIN;
    for(int X : range(256)) {
      float xt = !io.mode7.hflip ? X : 255 - X;
      bool doAbove = self.aboveEnable && !windowAbove[X];
      bool doBelow = self.belowEnable && !windowBelow[X];

      for(int xs : range(scale)) {
        float x = xt + xs * 1.0 / scale - 0.5;
        int pixelX = (originX + a * x) / 256;
        int pixelY = (originY + c * x) / 256;

        above++;
        below++;

        //only compute color again when coordinates have changed
        if(pixelX != pixelXp || pixelY != pixelYp) {
          pixelXp = pixelX;
          pixelYp = pixelY;

          tile    = io.mode7.repeat == 3 && ((pixelX | pixelY) & ~1023) ? 0 : ppufast.vram[(pixelY >> 3 & 127) * 128 + (pixelX >> 3 & 127)].byte(0);
          palette = io.mode7.repeat == 2 && ((pixelX | pixelY) & ~1023) ? 0 : ppufast.vram[(((pixelY & 7) << 3) + (pixelX & 7)) + (tile << 6)].byte(1);

          if(!extbg) {
            priority = self.priority[0];
          } else {
            priority = self.priority[palette >> 7];
            palette &= 0x7f;
          }
        }

        if(!mosaicEnable || --mosaicCounter == 0) {
          mosaicCounter = (1 + io.mosaicSize) * scale;
          mosaicPalette = palette;
          mosaicPriority = priority;
          if(io.col.directColor && !extbg) {
            mosaicColor = directColor(0, palette);
          } else {
            mosaicColor = cgram[palette];
          }
          pixel = {source, mosaicPriority, mosaicColor};
        }
        if(!mosaicPalette) continue;

        if(doAbove && (!extbg || mosaicPriority > above->priority)) *above = pixel;
        if(doBelow && (!extbg || mosaicPriority > below->priority)) *below = pixel;
      }
    }
  }
}

//interpolation and extrapolation
auto PPUfast::Line::lerp(float pa, float va, float pb, float vb, float pr) -> float {
  if(va == vb || pr == pa) return va;
  if(pr == pb) return vb;
  return va + (vb - va) / (pb - pa) * (pr - pa);
}
