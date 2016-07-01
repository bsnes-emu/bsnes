auto PPU::OAM::read(uint10 addr) -> uint8 {
  if(!addr.bit(9)) {
    uint n = addr >> 2;  //object#
    addr &= 3;
    if(addr == 0) return object[n].x.bits(0,7);
    if(addr == 1) return object[n].y;
    if(addr == 2) return object[n].character;
    return (
      object[n].nameselect << 0
    | object[n].palette    << 1
    | object[n].priority   << 4
    | object[n].hflip      << 6
    | object[n].vflip      << 7
    );
  } else {
    uint n = (addr & 0x1f) << 2;  //object#
    return (
      object[n + 0].x.bit(8) << 0
    | object[n + 0].size     << 1
    | object[n + 1].x.bit(8) << 2
    | object[n + 1].size     << 3
    | object[n + 2].x.bit(8) << 4
    | object[n + 2].size     << 5
    | object[n + 3].x.bit(8) << 6
    | object[n + 3].size     << 7
    );
  }
}

auto PPU::OAM::write(uint10 addr, uint8 data) -> void {
  if(!addr.bit(9)) {
    uint n = addr >> 2;  //object#
    addr &= 3;
    if(addr == 0) { object[n].x.bits(0,7) = data; return; }
    if(addr == 1) { object[n].y = data; return; }
    if(addr == 2) { object[n].character = data; return; }
    object[n].nameselect = data.bit (0);
    object[n].palette    = data.bits(1,3);
    object[n].priority   = data.bits(4,5);
    object[n].hflip      = data.bit (6);
    object[n].vflip      = data.bit (7);
  } else {
    uint n = (addr & 0x1f) << 2;  //object#
    object[n + 0].x.bit(8) = data.bit(0);
    object[n + 0].size     = data.bit(1);
    object[n + 1].x.bit(8) = data.bit(2);
    object[n + 1].size     = data.bit(3);
    object[n + 2].x.bit(8) = data.bit(4);
    object[n + 2].size     = data.bit(5);
    object[n + 3].x.bit(8) = data.bit(6);
    object[n + 3].size     = data.bit(7);
  }
}

auto PPU::OAM::Object::width() const -> uint {
  if(size == 0) {
    static const uint width[] = { 8,  8,  8, 16, 16, 32, 16, 16};
    return width[ppu.obj.io.baseSize];
  } else {
    static const uint width[] = {16, 32, 64, 32, 64, 64, 32, 32};
    return width[ppu.obj.io.baseSize];
  }
}

auto PPU::OAM::Object::height() const -> uint {
  if(size == 0) {
    if(ppu.obj.io.interlace && ppu.obj.io.baseSize >= 6) return 16;  //hardware quirk
    static const uint height[] = { 8,  8,  8, 16, 16, 32, 32, 32};
    return height[ppu.obj.io.baseSize];
  } else {
    static const uint height[] = {16, 32, 64, 32, 64, 64, 64, 32};
    return height[ppu.obj.io.baseSize];
  }
}
