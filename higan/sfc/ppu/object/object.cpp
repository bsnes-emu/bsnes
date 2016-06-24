#include "list.cpp"

auto PPU::Object::addressReset() -> void {
  ppu.r.oamAddress = ppu.r.oamBaseAddress;
  setFirstSprite();
}

auto PPU::Object::setFirstSprite() -> void {
  r.firstSprite = !ppu.r.oamPriority ? 0 : ppu.r.oamAddress >> 2;
}

auto PPU::Object::frame() -> void {
  r.timeOver = false;
  r.rangeOver = false;
}

auto PPU::Object::scanline() -> void {
  t.x = 0;
  t.y = ppu.vcounter();

  t.itemCount = 0;
  t.tileCount = 0;

  t.active = !t.active;
  auto oamItem = t.item[t.active];
  auto oamTile = t.tile[t.active];

  if(t.y == ppu.vdisp() && !ppu.r.displayDisable) addressReset();
  if(t.y >= ppu.vdisp() - 1) return;

  for(auto n : range(32)) oamItem[n].valid = false;  //default to invalid
  for(auto n : range(34)) oamTile[n].valid = false;  //default to invalid

  for(auto n : range(128)) {
    uint7 sprite = r.firstSprite + n;
    if(!onScanline(list[sprite])) continue;
    if(t.itemCount++ >= 32) break;
    oamItem[t.itemCount - 1] = {true, sprite};
  }

  if(t.itemCount > 0 && oamItem[t.itemCount - 1].valid) {
    ppu.latch.oamAddress = 0x0200 + (oamItem[t.itemCount - 1].index >> 2);
  }
}

auto PPU::Object::onScanline(Sprite& sprite) -> bool {
  if(sprite.x > 256 && (sprite.x + sprite.width() - 1) < 512) return false;
  int height = sprite.height() >> r.interlace;
  if(t.y >= sprite.y && t.y < (sprite.y + height)) return true;
  if((sprite.y + height) >= 256 && t.y < ((sprite.y + height) & 255)) return true;
  return false;
}

auto PPU::Object::run() -> void {
  output.above.priority = 0;
  output.below.priority = 0;

  auto oamTile = t.tile[!t.active];
  uint x = t.x++;

  for(auto n : range(34)) {
    const auto& tile = oamTile[n];
    if(!tile.valid) break;

    int px = x - (int9)tile.x;
    if(px & ~7) continue;

    uint color = 0, shift = tile.hflip ? px : 7 - px;
    color += tile.data >> (shift +  0) & 1;
    color += tile.data >> (shift +  7) & 2;
    color += tile.data >> (shift + 14) & 4;
    color += tile.data >> (shift + 21) & 8;

    if(color) {
      if(r.aboveEnable) {
        output.above.palette = tile.palette + color;
        output.above.priority = r.priority[tile.priority];
      }

      if(r.belowEnable) {
        output.below.palette = tile.palette + color;
        output.below.priority = r.priority[tile.priority];
      }
    }
  }
}

auto PPU::Object::tilefetch() -> void {
  auto oamItem = t.item[t.active];
  auto oamTile = t.tile[t.active];

  for(int i = 31; i >= 0; i--) {
    if(!oamItem[i].valid) continue;
    const auto& sprite = list[oamItem[i].index];

    uint tileWidth = sprite.width() >> 3;
    int x = sprite.x;
    int y = (t.y - sprite.y) & 0xff;
    if(r.interlace) y <<= 1;

    if(sprite.vflip) {
      if(sprite.width() == sprite.height()) {
        y = (sprite.height() - 1) - y;
      } else if(y < sprite.width()) {
        y = (sprite.width() - 1) - y;
      } else {
        y = sprite.width() + ((sprite.width() - 1) - (y - sprite.width()));
      }
    }

    if(r.interlace) {
      y = !sprite.vflip ? y + ppu.field() : y - ppu.field();
    }

    x &= 511;
    y &= 255;

    uint16 tiledataAddress = r.tiledataAddress;
    uint16 chrx = (sprite.character >> 0) & 15;
    uint16 chry = (sprite.character >> 4) & 15;
    if(sprite.nameSelect) {
      tiledataAddress += (256 * 16) + (r.nameSelect << 12);
    }
    chry  += (y >> 3);
    chry  &= 15;
    chry <<= 4;

    for(uint tx : range(tileWidth)) {
      uint sx = (x + (tx << 3)) & 511;
      if(x != 256 && sx >= 256 && (sx + 7) < 512) continue;
      if(t.tileCount++ >= 34) break;

      uint n = t.tileCount - 1;
      oamTile[n].valid = true;
      oamTile[n].x = sx;
      oamTile[n].priority = sprite.priority;
      oamTile[n].palette = 128 + (sprite.palette << 4);
      oamTile[n].hflip = sprite.hflip;

      uint mx = !sprite.hflip ? tx : (tileWidth - 1) - tx;
      uint pos = tiledataAddress + ((chry + ((chrx + mx) & 15)) << 4);
      uint15 addr = (pos & 0x7ff0) + (y & 7);

      oamTile[n].data.bits( 0,15) = ppu.vram[addr + 0];
      ppu.addClocks(2);

      oamTile[n].data.bits(16,31) = ppu.vram[addr + 8];
      ppu.addClocks(2);
    }
  }

  if(t.tileCount < 34) ppu.addClocks((34 - t.tileCount) * 4);
  r.timeOver  |= (t.tileCount > 34);
  r.rangeOver |= (t.itemCount > 32);
}

auto PPU::Object::reset() -> void {
  for(auto n : range(128)) {
    list[n].x = 0;
    list[n].y = 0;
    list[n].character = 0;
    list[n].nameSelect = 0;
    list[n].vflip = 0;
    list[n].hflip = 0;
    list[n].priority = 0;
    list[n].palette = 0;
    list[n].size = 0;
  }
  synchronize();

  t.x = 0;
  t.y = 0;

  t.itemCount = 0;
  t.tileCount = 0;

  t.active = 0;
  for(auto p : range(2)) {
    for(auto n : range(32)) {
      t.item[p][n].valid = false;
      t.item[p][n].index = 0;
    }
    for(auto n : range(34)) {
      t.tile[p][n].valid = false;
      t.tile[p][n].x = 0;
      t.tile[p][n].priority = 0;
      t.tile[p][n].palette = 0;
      t.tile[p][n].hflip = 0;
      t.tile[p][n].data = 0;
    }
  }

  r.aboveEnable = random(false);
  r.belowEnable = random(false);
  r.interlace = random(false);

  r.baseSize = random(0);
  r.nameSelect = random(0);
  r.tiledataAddress = (random(0x0000) & 3) << 13;
  r.firstSprite = 0;

  for(auto& p : r.priority) p = 0;

  r.timeOver = false;
  r.rangeOver = false;

  output.above.palette = 0;
  output.above.priority = 0;
  output.below.palette = 0;
  output.below.priority = 0;
}
