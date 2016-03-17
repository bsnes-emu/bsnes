auto PPU::renderSpriteFetch() -> void {
  uint16 spriteBase = r.spriteBase.bits(0, 4 + system.depth()) << 9;
  for(auto spriteIndex : range(128)) {
    oam[spriteIndex] = iram.read(spriteBase + (spriteIndex << 2), Long);
  }
}

auto PPU::renderSpriteDecode() -> void {
  sprites.reset();
  sprites.reserve(32);
  if(!r.spriteEnable) return;

  uint offset = 0;
  bool windowInside = status.vclk >= r.spriteWindowY0 && status.vclk <= r.spriteWindowY1;
  uint16 spriteBase = r.spriteBase.bits(0, 4 + system.depth()) << 9;
  uint7 spriteIndex = r.spriteFirst;
  uint8 spriteCount = min(128, (uint)r.spriteCount);
  while(spriteCount--) {
    uint32 attributes = oam[spriteIndex++];

    Sprite sprite;
    sprite.x = attributes.bits(24,31);
    if(sprite.x > 224) continue;
    sprite.y = attributes.bits(16,23);
    if(status.vclk < sprite.y) continue;
    if(status.vclk > sprite.y + 7) continue;
    sprite.vflip = attributes.bit(15);
    sprite.hflip = attributes.bit(14);
    sprite.priority = attributes.bit(13);
    sprite.window = attributes.bit(12);
    if(r.spriteWindowEnable && sprite.window && !windowInside) continue;
    sprite.palette = 8 + attributes.bits(9,11);
    sprite.tile = attributes.bits(0,8);

    sprites.append(sprite);
    if(sprites.size() >= 32) break;
  }
}
