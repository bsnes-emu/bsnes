auto VDP::Sprite::frame() -> void {
  uint15 address = io.attributeAddress;
  uint7 link = 0;

  oam.reset();
  while(oam.size() < 80) {
    uint64 attributes;
    attributes |= (uint64)vdp.vram[address + (link << 2) + 0] << 48;
    attributes |= (uint64)vdp.vram[address + (link << 2) + 1] << 32;
    attributes |= (uint64)vdp.vram[address + (link << 2) + 2] << 16;
    attributes |= (uint64)vdp.vram[address + (link << 2) + 3] <<  0;

    auto& object = oam.append();
    object.x              = attributes.bits( 0, 9) - 128;
    object.address        = attributes.bits(16,26) << 4;
    object.horizontalFlip = attributes.bit (27);
    object.verticalFlip   = attributes.bit (28);
    object.palette        = attributes.bits(29,30);
    object.priority       = attributes.bit (31);
    object.height         = attributes.bits(40,41) << 3;
    object.width          = attributes.bits(42,43) << 3;
    object.y              = attributes.bits(48,57) - 128;

    link = attributes.bits(32,38);
    if(!link) break;
  }
}

auto VDP::Sprite::scanline(uint y) -> void {
  object.reset();
  for(auto& o : oam) {
    if((uint9)(o.y + o.height - 1) < y) continue;
    if((uint9)(y + o.height - 1) < o.y) continue;
    if(o.x == 0) break;

    object.append(o);
    if(object.size() >= object.capacity()) break;
  }
}

auto VDP::Sprite::run(uint x, uint y) -> void {
  output.priority = 0;
  output.color = 0;

  for(auto& o : object) {
    if((uint9)(o.x + o.width - 1) < x) continue;
    if((uint9)(y + o.width - 1) < o.x) continue;

    auto objectX = (uint9)(x - o.x);
    auto objectY = (uint9)(y - o.y);
    if(o.horizontalFlip) objectX = (o.width - 1) - objectX;
    if(o.verticalFlip) objectY = (o.height - 1) - objectY;

    uint tileX = objectX >> 3;
    uint tileY = objectY >> 3;
    uint tileNumber = tileX * (o.width >> 3) + tileY;
    uint15 tileAddress = o.address + (tileNumber << 4);
    uint pixelX = objectX & 7;
    uint pixelY = objectY & 7;
    tileAddress += pixelY << 1 | pixelX >> 2;

    uint16 tileData = vdp.vram[tileAddress];
    uint4 color = tileData >> (((pixelX & 3) ^ 3) << 2);
    if(color) {
      output.color = o.palette << 4 | color;
      output.priority = o.priority;
    }
  }
}

auto VDP::Sprite::power() -> void {
}

auto VDP::Sprite::reset() -> void {
  memory::fill(&io, sizeof(IO));
}
