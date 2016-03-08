auto PPU::renderColorFetch(uint16 offset, uint3 y, uint3 x) -> uint4 {
  uint4 color;

  if(system.planar()) {
    uint32 data = iram.read(offset + (y << 2), Long);
    color |= data.bit( 7 - x) << 0;
    color |= data.bit(15 - x) << 1;
    color |= data.bit(23 - x) << 2;
    color |= data.bit(31 - x) << 3;
  }

  if(system.packed()) {
    uint8 data = iram.read(offset + (y << 2) + (x >> 1));
    color = data >> (4 - (x.bit(0) << 2));
  }

  return color;
}

auto PPU::renderColorBack() -> void {
  uint12 color = iram.read(0xfe00 + (r.backColor << 1), Word);
  pixel = {Pixel::Source::Back, color};
}

auto PPU::renderColorScreenOne() -> void {
  if(!r.screenOneEnable) return;

  uint8 scrollY = status.vclk + r.scrollOneY;
  uint8 scrollX = status.hclk + r.scrollOneX;

  uint16 tilemapOffset = r.screenOneMapBase << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint16 tileOffset = 0x4000 + (tile.bit(13) << 14) + (tile.bits(0,8) << 5);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint4 tileColor = renderColorFetch(tileOffset, tileY, tileX);
  if(tileColor == 0) return;

  uint12 color = iram.read(0xfe00 + (tile.bits(9, 12) << 5) + (tileColor << 1), Word);
  pixel = {Pixel::Source::ScreenOne, color};
}

auto PPU::renderColorScreenTwo() -> void {
  if(!r.screenTwoEnable) return;

  bool windowInside = status.vclk >= r.screenTwoWindowY0 && status.vclk <= r.screenTwoWindowY1
                   && status.hclk >= r.screenTwoWindowX0 && status.hclk <= r.screenTwoWindowX1;
  windowInside ^= r.screenTwoWindowInvert;
  if(r.screenTwoWindowEnable && !windowInside) return;

  uint8 scrollY = status.vclk + r.scrollTwoY;
  uint8 scrollX = status.hclk + r.scrollTwoX;

  uint16 tilemapOffset = r.screenTwoMapBase << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint16 tileOffset = 0x4000 + (tile.bit(13) << 14) + (tile.bits(0,8) << 5);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint4 tileColor = renderColorFetch(tileOffset, tileY, tileX);
  if(tileColor == 0) return;

  uint12 color = iram.read(0xfe00 + (tile.bits(9,12) << 5) + (tileColor << 1), Word);
  pixel = {Pixel::Source::ScreenTwo, color};
}

auto PPU::renderColorSprite() -> void {
  if(!r.spriteEnable) return;

  bool windowInside = status.vclk >= r.spriteWindowY0 && status.vclk <= r.spriteWindowY1
                   && status.hclk >= r.spriteWindowX0 && status.hclk <= r.spriteWindowX1;
  uint16 spriteBase = r.spriteBase << 9;

  uint7 spriteIndex = r.spriteFirst;
  uint8 spriteCount = min(128, (uint)r.spriteCount);
  while(spriteCount--) {
    uint32 sprite = iram.read(spriteBase + (spriteIndex++ << 2), Long);
    if(r.spriteWindowEnable && sprite.bit(12) && !windowInside) continue;

    uint8 spriteY = sprite.bits(16,23);
    uint8 spriteX = sprite.bits(24,31);

    if(status.vclk < spriteY) continue;
    if(status.vclk > (uint8)(spriteY + 7)) continue;
    if(status.hclk < spriteX) continue;
    if(status.hclk > (uint8)(spriteX + 7)) continue;

    uint16 tileOffset = 0x4000 + (sprite.bits(0,8) << 5);
    uint3 tileY = (uint8)(status.vclk - spriteY) ^ (sprite.bit(15) ? 7 : 0);
    uint3 tileX = (uint8)(status.hclk - spriteX) ^ (sprite.bit(14) ? 7 : 0);
    uint4 tileColor = renderColorFetch(tileOffset, tileY, tileX);
    if(tileColor == 0) continue;
    if(!sprite.bit(13) && pixel.source == Pixel::Source::ScreenTwo) continue;

    uint12 color = iram.read(0xff00 + (sprite.bits(9,11) << 5) + (tileColor << 1), Word);
    pixel = {Pixel::Source::Sprite, color};
    return;
  }
}
