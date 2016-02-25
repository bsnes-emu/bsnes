auto PPU::Sprite::update(uint10 addr, uint8 data) -> void {
  if(!addr.bit(9)) {
    uint n = addr >> 2;  //sprite#
    addr &= 3;
    if(addr == 0) {
      list[n].x.bits(0,7) = data;
    } else if(addr == 1) {
      list[n].y = data;
    } else if(addr == 2) {
      list[n].character = data;
    } else {  //(addr == 3)
      list[n].vflip      = data.bit (7);
      list[n].hflip      = data.bit (6);
      list[n].priority   = data.bits(5,4);
      list[n].palette    = data.bits(3,1);
      list[n].nameselect = data.bit (0);
    }
  } else {
    uint n = (addr & 0x1f) << 2;  //sprite#
    list[n + 0].x.bit(8) = data.bit(0);
    list[n + 0].size     = data.bit(1);
    list[n + 1].x.bit(8) = data.bit(2);
    list[n + 1].size     = data.bit(3);
    list[n + 2].x.bit(8) = data.bit(4);
    list[n + 2].size     = data.bit(5);
    list[n + 3].x.bit(8) = data.bit(6);
    list[n + 3].size     = data.bit(7);
  }
}

auto PPU::Sprite::synchronize() -> void {
  for(auto n : range(544)) update(n, ppu.oam[n]);
}

auto PPU::Sprite::Object::width() const -> uint{
  if(size == 0) {
    static uint width[] = { 8,  8,  8, 16, 16, 32, 16, 16};
    return width[ppu.sprite.regs.base_size];
  } else {
    static uint width[] = {16, 32, 64, 32, 64, 64, 32, 32};
    return width[ppu.sprite.regs.base_size];
  }
}

auto PPU::Sprite::Object::height() const -> uint {
  if(size == 0) {
    if(ppu.sprite.regs.interlace && ppu.sprite.regs.base_size >= 6) return 16;
    static uint height[] = { 8,  8,  8, 16, 16, 32, 32, 32};
    return height[ppu.sprite.regs.base_size];
  } else {
    static uint height[] = {16, 32, 64, 32, 64, 64, 64, 32};
    return height[ppu.sprite.regs.base_size];
  }
}
