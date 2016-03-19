auto PPU::renderSpriteFetch() -> void {
  uint16 spriteBase = l.spriteBase.bits(0, 4 + system.depth()) << 9;
  for(auto spriteIndex : range(128)) {
    oam[s.field][spriteIndex] = iram.read(spriteBase + (spriteIndex << 2), Long);
  }
}

auto PPU::renderSpriteDecode() -> void {
  sprites.reset();
  sprites.reserve(32);
  if(!l.spriteEnable) return;

  uint offset = 0;
  bool windowInside = s.vclk >= l.spriteWindowY0 && s.vclk <= l.spriteWindowY1;
  uint7 spriteIndex = l.spriteFirst;
  uint8 spriteCount = min(128, (uint)l.spriteCount);
  while(spriteCount--) {
    uint32 attributes = oam[s.field][spriteIndex++];

    Sprite sprite;
    sprite.x = attributes.bits(24,31);
    if(sprite.x > 224 && sprite.x < 249) continue;
    sprite.y = attributes.bits(16,23);
    if((uint8)(s.vclk - sprite.y) > 7) continue;
    sprite.vflip = attributes.bit(15);
    sprite.hflip = attributes.bit(14);
    sprite.priority = attributes.bit(13);
    sprite.window = attributes.bit(12);
    if(l.spriteWindowEnable && sprite.window == windowInside) continue;
    sprite.palette = 8 + attributes.bits(9,11);
    sprite.tile = attributes.bits(0,8);

    sprites.append(sprite);
    if(sprites.size() >= 32) break;
  }
}
