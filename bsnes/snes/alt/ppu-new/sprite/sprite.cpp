#ifdef PPU_CPP

void PPU::Sprite::update_list(unsigned addr, uint8 data) {
  if(addr < 0x0200) {
    unsigned i = addr >> 2;
    switch(addr & 3) {
      case 0: list[i].x = (list[i].x & 0x0100) | data; break;
      case 1: list[i].y = (data + 1) & 0xff; break;
      case 2: list[i].character = data; break;
      case 3: list[i].vflip = data & 0x80;
              list[i].hflip = data & 0x40;
              list[i].priority = (data >> 4) & 3;
              list[i].palette = (data >> 1) & 7;
              list[i].use_nameselect = data & 0x01;
              break;
    }
  } else {
    unsigned i = (addr & 0x1f) << 2;
    list[i + 0].x = ((data & 0x01) << 8) | (list[i + 0].x & 0xff);
    list[i + 0].size = data & 0x02;
    list[i + 1].x = ((data & 0x04) << 8) | (list[i + 1].x & 0xff);
    list[i + 1].size = data & 0x08;
    list[i + 2].x = ((data & 0x10) << 8) | (list[i + 2].x & 0xff);
    list[i + 2].size = data & 0x20;
    list[i + 3].x = ((data & 0x40) << 8) | (list[i + 3].x & 0xff);
    list[i + 3].size = data & 0x80;
  }
}

void PPU::Sprite::address_reset() {
  self.regs.oam_addr = self.regs.oam_baseaddr << 1;
  set_first();
}

void PPU::Sprite::set_first() {
  regs.first_sprite = (self.regs.oam_priority == false ? 0 : (self.regs.oam_addr >> 2) & 127);
}

bool PPU::Sprite::on_scanline(unsigned sprite) {
  auto &s = list[sprite];
  if(s.x > 256 && (s.x + s.width - 1) < 512) return false;
  signed height = (regs.interlace == false ? s.height : s.height >> 1);
  if(self.vcounter() >= s.y && self.vcounter() < (s.y + height)) return true;
  if((s.y + height) >= 256 && self.vcounter() < ((s.y + height) & 255)) return true;
  return false;
}

void PPU::Sprite::render() {
  for(unsigned i = 0; i < 128; i++) {
    if(list[i].size == 0) {
      static unsigned width[]  = { 8, 8, 8, 16, 16, 32, 16, 16 };
      static unsigned height[] = { 8, 8, 8, 16, 16, 32, 32, 32 };
      list[i].width = width[regs.base_size];
      list[i].height = height[regs.base_size];
    } else {
      static unsigned width[]  = { 16, 32, 64, 32, 64, 64, 32, 32 };
      static unsigned height[] = { 16, 32, 64, 32, 64, 64, 64, 32 };
      list[i].width = width[regs.base_size];
      list[i].height = height[regs.base_size];
      if(regs.interlace && regs.base_size >= 6) list[i].height = 16;
    }
  }

  unsigned itemcount = 0;
  unsigned tilecount = 0;
  memset(output.priority, 0xff, 256);
  memset(itemlist, 0xff, 32);
  for(unsigned i = 0; i < 34; i++) tilelist[i].tile = 0xffff;

  for(unsigned i = 0; i < 128; i++) {
    unsigned s = (regs.first_sprite + i) & 127;
    if(on_scanline(s) == false) continue;
    if(itemcount++ >= 32) break;
    itemlist[itemcount - 1] = s;
  }

  for(signed i = 31; i >= 0; i--) {
    if(itemlist[i] == 0xff) continue;
    auto &s = list[itemlist[i]];
    unsigned tile_width = s.width >> 3;
    signed x = s.x;
    signed y = (self.vcounter() - s.y) & 0xff;
    if(regs.interlace) y <<= 1;

    if(s.vflip) {
      if(s.width == s.height) {
        y = (s.height - 1) - y;
      } else {
        y = (y < s.width) ? ((s.width - 1) - y) : (s.width + ((s.width - 1) - (y - s.width)));
      }
    }

    if(regs.interlace) {
      y = (s.vflip == false) ? (y + self.field()) : (y - self.field());
    }

    x &= 511;
    y &= 255;

    uint16 tdaddr = regs.tiledata_addr;
    uint16 chrx   = (s.character >> 0) & 15;
    uint16 chry   = (s.character >> 4) & 15;
    if(s.use_nameselect) {
      tdaddr += (256 * 32) + (regs.nameselect << 13);
    }
    chry += (y >> 3);
    chry &= 15;
    chry <<= 4;

    for(unsigned tx = 0; tx < tile_width; tx++) {
      unsigned sx = (x + (tx << 3)) & 511;
      if(x != 256 && sx >= 256 && (sx + 7) < 512) continue;
      if(tilecount++ >= 34) break;

      unsigned n = tilecount - 1;
      tilelist[n].x = sx;
      tilelist[n].y = y;
      tilelist[n].priority = s.priority;
      tilelist[n].palette = 128 + (s.palette << 4);
      tilelist[n].hflip = s.hflip;

      unsigned mx = (s.hflip == false) ? tx : ((tile_width - 1) - tx);
      unsigned pos = tdaddr + ((chry + ((chrx + mx) & 15)) << 5);
      tilelist[n].tile = (pos >> 5) & 0x07ff;
    }
  }

  regs.time_over  |= (tilecount > 34);
  regs.range_over |= (itemcount > 32);
}

PPU::Sprite::Sprite(PPU &self) : self(self) {
}

#endif
