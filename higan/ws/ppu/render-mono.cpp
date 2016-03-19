auto PPU::renderMonoFetch(uint14 offset, uint3 y, uint3 x) -> uint2 {
  uint2 color;

  if(system.planar()) {
    uint16 data = iram.read(offset + (y << 1), Word);
    color |= data.bit( 7 - x) << 0;
    color |= data.bit(15 - x) << 1;
  }

  if(system.packed()) {
    uint8 data = iram.read(offset + (y << 1) + (x >> 2));
    color = data >> (6 - (x.bits(0,1) << 1));
  }

  return color;
}

auto PPU::renderMonoPalette(uint4 palette, uint2 index) -> uint12 {
  uint3 paletteColor = r.palette[palette].color[index];
  uint4 poolColor = 15 - r.pool[paletteColor];
  return poolColor << 0 | poolColor << 4 | poolColor << 8;
}

auto PPU::renderMonoBack() -> void {
  uint4 poolColor = 15 - r.pool[l.backColor.bits(0,2)];
  pixel = {Pixel::Source::Back, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderMonoScreenOne() -> void {
  if(!l.screenOneEnable) return;

  uint8 scrollY = s.vclk + l.scrollOneY;
  uint8 scrollX = s.hclk + l.scrollOneX;

  uint14 tilemapOffset = l.screenOneMapBase.bits(0,2) << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint14 tileOffset = 0x2000 + (tile.bits(0,8) << 4);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint2 tileColor = renderMonoFetch(tileOffset, tileY, tileX);
  if(tile.bit(11) && tileColor == 0) return;

  pixel = {Pixel::Source::ScreenOne, renderMonoPalette(tile.bits(9,12), tileColor)};
}

auto PPU::renderMonoScreenTwo() -> void {
  if(!l.screenTwoEnable) return;

  bool windowInside = s.vclk >= l.screenTwoWindowY0 && s.vclk <= l.screenTwoWindowY1
                   && s.hclk >= l.screenTwoWindowX0 && s.hclk <= l.screenTwoWindowX1;
  windowInside ^= l.screenTwoWindowInvert;
  if(l.screenTwoWindowEnable && !windowInside) return;

  uint8 scrollY = s.vclk + l.scrollTwoY;
  uint8 scrollX = s.hclk + l.scrollTwoX;

  uint14 tilemapOffset = l.screenTwoMapBase.bits(0,2) << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint14 tileOffset = 0x2000 + (tile.bits(0,8) << 4);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint2 tileColor = renderMonoFetch(tileOffset, tileY, tileX);
  if(tile.bit(11) && tileColor == 0) return;

  pixel = {Pixel::Source::ScreenTwo, renderMonoPalette(tile.bits(9,12), tileColor)};
}

auto PPU::renderMonoSprite() -> void {
  if(!l.spriteEnable) return;

  bool windowInside = s.hclk >= l.spriteWindowX0 && s.hclk <= l.spriteWindowX1;
  for(auto& sprite : sprites) {
    if(l.spriteWindowEnable && sprite.window == windowInside) continue;
    if((uint8)(s.hclk - sprite.x) > 7) continue;

    uint14 tileOffset = 0x2000 + (sprite.tile << 4);
    uint3 tileY = (uint8)(s.vclk - sprite.y) ^ (sprite.vflip ? 7 : 0);
    uint3 tileX = (uint8)(s.hclk - sprite.x) ^ (sprite.hflip ? 7 : 0);
    uint2 tileColor = renderMonoFetch(tileOffset, tileY, tileX);
    if(sprite.palette.bit(2) && tileColor == 0) continue;
    if(!sprite.priority && pixel.source == Pixel::Source::ScreenTwo) continue;

    pixel = {Pixel::Source::Sprite, renderMonoPalette(sprite.palette, tileColor)};
    break;
  }
}
