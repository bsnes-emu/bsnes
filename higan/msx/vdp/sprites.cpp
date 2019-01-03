auto VDP::sprite(uint8 voffset) -> void {
  uint valid = 0;
  uint limit = io.spriteSize ? 15 : 7;
  for(uint index : range(4)) sprites[index].y = 0xd0;

  uint14 attributeAddress;
  attributeAddress.bits(7,13) = io.spriteAttributeTableAddress;
  for(uint index : range(32)) {
    uint8 y = vram.read(attributeAddress++);
    if(y == 0xd0) break;

    uint8 x = vram.read(attributeAddress++);
    uint8 pattern = vram.read(attributeAddress++);
    uint8 extra = vram.read(attributeAddress++);

    if(extra.bit(7)) x -= 32;
    y += 1;
    if(voffset < y) continue;
    if(voffset > y + limit) continue;

    if(limit == 15) pattern.bits(0,1) = 0;

    if(valid == 4) {
      io.spriteOverflow = true;
      io.spriteOverflowIndex = index;
      break;
    }

    sprites[valid++] = {x, y, pattern, extra.bits(0,3)};
  }
}

auto VDP::sprite(uint8 hoffset, uint8 voffset) -> void {
  uint4 color;
  uint limit = io.spriteSize ? 15 : 7;

  for(uint n : range(4)) {
    auto& o = sprites[n];
    if(o.y == 0xd0) continue;
    if(hoffset < o.x) continue;
    if(hoffset > o.x + limit) continue;

    uint x = hoffset - o.x;
    uint y = voffset - o.y;

    uint14 address;
    address.bits( 0,10) = (o.pattern << 3) + (x >> 3 << 4) + (y & limit);
    address.bits(11,13) = io.spritePatternTableAddress;

    uint3 index = x ^ 7;
    if(vram.read(address).bit(index)) {
      if(color) { io.spriteCollision = true; break; }
      color = o.color;
    }
  }

  if(color) output.color = color;
}
