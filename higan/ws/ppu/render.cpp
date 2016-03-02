auto PPU::renderBack() -> void {
  uint4 poolColor = 15 - r.pool[r.backColorIndex];
  pixel = {Pixel::Source::Back, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderScreenOne() -> void {
  if(!r.screenOneEnable) return;

  uint8 scrollX = status.hclk + r.scrollOneX;
  uint8 scrollY = status.vclk + r.scrollOneY;

  uint14 tilemapOffset = r.screenOneMapBase << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile;
  tile.byte(0) = iram[tilemapOffset++];
  tile.byte(1) = iram[tilemapOffset++];

  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);

  uint14 tileOffset = 0x2000 + (tile.bits(0,8) << 4) + (tileY << 1);
  uint8 d0 = iram[tileOffset++];
  uint8 d1 = iram[tileOffset++];

  uint8 tileMask = 0x80 >> tileX;
  uint2 tileColor = (d0 & tileMask ? 1 : 0) | (d1 & tileMask ? 2 : 0);

  uint3 paletteColor = r.palette[tile.bits(9,12)].color[tileColor];
  uint4 poolColor = 15 - r.pool[paletteColor];

  pixel = {Pixel::Source::ScreenOne, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderScreenTwo() -> void {
  if(!r.screenTwoEnable) return;

  bool windowInside = (
     status.hclk >= r.screenTwoWindowX0
  && status.hclk <= r.screenTwoWindowX1
  && status.vclk >= r.screenTwoWindowY0
  && status.vclk <= r.screenTwoWindowY1
  );
  windowInside ^= r.screenTwoWindowInvert;
  if(r.screenTwoWindowEnable && !windowInside) return;

  uint8 scrollX = status.hclk + r.scrollTwoX;
  uint8 scrollY = status.vclk + r.scrollTwoY;

  uint14 tilemapOffset = r.screenTwoMapBase << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile;
  tile.byte(0) = iram[tilemapOffset++];
  tile.byte(1) = iram[tilemapOffset++];

  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);

  uint14 tileOffset = 0x2000 + (tile.bits(0,8) << 4) + (tileY << 1);
  uint8 d0 = iram[tileOffset++];
  uint8 d1 = iram[tileOffset++];

  uint8 tileMask = 0x80 >> tileX;
  uint2 tileColor = (d0 & tileMask ? 1 : 0) | (d1 & tileMask ? 2 : 0);

  if(tile.bit(11) && tileColor == 0) return;
  uint3 paletteColor = r.palette[tile.bits(9,12)].color[tileColor];
  uint4 poolColor = 15 - r.pool[paletteColor];

  pixel = {Pixel::Source::ScreenTwo, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderSprite() -> void {
  if(!r.spriteEnable) return;

  bool windowInside = (
     status.hclk >= r.spriteWindowX0
  && status.hclk <= r.spriteWindowX1
  && status.vclk >= r.spriteWindowY0
  && status.vclk <= r.spriteWindowY1
  );

  uint14 spriteBase = r.spriteBase << 9;

  uint7 spriteIndex = r.spriteFirst;
  uint8 spriteCount = min(128, (uint)r.spriteCount);
  while(spriteCount--) {
    uint32 sprite;
    sprite.byte(0) = iram[spriteBase + (spriteIndex << 2) + 0];
    sprite.byte(1) = iram[spriteBase + (spriteIndex << 2) + 1];
    sprite.byte(2) = iram[spriteBase + (spriteIndex << 2) + 2];
    sprite.byte(3) = iram[spriteBase + (spriteIndex << 2) + 3];
    spriteIndex++;

    if(r.spriteWindowEnable && sprite.bit(12) && !windowInside) continue;

    uint8 spriteY = sprite.bits(16,23);
    uint8 spriteX = sprite.bits(24,31);

    if(status.hclk < spriteX) continue;
    if(status.hclk > (uint8)(spriteX + 7)) continue;
    if(status.vclk < spriteY) continue;
    if(status.vclk > (uint8)(spriteY + 7)) continue;

    uint3 tileX = (uint8)(status.hclk - spriteX) ^ (sprite.bit(14) ? 7 : 0);
    uint3 tileY = (uint8)(status.vclk - spriteY) ^ (sprite.bit(15) ? 7 : 0);

    uint14 tileOffset = 0x2000 + (sprite.bits(0,8) << 4) + (tileY << 1);
    uint8 d0 = iram[tileOffset++];
    uint8 d1 = iram[tileOffset++];

    uint8 tileMask = 0x80 >> tileX;
    uint2 tileColor = (d0 & tileMask ? 1 : 0) | (d1 & tileMask ? 2 : 0);

    if(sprite.bit(11) && tileColor == 0) continue;
    if(!sprite.bit(13) && pixel.source == Pixel::Source::ScreenTwo) continue;
    uint3 paletteColor = r.palette[8 + sprite.bits(9,11)].color[tileColor];
    uint4 poolColor = 15 - r.pool[paletteColor];

    pixel = {Pixel::Source::Sprite, poolColor << 0 | poolColor << 4 | poolColor << 8};
    return;
  }
}
