auto VDP::Sprite::write(uint9 address, uint16 data) -> void {
  if(address > 320) return;

  auto& object = oam[address >> 2];
  switch(address.bits(0,1)) {

  case 0: {
    object.y = data.bits(0,9) - 128;
    break;
  }

  case 1: {
    object.link = data.bits(0,6);
    object.height = data.bits(8,9) << 3;
    object.width = data.bits(10,11) << 3;
    break;
  }

  case 2: {
    object.address = data.bits(0,10) << 4;
    object.horizontalFlip = data.bit(11);
    object.verticalFlip = data.bit(12);
    object.palette = data.bits(13,14);
    object.priority = data.bit(15);
    break;
  }

  case 3: {
    object.x = data.bits(0,9) - 128;
    break;
  }

  }
}

auto VDP::Sprite::scanline(uint y) -> void {
  object.reset();

  uint7 link = 0;
  while(link) {
    auto& o = oam[link];

    if((uint9)(o.y + o.height - 1) < y) continue;
    if((uint9)(y + o.height - 1) < o.y) continue;
    if(o.x == 0) break;

    object.append(o);
    if(object.size() >= 20) break;

    link = o.link;
    if(!link || link >= 80) break;
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
