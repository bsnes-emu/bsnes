uint PPU::Line::start = 0;
uint PPU::Line::count = 0;

auto PPU::Line::flush() -> void {
  if(Line::count) {
    #pragma omp parallel for if(Line::count >= 8)
    for(uint y = 0; y < Line::count; y++) {
      ppu.lines[Line::start + y].render();
    }
    Line::start = 0;
    Line::count = 0;
  }
}

auto PPU::Line::render() -> void {
  auto output = ppu.output + y * 1024;
  if(ppu.interlace() && ppu.field()) output += 512;
  auto width = !ppu.hires() ? 256 : 512;

  if(io.displayDisable) {
    memory::fill<uint32>(output, width);
    return;
  }

  bool hires = io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;
  bool hiresMode7 = io.bgMode == 7 && configuration.hacks.ppuFast.hiresMode7;
  auto aboveColor = cgram[0];
  auto belowColor = hires ? cgram[0] : io.col.fixedColor;
  for(uint x : range(256 << hiresMode7)) {
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
  if(hiresMode7) for(uint x : range(512)) {
    auto Above = above[x >> 1].source >= Source::OBJ1 ? above[x >> 1] : above[x >> 1 | (x & 1 ? 256 : 0)];
    auto Below = below[x >> 1].source >= Source::OBJ1 ? below[x >> 1] : below[x >> 1 | (x & 1 ? 256 : 0)];
    *output++ = luma | pixel(x >> 1, Above, Below);
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

auto PPU::Line::directColor(uint paletteIndex, uint paletteColor) const -> uint15 {
  //paletteIndex = bgr
  //paletteColor = BBGGGRRR
  //output       = 0 BBb00 GGGg0 RRRr0
  return (paletteColor << 2 & 0x001c) + (paletteIndex <<  1 & 0x0002)   //R
       + (paletteColor << 4 & 0x0380) + (paletteIndex <<  5 & 0x0040)   //G
       + (paletteColor << 7 & 0x6000) + (paletteIndex << 10 & 0x1000);  //B
}

auto PPU::Line::plotAbove(uint x, uint source, uint priority, uint color) -> void {
  if(priority > above[x].priority) above[x] = {source, priority, color};
}

auto PPU::Line::plotBelow(uint x, uint source, uint priority, uint color) -> void {
  if(priority > below[x].priority) below[x] = {source, priority, color};
}
