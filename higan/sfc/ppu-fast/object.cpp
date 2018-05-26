auto PPU::readOAM(uint10 address) -> uint8 {
  if(!address.bit(9)) {
    uint n = address >> 2;  //object#
    address &= 3;
    if(address == 0) return object[n].x.bits(0,7);
    if(address == 1) return object[n].y;
    if(address == 2) return object[n].character;
    return (
      object[n].nameselect << 0
    | object[n].palette    << 1
    | object[n].priority   << 4
    | object[n].hflip      << 6
    | object[n].vflip      << 7
    );
  } else {
    uint n = (address & 0x1f) << 2;  //object#
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

auto PPU::writeOAM(uint10 address, uint8 data) -> void {
  if(!address.bit(9)) {
    uint n = address >> 2;  //object#
    if(address == 0) { object[n].x.bits(0,7) = data; return; }
    if(address == 1) { object[n].y = data; return; }
    if(address == 2) { object[n].character = data; return; }
    object[n].nameselect = data.bit (0);
    object[n].palette    = data.bits(1,3);
    object[n].priority   = data.bits(4,5);
    object[n].hflip      = data.bit (6);
    object[n].vflip      = data.bit (7);
  } else {
    uint n = (address & 0x1f) << 2;  //object#
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
