auto PPU::latchRegisters() -> void {
  l.backColor = r.backColor;

  l.screenOneEnable = r.screenOneEnable;
  l.screenOneMapBase = r.screenOneMapBase;
  l.scrollOneX = r.scrollOneX;
  l.scrollOneY = r.scrollOneY;

  l.screenTwoEnable = r.screenTwoEnable;
  l.screenTwoMapBase = r.screenTwoMapBase;
  l.scrollTwoX = r.scrollTwoX;
  l.scrollTwoY = r.scrollTwoY;
  l.screenTwoWindowEnable = r.screenTwoWindowEnable;
  l.screenTwoWindowInvert = r.screenTwoWindowInvert;
  l.screenTwoWindowX0 = r.screenTwoWindowX0;
  l.screenTwoWindowY0 = r.screenTwoWindowY0;
  l.screenTwoWindowX1 = r.screenTwoWindowX1;
  l.screenTwoWindowY1 = r.screenTwoWindowY1;

  l.spriteEnable = r.spriteEnable;
  l.spriteWindowEnable = r.spriteWindowEnable;
  l.spriteWindowX0 = r.spriteWindowX0;
  l.spriteWindowY0 = r.spriteWindowY0;
  l.spriteWindowX1 = r.spriteWindowX1;
  l.spriteWindowY1 = r.spriteWindowY1;
}

auto PPU::latchSprites() -> void {
  l.spriteCount = 0;
  if(!l.spriteEnable) return;

  uint offset = 0;
  bool windowInside = s.vclk >= l.spriteWindowY0 && s.vclk <= l.spriteWindowY1;
  for(auto index : range(l.oamCount)) {
    uint32 attributes = l.oam[!s.field][index];

    auto& sprite = l.sprite[l.spriteCount];
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

    if(++l.spriteCount >= 32) break;
  }
}

//note: this implicitly latches spriteBase, spriteFirst, spriteCount
auto PPU::latchOAM() -> void {
  uint7 spriteIndex = r.spriteFirst;
  uint8 spriteCount = min(128, (uint)r.spriteCount);
  uint16 spriteBase = r.spriteBase.bits(0, 4 + system.depth()) << 9;
  l.oamCount = spriteCount;
  for(auto index : range(spriteCount)) {
    l.oam[s.field][index] = iram.read(spriteBase + (spriteIndex++ << 2), Long);
  }
}
