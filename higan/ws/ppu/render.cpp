auto PPU::renderFetch(uint10 tile, uint3 y, uint3 x) -> uint4 {
  uint16 offset = 0x200 + tile << (4 + system.depth());
  uint4 color = 0;

  if(system.planar()) {
    if(!system.depth()) {
      uint16 data = iram.read16(offset + (y << 1));
      color |= data.bit( 7 - x) << 0;
      color |= data.bit(15 - x) << 1;
    } else {
      uint32 data = iram.read32(offset + (y << 2));
      color |= data.bit( 7 - x) << 0;
      color |= data.bit(15 - x) << 1;
      color |= data.bit(23 - x) << 2;
      color |= data.bit(31 - x) << 3;
    }
  }

  if(system.packed()) {
    if(!system.depth()) {
      uint8 data = iram.read8(offset + (y << 1) + (x >> 2));
      color = data >> (6 - (x.bits(0,1) << 1));
      color = color.bits(0,1);
    } else {
      uint8 data = iram.read8(offset + (y << 2) + (x >> 1));
      color = data >> (4 - (x.bit(0) << 2));
    }
  }

  return color;
}

auto PPU::renderTransparent(bool palette, uint4 color) -> bool {
  if(color) return false;
  if(!system.depth() && !palette) return false;
  return true;
}

auto PPU::renderPalette(uint4 palette, uint4 color) -> uint12 {
  if(!system.color()) {
    uint3 paletteColor = r.palette[palette].color[color.bits(0,1)];
    uint4 poolColor = 15 - r.pool[paletteColor];
    return poolColor << 0 | poolColor << 4 | poolColor << 8;
  } else {
    return iram.read16(0xfe00 + (palette << 5) + (color << 1));
  }
}

auto PPU::renderBack() -> void {
  if(!system.color()) {
    uint4 poolColor = 15 - r.pool[l.backColor.bits(0,2)];
    s.pixel = {Pixel::Source::Back, poolColor << 0 | poolColor << 4 | poolColor << 8};
  } else {
    uint12 color = iram.read16(0xfe00 + (l.backColor << 1));
    s.pixel = {Pixel::Source::Back, color};
  }
}

auto PPU::renderScreenOne(uint8 x, uint8 y) -> void {
  uint8 scrollY = y + l.scrollOneY;
  uint8 scrollX = x + l.scrollOneX;

  uint16 tilemapOffset = l.screenOneMapBase.bits(0, 2 + system.depth()) << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read16(tilemapOffset);
  uint3 tileY = scrollY ^ tile.bit(15) * 7;
  uint3 tileX = scrollX ^ tile.bit(14) * 7;
  uint4 tileColor = renderFetch((tile.bit(13) & system.depth()) << 9 | tile.bits(0,8), tileY, tileX);
  if(renderTransparent(tile.bit(11), tileColor)) return;

  s.pixel = {Pixel::Source::ScreenOne, renderPalette(tile.bits(9,12), tileColor)};
}

auto PPU::renderScreenTwo(uint8 x, uint8 y) -> void {
  bool windowInside = y >= l.screenTwoWindowY0 && y <= l.screenTwoWindowY1
                   && x >= l.screenTwoWindowX0 && x <= l.screenTwoWindowX1;
  windowInside ^= l.screenTwoWindowInvert;
  if(l.screenTwoWindowEnable && !windowInside) return;

  uint8 scrollY = y + l.scrollTwoY;
  uint8 scrollX = x + l.scrollTwoX;

  uint16 tilemapOffset = l.screenTwoMapBase.bits(0, 2 + system.depth()) << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read16(tilemapOffset);
  uint3 tileY = scrollY ^ tile.bit(15) * 7;
  uint3 tileX = scrollX ^ tile.bit(14) * 7;
  uint4 tileColor = renderFetch((tile.bit(13) & system.depth()) << 9 | tile.bits(0,8), tileY, tileX);
  if(renderTransparent(tile.bit(11), tileColor)) return;

  s.pixel = {Pixel::Source::ScreenTwo, renderPalette(tile.bits(9,12), tileColor)};
}

auto PPU::renderSprite(uint8 x, uint8 y) -> void {
  bool windowInside = y >= l.spriteWindowY0 && y <= l.spriteWindowY1
                   && x >= l.spriteWindowX0 && x <= l.spriteWindowX1;
  for(auto index : range(l.spriteCount)) {
    auto sprite = l.sprite[index];
    if(l.spriteWindowEnable && sprite.bit(12) == windowInside) continue;
    if((uint8)(x - sprite.bits(24,31)) > 7) continue;

    uint3 tileY = (y - sprite.bits(16,23)) ^ sprite.bit(15) * 7;
    uint3 tileX = (x - sprite.bits(24,31)) ^ sprite.bit(14) * 7;
    uint4 tileColor = renderFetch(sprite.bits(0,8), tileY, tileX);
    if(renderTransparent(sprite.bit(11), tileColor)) continue;
    if(!sprite.bit(13) && s.pixel.source == Pixel::Source::ScreenTwo) continue;

    s.pixel = {Pixel::Source::Sprite, renderPalette(8 + sprite.bits(9,11), tileColor)};
    break;
  }
}
