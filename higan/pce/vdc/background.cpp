auto VDC::Background::scanline(uint y) -> void {
  if(y == 0) {
    vcounter = vscroll;
  } else {
    vcounter++;
  }
  hoffset = hscroll;
  voffset = vcounter;
}

auto VDC::Background::run(uint x, uint y) -> void {
  color = 0;
  palette = 0;

  uint16 batAddress;
  batAddress  = (voffset >> 3) & (height - 1);
  batAddress *= width;
  batAddress += (hoffset >> 3) & (width  - 1);

  uint16 tiledata = vdc->vram.read(batAddress);
  uint16 patternAddress = tiledata.bits(0,11) << 4;
  patternAddress += (voffset & 7);
  uint4 palette = tiledata.bits(12,15);

  uint16 d0 = vdc->vram.read(patternAddress + 0);
  uint16 d1 = vdc->vram.read(patternAddress + 8);

  uint3 index = 7 - (hoffset & 7);
  uint4 color;
  color.bit(0) = d0.bit(0 + index);
  color.bit(1) = d0.bit(8 + index);
  color.bit(2) = d1.bit(0 + index);
  color.bit(3) = d1.bit(8 + index);

  if(enable && color) {
    this->color = color;
    this->palette = palette;
  }

  hoffset++;
}
