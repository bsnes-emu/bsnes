auto PPU::Line::render() -> void {
  bool hires = io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;

  if(!io.displayDisable) {
    auto aboveColor = cgram[0];
    auto belowColor = hires ? cgram[0] : io.col.fixedColor;
    for(uint x : range(256)) {
      above[x] = {Source::COL, 0, aboveColor};
      below[x] = {Source::COL, 0, belowColor};
    }
  }

  renderBackground(io.bg1, Source::BG1);
  renderBackground(io.bg2, Source::BG2);
  renderBackground(io.bg3, Source::BG3);
  renderBackground(io.bg4, Source::BG4);
  renderObject(io.obj);

  auto output = !ppu.interlace() || !ppu.field() ? outputLo : outputHi;
  auto width = !ppu.hires() ? 256 : 512;
  auto luma = io.displayBrightness << 15;

  if(io.displayDisable) {
    for(uint x : range(width)) output[x] = 0;
    return;
  }

  renderWindow(io.col.window, io.col.window.aboveMask, windowAbove);
  renderWindow(io.col.window, io.col.window.belowMask, windowBelow);

  if(width == 256) for(uint x : range(width)) {
    output[x] = luma | pixel(x, above[x], below[x]);
  } else if(!hires) for(uint x : range(256)) {
    output[x << 1 | 0] =
    output[x << 1 | 1] = luma | pixel(x, above[x], below[x]);
  } else for(uint x : range(256)) {
    output[x << 1 | 0] = luma | pixel(x, below[x], above[x]);
    output[x << 1 | 1] = luma | pixel(x, above[x], below[x]);
  }
}

auto PPU::Line::pixel(uint x, Pixel above, Pixel below) const -> uint15 {
  if(!windowAbove[x]) above.color = 0x0000;
  if(!windowBelow[x]) return above.color;
  if(!io.col.enable[above.source]) return above.color;
  if(!io.col.blendMode) return blend(above.color, io.col.fixedColor, io.col.halve && windowAbove[x]);
  return blend(above.color, below.color, io.col.halve && windowAbove[x] && below.source != Source::COL);
}

auto PPU::Line::blend(uint x, uint y, bool halve) const -> uint15 {
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

auto PPU::Line::directColor(uint palette, uint tile) const -> uint15 {
  return (palette << 7 & 0x6000) + (tile >> 0 & 0x1000)
       + (palette << 4 & 0x0380) + (tile >> 5 & 0x0040)
       + (palette << 2 & 0x001c) + (tile >> 9 & 0x0002);
}

auto PPU::Line::plotAbove(uint x, uint source, uint priority, uint color) -> void {
  if(priority >= above[x].priority) above[x] = {source, priority, color};
}

auto PPU::Line::plotBelow(uint x, uint source, uint priority, uint color) -> void {
  if(priority >= below[x].priority) below[x] = {source, priority, color};
}
