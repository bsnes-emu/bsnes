auto VDC::Background::scanline(uint y) -> void {
  hoffset = hscroll;
  if(y == 0) {
    voffset = vscroll;
  } else {
    voffset++;
  }
}

auto VDC::Background::run(uint x, uint y) -> void {
  color = nothing;

  uint16 batAddress;
  batAddress  = (voffset >> 3) & (height - 1);
  batAddress *= width;
  batAddress += (hoffset >> 3) & (width  - 1);

  uint16 tiledata = vdc.vramRead(batAddress);
  uint16 patternAddress = tiledata.bits(0,11) << 4;
  patternAddress += (voffset & 7);
  uint4 palette = tiledata.bits(12,15);

  uint16 d0 = vdc.vramRead(patternAddress + 0);
  uint16 d1 = vdc.vramRead(patternAddress + 8);

  uint3 index = 7 - (hoffset & 7);
  uint4 output;
  output.bit(0) = d0.bit(0 + index);
  output.bit(1) = d0.bit(8 + index);
  output.bit(2) = d1.bit(0 + index);
  output.bit(3) = d1.bit(8 + index);

  if(enable && output) color = vdc.cram[palette << 4 | output];
  hoffset++;
}
