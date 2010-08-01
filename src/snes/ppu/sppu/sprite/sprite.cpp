#ifdef SPPU_CPP

void sPPU::Sprite::address_reset() {
  self.regs.oam_addr = self.regs.oam_baseaddr << 1;
  regs.first_sprite = (self.regs.oam_priority == false ? 0 : (self.regs.oam_addr >> 2) & 127);
}

void sPPU::Sprite::frame() {
  regs.time_over = false;
  regs.range_over = false;
}

void sPPU::Sprite::scanline() {
  t.x = 0;
  t.y = self.vcounter();

  if(t.y == (!self.regs.overscan ? 225 : 240) && self.regs.display_disabled == false) address_reset();
  if(t.y < 1 || t.y > (!self.regs.overscan ? 224 : 239)) return;

  const uint8 *tableA = memory::oam.data();
  const uint8 *tableB = memory::oam.data() + 512;

  for(unsigned i = 0; i < 128; i++) {
    bool x = *tableB & (1 << ((i & 3) << 1));
    bool size = *tableB & (2 << ((i & 3) << 1));

    switch(regs.base_size) {
      case 0: list[i].width  = (!size ?  8 : 16);
              list[i].height = (!size ?  8 : 16);
              break;
      case 1: list[i].width  = (!size ?  8 : 32);
              list[i].height = (!size ?  8 : 32);
              break;
      case 2: list[i].width  = (!size ?  8 : 64);
              list[i].height = (!size ?  8 : 64);
              break;
      case 3: list[i].width  = (!size ? 16 : 32);
              list[i].height = (!size ? 16 : 32);
              break;
      case 4: list[i].width  = (!size ? 16 : 64);
              list[i].height = (!size ? 16 : 64);
              break;
      case 5: list[i].width  = (!size ? 32 : 64);
              list[i].height = (!size ? 32 : 64);
              break;
      case 6: list[i].width  = (!size ? 16 : 32);
              list[i].height = (!size ? 32 : 64);
              if(regs.interlace && !size) list[i].height = 16;
              break;
      case 7: list[i].width  = (!size ? 16 : 32);
              list[i].height = (!size ? 32 : 32);
              if(regs.interlace && !size) list[i].height = 16;
              break;
    }

    list[i].x = (x << 8) + tableA[0];
    list[i].y = (tableA[1] + 1) & 0xff;
    list[i].character = tableA[2];
    list[i].vflip = tableA[3] & 0x80;
    list[i].hflip = tableA[3] & 0x40;
    list[i].priority = (tableA[3] >> 4) & 3;
    list[i].palette = (tableA[3] >> 1) & 7;
    list[i].nameselect = tableA[3] & 1;

    tableA += 4;
    if((i & 3) == 3) tableB++;
  }

  t.item_count = 0;
  t.tile_count = 0;
  memset(t.output_priority, 0xff, 256);
  memset(t.item_list, 0xff, 32);
  for(unsigned i = 0; i < 34; i++) t.tile_list[i].tile = 0xffff;

  for(unsigned i = 0; i < 128; i++) {
    t.active_sprite = (i + regs.first_sprite) & 127;
    if(on_scanline() == false) continue;
    if(t.item_count++ >= 32) break;
    t.item_list[t.item_count - 1] = (i + regs.first_sprite) & 127;
  }

  if(t.item_count > 0 && t.item_list[t.item_count - 1] != 0xff) {
    ppu.regs.ioamaddr = 0x0200 + (t.item_list[t.item_count - 1] >> 2);
  }

  for(signed i = 31; i >= 0; i--) {
    if(t.item_list[i] == 0xff) continue;
    t.active_sprite = t.item_list[i];
    load_tiles();
  }

  regs.time_over  |= (t.tile_count > 34);
  regs.range_over |= (t.item_count > 32);

  for(unsigned i = 0; i < 34; i++) {
    if(t.tile_list[i].tile == 0xffff) continue;
    render_tile(i);
  }
}

void sPPU::Sprite::run() {
  output.main.priority = 0;
  output.sub.priority = 0;

  unsigned x = t.x++;
  if(t.output_priority[x] != 0xff) {
    unsigned priority_table[] = { regs.priority0, regs.priority1, regs.priority2, regs.priority3 };

    if(regs.main_enabled) {
      output.main.palette = t.output_palette[x];
      output.main.priority = priority_table[t.output_priority[x]];
    }

    if(regs.sub_enabled) {
      output.sub.palette = t.output_palette[x];
      output.sub.priority = priority_table[t.output_priority[x]];
    }
  }
}

bool sPPU::Sprite::on_scanline() {
  SpriteItem &sprite = list[t.active_sprite];
  if(sprite.x > 256 && (sprite.x + sprite.width - 1) < 512) return false;

  signed height = (regs.interlace == false ? sprite.height : (sprite.height >> 1));
  if(t.y >= sprite.y && t.y < (sprite.y + height)) return true;
  if((sprite.y + height) >= 256 && t.y < ((sprite.y + height) & 255)) return true;
  return false;
}

void sPPU::Sprite::load_tiles() {
  SpriteItem &sprite = list[t.active_sprite];
  unsigned tile_width = sprite.width >> 3;
  signed x = sprite.x;
  signed y = (t.y - sprite.y) & 0xff;
  if(regs.interlace) y <<= 1;

  if(sprite.vflip) {
    if(sprite.width == sprite.height) {
      y = (sprite.height - 1) - y;
    } else {
      y = (y < sprite.width) ? ((sprite.width - 1) - y) : (sprite.width + ((sprite.width - 1) - (y - sprite.width)));
    }
  }

  if(regs.interlace) {
    y = (sprite.vflip == false ? y + self.field() : y - self.field());
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

  for(unsigned tx = 0; tx < tile_width; tx++) {
    unsigned sx = (x + (tx << 3)) & 511;
    if(x != 256 && sx >= 256 && (sx + 7) < 512) continue;

    if(t.tile_count++ >= 34) break;
    unsigned n = t.tile_count - 1;
    t.tile_list[n].x = sx;
    t.tile_list[n].y = y;
    t.tile_list[n].priority = sprite.priority;
    t.tile_list[n].palette = 128 + (sprite.palette << 4);
    t.tile_list[n].hflip = sprite.hflip;

    unsigned mx = (sprite.hflip == false) ? tx : ((tile_width - 1) - tx);
    unsigned pos = tiledata_addr + ((chry + ((chrx + mx) & 15)) << 5);
    t.tile_list[n].tile = (pos >> 5) & 0x07ff;
  }
}

void sPPU::Sprite::render_tile(unsigned tile) {
  TileItem &item = t.tile_list[tile];

  unsigned sx = item.x;
  uint16 addr = (item.tile << 5) + ((item.y & 7) * 2);
  for(unsigned x = 0; x < 8; x++) {
    sx &= 511;
    if(sx < 256) {
      unsigned px = (item.hflip == false ? x : (7 - x));
      unsigned mask = 0x80 >> (px & 7);

      uint8 d0 = memory::vram[addr +  0];
      uint8 d1 = memory::vram[addr +  1];
      uint8 d2 = memory::vram[addr + 16];
      uint8 d3 = memory::vram[addr + 17];

      unsigned color;
      color  = ((bool)(d0 & mask)) << 0;
      color |= ((bool)(d1 & mask)) << 1;
      color |= ((bool)(d2 & mask)) << 2;
      color |= ((bool)(d3 & mask)) << 3;

      if(color) {
        color += item.palette;
        t.output_palette[sx] = color;
        t.output_priority[sx] = item.priority;
      }
    }
    sx++;
  }
}

void sPPU::Sprite::reset() {
  t.x = 0;
  t.y = 0;
  t.item_count = 0;
  t.tile_count = 0;
  memset(t.output_palette, 0, 256);
  memset(t.output_priority, 0, 256);
  memset(t.item_list, 0, 32);
  for(unsigned i = 0; i < 34; i++) {
    t.tile_list[i].x = 0;
    t.tile_list[i].y = 0;
    t.tile_list[i].priority = 0;
    t.tile_list[i].palette = 0;
    t.tile_list[i].tile = 0;
    t.tile_list[i].hflip = 0;
  }
  t.active_sprite = 0;

  for(unsigned i = 0; i < 128; i++) {
    list[i].width = 0;
    list[i].height = 0;
    list[i].x = 0;
    list[i].y = 0;
    list[i].character = 0;
    list[i].nameselect = 0;
    list[i].vflip = 0;
    list[i].hflip = 0;
    list[i].palette = 0;
    list[i].priority = 0;
  }

  regs.main_enabled = 0;
  regs.sub_enabled = 0;
  regs.interlace = 0;
  regs.base_size = 0;
  regs.nameselect = 0;
  regs.tiledata_addr = 0;
  regs.first_sprite = 0;
  regs.priority0 = 0;
  regs.priority1 = 0;
  regs.priority2 = 0;
  regs.priority3 = 0;
  regs.time_over = 0;
  regs.range_over = 0;

  output.main.palette = 0;
  output.main.priority = 0;
  output.sub.palette = 0;
  output.sub.priority = 0;
}

sPPU::Sprite::Sprite(sPPU &self) : self(self) {
}

#endif
