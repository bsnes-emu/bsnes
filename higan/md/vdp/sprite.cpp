auto VDP::Object::width() const -> uint {
  return 1 + tileWidth << 3;
}

auto VDP::Object::height() const -> uint {
  return 1 + tileHeight << 3 + (vdp.io.interlaceMode == 3);
}

auto VDP::Sprite::write(uint9 address, uint16 data) -> void {
  if(address > 320) return;

  auto& object = oam[address >> 2];
  switch(address.bits(0,1)) {

  case 0: {
    object.y = data.bits(0,9);
    break;
  }

  case 1: {
    object.link = data.bits(0,6);
    object.tileHeight = data.bits(8,9);
    object.tileWidth = data.bits(10,11);
    break;
  }

  case 2: {
    object.address = data.bits(0,10);
    object.horizontalFlip = data.bit(11);
    object.verticalFlip = data.bit(12);
    object.palette = data.bits(13,14);
    object.priority = data.bit(15);
    break;
  }

  case 3: {
    object.x = data.bits(0,8);
    break;
  }

  }
}

auto VDP::Sprite::scanline(uint y) -> void {
  bool interlace = vdp.io.interlaceMode == 3;
  y += 128;
  if(interlace) y = y << 1 | vdp.state.field;

  objects.reset();

  uint7 link = 0;
  uint tiles = 0;
  uint count = 0;
  do {
    auto& object = oam[link];
    link = object.link;

    if(y <  object.y) continue;
    if(y >= object.y + object.height()) continue;
    if(object.x == 0) break;

    objects.append(object);
    tiles += object.width() >> 3;
  } while(link && link < 80 && objects.size() < 20 && tiles < 40 && ++count < 80);
}

auto VDP::Sprite::run(uint x, uint y) -> void {
  bool interlace = vdp.io.interlaceMode == 3;
  x += 128;
  y += 128;
  if(interlace) y = y << 1 | vdp.state.field;

  output.priority = 0;
  output.color = 0;

  for(auto& object : objects) {
    if(x <  object.x) continue;
    if(x >= object.x + object.width()) continue;

    uint objectX = x - object.x;
    uint objectY = y - object.y;
    if(object.horizontalFlip) objectX = (object.width() - 1) - objectX;
    if(object.verticalFlip) objectY = (object.height() - 1) - objectY;

    uint tileX = objectX >> 3;
    uint tileY = objectY >> 3 + interlace;
    uint tileNumber = tileX * (object.height() >> 3 + interlace) + tileY;
    uint15 tileAddress = object.address + tileNumber << 4 + interlace;
    uint pixelX = objectX & 7;
    uint pixelY = objectY & 7 + interlace * 8;
    tileAddress += pixelY << 1 | pixelX >> 2;

    uint16 tileData = vdp.vram.read(tileAddress);
    uint4 color = tileData >> (((pixelX & 3) ^ 3) << 2);
    if(!color) continue;

    output.color = object.palette << 4 | color;
    output.priority = object.priority;
    break;
  }
}

auto VDP::Sprite::power() -> void {
  io = {};
}
