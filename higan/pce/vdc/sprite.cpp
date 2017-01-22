auto VDC::Sprite::scanline(uint y) -> void {
  y += 64;
  objects.reset();

  static const uint widths[]  = {15, 31};
  static const uint heights[] = {15, 31, 63, 63};

  uint count = 0;
  for(uint index : range(64)) {
    uint16 d0 = vdc.satb.read(index << 2 | 0);
    uint16 d1 = vdc.satb.read(index << 2 | 1);
    uint16 d2 = vdc.satb.read(index << 2 | 2);
    uint16 d3 = vdc.satb.read(index << 2 | 3);

    Object object;
    object.y = d0.bits(0,9);
    object.height = heights[d3.bits(12,13)];
    if(y < object.y) continue;
    if(y > object.y + object.height) continue;

    object.x = d1.bits(0,9);
    object.mode = d2.bit(0);
    object.pattern = d2.bits(1,10);
    object.palette = d3.bits(0,3);
    object.priority = d3.bit(7);
    object.width = widths[d3.bit(8)];
    object.hflip = d3.bit(11);
    object.vflip = d3.bit(15);
    object.first = index == 0;

    if(object.width  == 31) object.pattern.bit(0) = 0;
    if(object.height == 31) object.pattern.bit(1) = 0;
    if(object.height == 63) object.pattern.bits(1,2) = 0;

    if(object.width == 15) {
      objects.append(object);
      if(++count >= 16) return vdc.irq.raise(VDC::IRQ::Line::Overflow);
    } else {
      //32-width sprites count as two 16-width sprite slots
      object.pattern ^= object.hflip;
      object.width = 15;
      objects.append(object);
      if(++count >= 16) return vdc.irq.raise(VDC::IRQ::Line::Overflow);

      object.x += 16;
      object.pattern ^= 1;
      objects.append(object);
      if(++count >= 16) return vdc.irq.raise(VDC::IRQ::Line::Overflow);
    }
  }
}

auto VDC::Sprite::run(uint x, uint y) -> void {
  x += 32;
  y += 64;

  color = nothing;
  if(!enable) return;

  bool first = false;
  for(auto& object : objects) {
    if(x < object.x) continue;
    if(x > object.x + object.width) continue;

    uint10 hoffset = x - object.x;
    uint10 voffset = y - object.y;
    if(object.hflip) hoffset ^= object.width;
    if(object.vflip) voffset ^= object.height;

    uint16 patternAddress = object.pattern;
    patternAddress  += (voffset >> 4) << 1;
    patternAddress  += (hoffset >> 4);
    patternAddress <<= 6;
    patternAddress  += (voffset & 15);

    uint16 d0 = vdc.vram.read(patternAddress +  0);
    uint16 d1 = vdc.vram.read(patternAddress + 16);
    uint16 d2 = vdc.vram.read(patternAddress + 32);
    uint16 d3 = vdc.vram.read(patternAddress + 48);

    uint4 index = 15 - (hoffset & 15);
    uint4 output;
    output.bit(0) = d0.bit(index);
    output.bit(1) = d1.bit(index);
    output.bit(2) = d2.bit(index);
    output.bit(3) = d3.bit(index);
    if(output == 0) continue;

    if(color) {
      if(first) return vdc.irq.raise(VDC::IRQ::Line::Collision);
      return;
    }

    color = vdc.cram.read(1 << 8 | object.palette << 4 | output);
    priority = object.priority;
    if(object.first) first = true;
  }
}
