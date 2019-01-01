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

auto PPU::latchSprites(uint8 y) -> void {
  l.spriteCount = 0;
  if(!l.spriteEnable) return;
  for(uint index : range(l.oamCount)) {
    uint32 attributes = l.oam[!s.field][index];
    if((uint8)(y - attributes.bits(16,23)) > 7) continue;
    l.sprite[l.spriteCount] = attributes;
    if(++l.spriteCount >= 32) break;
  }
}

//note: this implicitly latches spriteBase, spriteFirst, spriteCount
auto PPU::latchOAM() -> void {
  uint7 spriteIndex = r.spriteFirst;
  uint8 spriteCount = min(128, (uint)r.spriteCount);
  uint16 spriteBase = r.spriteBase.bits(0, 4 + system.depth()) << 9;
  l.oamCount = spriteCount;
  for(uint index : range(spriteCount)) {
    l.oam[s.field][index] = iram.read32(spriteBase + (spriteIndex++ << 2));
  }
}
