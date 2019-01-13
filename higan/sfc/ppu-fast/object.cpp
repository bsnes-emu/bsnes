auto PPU::Line::renderObject(PPU::IO::Object& self) -> void {
  if(!self.aboveEnable && !self.belowEnable) return;

  array<bool[256]> windowAbove;
  array<bool[256]> windowBelow;
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  uint itemCount = 0;
  uint tileCount = 0;
  for(auto n : range(ppu.ItemLimit)) items[n].valid = false;
  for(auto n : range(ppu.TileLimit)) tiles[n].valid = false;

  for(auto n : range(128)) {
    ObjectItem item{true, self.first + n};
    const auto& object = ppu.objects[item.index];

    if(object.size == 0) {
      static const uint widths[]  = { 8,  8,  8, 16, 16, 32, 16, 16};
      static const uint heights[] = { 8,  8,  8, 16, 16, 32, 32, 32};
      item.width  = widths [self.baseSize];
      item.height = heights[self.baseSize];
      if(self.interlace && self.baseSize >= 6) item.height = 16;  //hardware quirk
    } else {
      static const uint widths[]  = {16, 32, 64, 32, 64, 64, 32, 32};
      static const uint heights[] = {16, 32, 64, 32, 64, 64, 64, 32};
      item.width  = widths [self.baseSize];
      item.height = heights[self.baseSize];
    }

    if(object.x > 256 && object.x + item.width - 1 < 512) continue;
    uint height = item.height >> self.interlace;
    if((y >= object.y && y < object.y + height)
    || (object.y + height >= 256 && y < (object.y + height & 255))
    ) {
      if(itemCount++ >= ppu.ItemLimit) break;
      items[itemCount - 1] = item;
    }
  }

  for(int n : reverse(range(ppu.ItemLimit))) {
    const auto& item = items[n];
    if(!item.valid) continue;

    const auto& object = ppu.objects[item.index];
    uint tileWidth = item.width >> 3;
    int x = object.x;
    int y = this->y - object.y & 0xff;
    if(self.interlace) y <<= 1;

    if(object.vflip) {
      if(item.width == item.height) {
        y = item.height - 1 - y;
      } else if(y < item.width) {
        y = item.width - 1 - y;
      } else {
        y = item.width + (item.width - 1) - (y - item.width);
      }
    }

    if(self.interlace) {
      y = !object.vflip ? y + ppu.field() : y - ppu.field();
    }

    x &= 511;
    y &= 255;

    uint16 tiledataAddress = self.tiledataAddress;
    if(object.nameselect) tiledataAddress += 1 + self.nameselect << 12;
    uint16 characterX =  object.character.bits(0,3);
    uint16 characterY = (object.character.bits(4,7) + (y >> 3) & 15) << 4;

    for(uint tileX : range(tileWidth)) {
      uint objectX = x + (tileX << 3) & 511;
      if(x != 256 && objectX >= 256 && objectX + 7 < 512) continue;

      ObjectTile tile{true};
      tile.x = objectX;
      tile.y = y;
      tile.priority = object.priority;
      tile.palette = 128 + (object.palette << 4);
      tile.hflip = object.hflip;

      uint mirrorX = !object.hflip ? tileX : tileWidth - 1 - tileX;
      uint address = tiledataAddress + ((characterY + (characterX + mirrorX & 15)) << 4);
      tile.number = address >> 4;

      if(tileCount++ >= ppu.TileLimit) break;
      tiles[tileCount - 1] = tile;
    }
  }

  ppu.io.obj.rangeOver |= itemCount > ppu.ItemLimit;
  ppu.io.obj.timeOver  |= tileCount > ppu.TileLimit;

  uint8 palette[256];
  uint8 priority[256];

  for(uint n : range(ppu.TileLimit)) {
    const auto& tile = tiles[n];
    if(!tile.valid) continue;

    auto tiledata = ppu.tilecache[TileMode::BPP4] + (tile.number << 6) + ((tile.y & 7) << 3);
    uint tileX = tile.x;
    uint mirrorX = tile.hflip ? 7 : 0;
    for(uint x : range(8)) {
      tileX &= 511;
      if(tileX < 256) {
        if(uint color = tiledata[x ^ mirrorX]) {
          palette[tileX] = tile.palette + color;
          priority[tileX] = self.priority[tile.priority];
        }
      }
      tileX++;
    }
  }

  for(uint x : range(256)) {
    if(!priority[x]) continue;
    uint source = palette[x] < 192 ? Source::OBJ1 : Source::OBJ2;
    if(self.aboveEnable && !windowAbove[x]) plotAbove(x, source, priority[x], cgram[palette[x]]);
    if(self.belowEnable && !windowBelow[x]) plotBelow(x, source, priority[x], cgram[palette[x]]);
  }
}

auto PPU::oamAddressReset() -> void {
  io.oamAddress = io.oamBaseAddress;
  oamSetFirstObject();
}

auto PPU::oamSetFirstObject() -> void {
  io.obj.first = !io.oamPriority ? 0 : uint(io.oamAddress >> 2);
}

auto PPU::readObject(uint10 address) -> uint8 {
  if(!address.bit(9)) {
    uint n = address >> 2;  //object#
    address &= 3;
    if(address == 0) return objects[n].x.bits(0,7);
    if(address == 1) return objects[n].y - 1;
    if(address == 2) return objects[n].character;
    return (
      objects[n].nameselect << 0
    | objects[n].palette    << 1
    | objects[n].priority   << 4
    | objects[n].hflip      << 6
    | objects[n].vflip      << 7
    );
  } else {
    uint n = (address & 0x1f) << 2;  //object#
    return (
      objects[n + 0].x.bit(8) << 0
    | objects[n + 0].size     << 1
    | objects[n + 1].x.bit(8) << 2
    | objects[n + 1].size     << 3
    | objects[n + 2].x.bit(8) << 4
    | objects[n + 2].size     << 5
    | objects[n + 3].x.bit(8) << 6
    | objects[n + 3].size     << 7
    );
  }
}

auto PPU::writeObject(uint10 address, uint8 data) -> void {
  if(!address.bit(9)) {
    uint n = address >> 2;  //object#
    address &= 3;
    if(address == 0) { objects[n].x.bits(0,7) = data; return; }
    if(address == 1) { objects[n].y = data + 1; return; }  //+1 => rendering happens one scanline late
    if(address == 2) { objects[n].character = data; return; }
    objects[n].nameselect = data.bit (0);
    objects[n].palette    = data.bits(1,3);
    objects[n].priority   = data.bits(4,5);
    objects[n].hflip      = data.bit (6);
    objects[n].vflip      = data.bit (7);
  } else {
    uint n = (address & 0x1f) << 2;  //object#
    objects[n + 0].x.bit(8) = data.bit(0);
    objects[n + 0].size     = data.bit(1);
    objects[n + 1].x.bit(8) = data.bit(2);
    objects[n + 1].size     = data.bit(3);
    objects[n + 2].x.bit(8) = data.bit(4);
    objects[n + 2].size     = data.bit(5);
    objects[n + 3].x.bit(8) = data.bit(6);
    objects[n + 3].size     = data.bit(7);
  }
}
