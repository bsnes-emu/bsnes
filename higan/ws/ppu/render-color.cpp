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

auto PPU::renderColorPalette(uint4 palette, uint4 index) -> uint12 {
  return iram.read(0xfe00 + (palette << 5) + (index << 1), Word);
}

auto PPU::renderColorBack() -> void {
  uint12 color = iram.read(0xfe00 + (l.backColor << 1), Word);
  pixel = {Pixel::Source::Back, color};
}

auto PPU::renderColorScreenOne() -> void {
  if(!l.screenOneEnable) return;

  uint8 scrollY = s.vclk + l.scrollOneY;
  uint8 scrollX = s.hclk + l.scrollOneX;

  uint16 tilemapOffset = l.screenOneMapBase << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint16 tileOffset = 0x4000 + (tile.bit(13) << 14) + (tile.bits(0,8) << 5);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint4 tileColor = renderColorFetch(tileOffset, tileY, tileX);
  if(tileColor == 0) return;

  pixel = {Pixel::Source::ScreenOne, renderColorPalette(tile.bits(9, 12), tileColor)};
}

auto PPU::renderColorScreenTwo() -> void {
  if(!l.screenTwoEnable) return;

  bool windowInside = s.vclk >= l.screenTwoWindowY0 && s.vclk <= l.screenTwoWindowY1
                   && s.hclk >= l.screenTwoWindowX0 && s.hclk <= l.screenTwoWindowX1;
  windowInside ^= l.screenTwoWindowInvert;
  if(l.screenTwoWindowEnable && !windowInside) return;

  uint8 scrollY = s.vclk + l.scrollTwoY;
  uint8 scrollX = s.hclk + l.scrollTwoX;

  uint16 tilemapOffset = l.screenTwoMapBase << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint16 tileOffset = 0x4000 + (tile.bit(13) << 14) + (tile.bits(0,8) << 5);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint4 tileColor = renderColorFetch(tileOffset, tileY, tileX);
  if(tileColor == 0) return;

  pixel = {Pixel::Source::ScreenTwo, renderColorPalette(tile.bits(9, 12), tileColor)};
}

auto PPU::renderColorSprite() -> void {
  if(!l.spriteEnable) return;

  bool windowInside = s.hclk >= l.spriteWindowX0 && s.hclk <= l.spriteWindowX1;
  for(auto& sprite : sprites) {
    if(l.spriteWindowEnable && sprite.window == windowInside) continue;
    if((uint8)(s.hclk - sprite.x) > 7) continue;

    uint16 tileOffset = 0x4000 + (sprite.tile << 5);
    uint3 tileY = (uint8)(s.vclk - sprite.y) ^ (sprite.vflip ? 7 : 0);
    uint3 tileX = (uint8)(s.hclk - sprite.x) ^ (sprite.hflip ? 7 : 0);
    uint4 tileColor = renderColorFetch(tileOffset, tileY, tileX);
    if(tileColor == 0) continue;
    if(!sprite.priority && pixel.source == Pixel::Source::ScreenTwo) continue;

    pixel = {Pixel::Source::Sprite, renderColorPalette(sprite.palette, tileColor)};
    break;
  }
}
