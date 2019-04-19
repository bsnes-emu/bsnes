uint PPUfast::Line::start = 0;
uint PPUfast::Line::count = 0;

auto PPUfast::Line::flush() -> void {
  if(Line::count) {
    #pragma omp parallel for if(Line::count >= 8)
    for(uint y = 0; y < Line::count; y++) {
      ppufast.lines[Line::start + y].render();
    }
    Line::start = 0;
    Line::count = 0;
  }
}

auto PPUfast::Line::render() -> void {
  auto hd = ppufast.hd();
  auto ss = ppufast.ss();
  auto scale = ppufast.hdScale();
  auto output = ppufast.output + (!hd
  ? (y * 1024 + (ppufast.interlace() && ppufast.field() ? 512 : 0))
  : (y * 256 * scale * scale)
  );
  auto width = (!hd
  ? (!ppufast.hires() ? 256 : 512)
  : (256 * scale * scale));

  if(io.displayDisable) {
    memory::fill<uint32>(output, width);
    return;
  }

  bool hires = io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;
  auto aboveColor = cgram[0];
  auto belowColor = hires ? cgram[0] : io.col.fixedColor;
  uint xa =  (hd || ss) && ppufast.interlace() && ppufast.field() ? 256 * scale * scale / 2 : 0;
  uint xb = !(hd || ss) ? 256 : ppufast.interlace() && !ppufast.field() ? 256 * scale * scale / 2 : 256 * scale * scale;
  for(uint x = xa; x < xb; x++) {
    above[x] = {Source::COL, 0, aboveColor};
    below[x] = {Source::COL, 0, belowColor};
  }

  renderBackground(io.bg1, Source::BG1);
  renderBackground(io.bg2, Source::BG2);
  renderBackground(io.bg3, Source::BG3);
  renderBackground(io.bg4, Source::BG4);
  renderObject(io.obj);
  renderWindow(io.col.window, io.col.window.aboveMask, windowAbove);
  renderWindow(io.col.window, io.col.window.belowMask, windowBelow);

  auto luma = io.displayBrightness << 15;
  if(hd) for(uint x : range(256 * scale * scale)) {
    *output++ = luma | pixel(x / scale & 255, above[x], below[x]);
  } else if(width == 256) for(uint x : range(256)) {
    *output++ = luma | pixel(x, above[x], below[x]);
  } else if(!hires) for(uint x : range(256)) {
    auto color = luma | pixel(x, above[x], below[x]);
    *output++ = color;
    *output++ = color;
  } else for(uint x : range(256)) {
    *output++ = luma | pixel(x, below[x], above[x]);
    *output++ = luma | pixel(x, above[x], below[x]);
  }
}

auto PPUfast::Line::pixel(uint x, Pixel above, Pixel below) const -> uint15 {
  if(!windowAbove[x]) above.color = 0x0000;
  if(!windowBelow[x]) return above.color;
  if(!io.col.enable[above.source]) return above.color;
  if(!io.col.blendMode) return blend(above.color, io.col.fixedColor, io.col.halve && windowAbove[x]);
  return blend(above.color, below.color, io.col.halve && windowAbove[x] && below.source != Source::COL);
}

auto PPUfast::Line::blend(uint x, uint y, bool halve) const -> uint15 {
  if(!io.col.mathMode) {  //add
    if(!halve) {
      uint sum = x + y;
      uint carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {  //sub
    uint diff = x - y + 0x8420;
    uint borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!halve) {
      return   (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

auto PPUfast::Line::directColor(uint paletteIndex, uint paletteColor) const -> uint15 {
  //paletteIndex = bgr
  //paletteColor = BBGGGRRR
  //output       = 0 BBb00 GGGg0 RRRr0
  return (paletteColor << 2 & 0x001c) + (paletteIndex <<  1 & 0x0002)   //R
       + (paletteColor << 4 & 0x0380) + (paletteIndex <<  5 & 0x0040)   //G
       + (paletteColor << 7 & 0x6000) + (paletteIndex << 10 & 0x1000);  //B
}

auto PPUfast::Line::plotAbove(uint x, uint source, uint priority, uint color) -> void {
  if(ppufast.hd()) return plotHD(above, x, source, priority, color, false, false);
  if(priority > above[x].priority) above[x] = {source, priority, color};
}

auto PPUfast::Line::plotBelow(uint x, uint source, uint priority, uint color) -> void {
  if(ppufast.hd()) return plotHD(below, x, source, priority, color, false, false);
  if(priority > below[x].priority) below[x] = {source, priority, color};
}

//todo: name these variables more clearly ...
auto PPUfast::Line::plotHD(Pixel* pixel, uint x, uint source, uint priority, uint color, bool hires, bool subpixel) -> void {
  auto scale = ppufast.hdScale();
  int xss = hires && subpixel ? scale / 2 : 0;
  int ys = ppufast.interlace() && ppufast.field() ? scale / 2 : 0;
  if(priority > pixel[x * scale + xss + ys * 256 * scale].priority) {
    Pixel p = {source, priority, color};
    int xsm = hires && !subpixel ? scale / 2 : scale;
    int ysm = ppufast.interlace() && !ppufast.field() ? scale / 2 : scale;
    for(int xs = xss; xs < xsm; xs++) {
      pixel[x * scale + xs + ys * 256 * scale] = p;
    }
    int size = sizeof(Pixel) * (xsm - xss);
    Pixel* source = &pixel[x * scale + xss + ys * 256 * scale];
    for(int yst = ys + 1; yst < ysm; yst++) {
      memcpy(&pixel[x * scale + xss + yst * 256 * scale], source, size);
    }
  }
}
