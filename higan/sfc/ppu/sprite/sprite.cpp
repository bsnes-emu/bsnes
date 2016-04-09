#include "list.cpp"

PPU::Sprite::Sprite(PPU& self) : self(self) {
}

auto PPU::Sprite::address_reset() -> void {
  self.regs.oam_addr = self.regs.oam_baseaddr;
  set_first_sprite();
}

auto PPU::Sprite::set_first_sprite() -> void {
  regs.first_sprite = !self.regs.oam_priority ? 0 : self.regs.oam_addr >> 2;
}

auto PPU::Sprite::frame() -> void {
  regs.time_over = false;
  regs.range_over = false;
}

auto PPU::Sprite::scanline() -> void {
  t.x = 0;
  t.y = self.vcounter();

  t.item_count = 0;
  t.tile_count = 0;

  t.active = !t.active;
  auto oam_item = t.item[t.active];
  auto oam_tile = t.tile[t.active];

  if(t.y == self.vdisp() && !self.regs.display_disable) address_reset();
  if(t.y >= self.vdisp() - 1) return;

  for(auto n : range(32)) oam_item[n].valid = false;  //default to invalid
  for(auto n : range(34)) oam_tile[n].valid = false;  //default to invalid

  for(auto n : range(128)) {
    uint7 sprite = regs.first_sprite + n;
    if(!on_scanline(list[sprite])) continue;
    if(t.item_count++ >= 32) break;
    oam_item[t.item_count - 1] = {true, sprite};
  }

  if(t.item_count > 0 && oam_item[t.item_count - 1].valid) {
    ppu.regs.oam_iaddr = 0x0200 + (oam_item[t.item_count - 1].index >> 2);
  }
}

auto PPU::Sprite::on_scanline(Object& sprite) -> bool {
  if(sprite.x > 256 && (sprite.x + sprite.width() - 1) < 512) return false;
  int height = sprite.height() >> regs.interlace;
  if(t.y >= sprite.y && t.y < (sprite.y + height)) return true;
  if((sprite.y + height) >= 256 && t.y < ((sprite.y + height) & 255)) return true;
  return false;
}

auto PPU::Sprite::run() -> void {
  output.main.priority = 0;
  output.sub.priority = 0;

  auto oam_tile = t.tile[!t.active];
  uint priority_table[] = {regs.priority0, regs.priority1, regs.priority2, regs.priority3};
  uint x = t.x++;

  for(auto n : range(34)) {
    auto tile = oam_tile[n];
    if(!tile.valid) break;

    int px = x - sclip<9>(tile.x);
    if(px & ~7) continue;

    uint mask = 0x80 >> (!tile.hflip ? px : 7 - px);
    uint color;
    color  = ((bool)(tile.d0 & mask)) << 0;
    color |= ((bool)(tile.d1 & mask)) << 1;
    color |= ((bool)(tile.d2 & mask)) << 2;
    color |= ((bool)(tile.d3 & mask)) << 3;

    if(color) {
      if(regs.main_enable) {
        output.main.palette = tile.palette + color;
        output.main.priority = priority_table[tile.priority];
      }

      if(regs.sub_enable) {
        output.sub.palette = tile.palette + color;
        output.sub.priority = priority_table[tile.priority];
      }
    }
  }
}

auto PPU::Sprite::tilefetch() -> void {
  auto oam_item = t.item[t.active];
  auto oam_tile = t.tile[t.active];

  for(int i = 31; i >= 0; i--) {
    if(!oam_item[i].valid) continue;
    auto sprite = list[oam_item[i].index];

    uint tile_width = sprite.width() >> 3;
    int x = sprite.x;
    int y = (t.y - sprite.y) & 0xff;
    if(regs.interlace) y <<= 1;

    if(sprite.vflip) {
      if(sprite.width() == sprite.height()) {
        y = (sprite.height() - 1) - y;
      } else if(y < sprite.width()) {
        y = (sprite.width() - 1) - y;
      } else {
        y = sprite.width() + ((sprite.width() - 1) - (y - sprite.width()));
      }
    }

    if(regs.interlace) {
      y = !sprite.vflip ? y + self.field() : y - self.field();
    }

    x &= 511;
    y &= 255;

    uint16 tiledata_addr = regs.tiledata_addr;
    uint16 chrx = (sprite.character >> 0) & 15;
    uint16 chry = (sprite.character >> 4) & 15;
    if(sprite.nameselect) {
      tiledata_addr += (256 * 32) + (regs.nameselect << 13);
    }
    chry  += (y >> 3);
    chry  &= 15;
    chry <<= 4;

    for(uint tx = 0; tx < tile_width; tx++) {
      uint sx = (x + (tx << 3)) & 511;
      if(x != 256 && sx >= 256 && (sx + 7) < 512) continue;
      if(t.tile_count++ >= 34) break;

      uint n = t.tile_count - 1;
      oam_tile[n].valid = true;
      oam_tile[n].x = sx;
      oam_tile[n].priority = sprite.priority;
      oam_tile[n].palette = 128 + (sprite.palette << 4);
      oam_tile[n].hflip = sprite.hflip;

      uint mx = !sprite.hflip ? tx : (tile_width - 1) - tx;
      uint pos = tiledata_addr + ((chry + ((chrx + mx) & 15)) << 5);
      uint16 addr = (pos & 0xffe0) + ((y & 7) * 2);

      oam_tile[n].d0 = ppu.vram[addr +  0];
      oam_tile[n].d1 = ppu.vram[addr +  1];
      self.addClocks(2);

      oam_tile[n].d2 = ppu.vram[addr + 16];
      oam_tile[n].d3 = ppu.vram[addr + 17];
      self.addClocks(2);
    }
  }

  if(t.tile_count < 34) self.addClocks((34 - t.tile_count) * 4);
  regs.time_over  |= (t.tile_count > 34);
  regs.range_over |= (t.item_count > 32);
}

auto PPU::Sprite::reset() -> void {
  for(auto n : range(128)) {
    list[n].x = 0;
    list[n].y = 0;
    list[n].character = 0;
    list[n].nameselect = 0;
    list[n].vflip = 0;
    list[n].hflip = 0;
    list[n].priority = 0;
    list[n].palette = 0;
    list[n].size = 0;
  }
  synchronize();

  t.x = 0;
  t.y = 0;

  t.item_count = 0;
  t.tile_count = 0;

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
      t.tile[p][n].d0 = 0;
      t.tile[p][n].d1 = 0;
      t.tile[p][n].d2 = 0;
      t.tile[p][n].d3 = 0;
    }
  }

  regs.main_enable = random(false);
  regs.sub_enable = random(false);
  regs.interlace = random(false);

  regs.base_size = random(0);
  regs.nameselect = random(0);
  regs.tiledata_addr = (random(0x0000) & 3) << 14;
  regs.first_sprite = 0;

  regs.priority0 = 0;
  regs.priority1 = 0;
  regs.priority2 = 0;
  regs.priority3 = 0;

  regs.time_over = false;
  regs.range_over = false;

  output.main.palette = 0;
  output.main.priority = 0;
  output.sub.palette = 0;
  output.sub.priority = 0;
}
