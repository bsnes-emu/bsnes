auto VDP::Sprite::setup(uint9 voffset) -> void {
  objectsValid = 0;
  uint limit = vdp.io.spriteTile ? 15 : 7;

  if(!vdp.io.mode.bit(3)) {
    uint14 attributeAddress;
    attributeAddress.bits(7,13) = vdp.io.spriteAttributeTableAddress;
    for(uint index : range(32)) {
      uint8 y = vdp.vram[attributeAddress++];
      if(y == 0xd0) break;

      uint8 x = vdp.vram[attributeAddress++];
      uint8 pattern = vdp.vram[attributeAddress++];
      uint8 extra = vdp.vram[attributeAddress++];

      if(extra.bit(7)) x -= 32;
      y += 1;
      if(voffset < y) continue;
      if(voffset > y + limit) continue;

      if(limit == 15) pattern.bits(0,1) = 0;

      objects[objectsValid] = {x, y, pattern, extra.bits(0,3)};
      if(++objectsValid == 4) {
        vdp.io.spriteOverflow = 1;
        break;
      }
    }
  } else {
    uint14 attributeAddress;
    attributeAddress.bits(8,13) = vdp.io.spriteAttributeTableAddress.bits(1,6);
    for(uint index : range(64)) {
      uint8 y = vdp.vram[attributeAddress + index];
      uint8 x = vdp.vram[attributeAddress + 0x80 + (index << 1)];
      uint8 pattern = vdp.vram[attributeAddress + 0x81 + (index << 1)];
      if(vdp.vlines() == 192 && y == 0xd0) break;

      if(vdp.io.spriteShift) x -= 8;
      y += 1;
      if(voffset < y) continue;
      if(voffset > y + limit) continue;

      if(limit == 15) pattern.bit(0) = 0;

      objects[objectsValid] = {x, y, pattern};
      if(++objectsValid == 8) {
        vdp.io.spriteOverflow = 1;
        break;
      }
    }
  }
}

auto VDP::Sprite::run(uint8 hoffset, uint9 voffset) -> void {
  output = {};
  switch(vdp.io.mode) {
  case 0b0000: return graphics1(hoffset, voffset);
  case 0b0001: return;
  case 0b0010: return graphics2(hoffset, voffset);
  case 0b0011: return;
  case 0b0100: return;
  case 0b0101: return;
  case 0b0110: return;
  case 0b0111: return;
  case 0b1000: return graphics3(hoffset, voffset, 192);
  case 0b1001: return;
  case 0b1010: return graphics3(hoffset, voffset, 192);
  case 0b1011: return graphics3(hoffset, voffset, 224);
  case 0b1100: return graphics3(hoffset, voffset, 192);
  case 0b1101: return;
  case 0b1110: return graphics3(hoffset, voffset, 240);
  case 0b1111: return graphics3(hoffset, voffset, 192);
  }
}

auto VDP::Sprite::graphics1(uint8 hoffset, uint9 voffset) -> void {
  //todo: are sprites different in graphics mode 1?
  return graphics2(hoffset, voffset);
}

auto VDP::Sprite::graphics2(uint8 hoffset, uint9 voffset) -> void {
  uint limit = vdp.io.spriteTile ? 15 : 7;
  for(uint objectIndex : range(objectsValid)) {
    auto& o = objects[objectIndex];
    if(hoffset < o.x) continue;
    if(hoffset > o.x + limit) continue;

    uint x = hoffset - o.x;
    uint y = voffset - o.y;

    uint14 address;
    address.bits( 0,10) = (o.pattern << 3) + (x >> 3 << 4) + (y & limit);
    address.bits(11,13) = vdp.io.spritePatternTableAddress;

    uint3 index = x ^ 7;
    if(vdp.vram[address].bit(index)) {
      if(output.color) { vdp.io.spriteCollision = true; break; }
      output.color = o.color;
    }
  }
}

auto VDP::Sprite::graphics3(uint8 hoffset, uint9 voffset, uint vlines) -> void {
  uint limit = vdp.io.spriteTile ? 15 : 7;
  for(uint objectIndex : range(objectsValid)) {
    auto& o = objects[objectIndex];
    if(hoffset < o.x) continue;
    if(hoffset > o.x + 7) continue;

    uint x = hoffset - o.x;
    uint y = voffset - o.y;

    uint14 address;
    address.bits(2,12) = (o.pattern << 3) + (y & limit);
    address.bit   (13) = vdp.io.spritePatternTableAddress.bit(2);

    uint3 index = x ^ 7;
    uint4 color;
    color.bit(0) = vdp.vram[address | 0].bit(index);
    color.bit(1) = vdp.vram[address | 1].bit(index);
    color.bit(2) = vdp.vram[address | 2].bit(index);
    color.bit(3) = vdp.vram[address | 3].bit(index);
    if(color == 0) continue;

    if(output.color) { vdp.io.spriteCollision = true; break; }
    output.color = color;
  }
}

auto VDP::Sprite::power() -> void {
  output = {};
  objectsValid = 0;
}
