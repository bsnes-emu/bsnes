auto PPUfast::latchCounters() -> void {
  io.hcounter = cpu.hdot();
  io.vcounter = cpu.vcounter();
  latch.counters = 1;
}

auto PPUfast::vramAddress() const -> uint15 {  //uint15 for 64K VRAM; uint16 for 128K VRAM
  uint15 address = io.vramAddress;
  switch(io.vramMapping) {
  case 0: return address;
  case 1: return bits(address, 8-15) <<  8 | bits(address,0-4) << 3 | bits(address,5-7);
  case 2: return bits(address, 9-15) <<  9 | bits(address,0-5) << 3 | bits(address,6-8);
  case 3: return bits(address,10-15) << 10 | bits(address,0-6) << 3 | bits(address,7-9);
  }
  unreachable;
}

auto PPUfast::readVRAM() -> uint16 {
  if(!io.displayDisable && cpu.vcounter() < vdisp()) return 0x0000;
  auto address = vramAddress();
  return vram[address];
}

template<bool Byte>
auto PPUfast::writeVRAM(uint8 data) -> void {
  if(!io.displayDisable && cpu.vcounter() < vdisp()) return;
  Line::flush();
  auto address = vramAddress();
  if constexpr(Byte == 0) {
    vram[address] = vram[address] & 0xff00 | data << 0;
  }
  if constexpr(Byte == 1) {
    vram[address] = vram[address] & 0x00ff | data << 8;
  }
  updateTiledata(address);
}

auto PPUfast::updateTiledata(uint address) -> void {
  auto word = vram[address];
  auto line2bpp = tilecache[TileMode::BPP2] + ((address & 0x7fff) << 3);
  auto line4bpp = tilecache[TileMode::BPP4] + ((address & 0x7ff0) << 2) + ((address & 7) << 3);
  auto line8bpp = tilecache[TileMode::BPP8] + ((address & 0x7fe0) << 1) + ((address & 7) << 3);
  uint plane4bpp = address >> 2 & 2;
  uint plane8bpp = address >> 2 & 6;
  for(uint x : range(8)) {
    line2bpp[7 - x] = word >> x & 1 | word >> x + 7 & 2;
    line4bpp[7 - x] = line4bpp[7 - x] & ~(3 << plane4bpp) | (word >> x & 1) << plane4bpp | (word >> x + 7 & 2) << plane4bpp;
    line8bpp[7 - x] = line8bpp[7 - x] & ~(3 << plane8bpp) | (word >> x & 1) << plane8bpp | (word >> x + 7 & 2) << plane8bpp;
  }
}

auto PPUfast::readOAM(uint10 address) -> uint8 {
  if(!io.displayDisable && cpu.vcounter() < vdisp()) address = latch.oamAddress;
  return readObject(address);
}

auto PPUfast::writeOAM(uint10 address, uint8 data) -> void {
  Line::flush();
  //Uniracers 2-player mode hack
  if(!io.displayDisable && cpu.vcounter() < vdisp()) address = 0x0218;  //latch.oamAddress;
  return writeObject(address, data);
}

template<bool Byte>
auto PPUfast::readCGRAM(uint8 address) -> uint8 {
  if(!io.displayDisable
  && cpu.vcounter() > 0 && cpu.vcounter() < vdisp()
  && cpu.hcounter() >= 88 && cpu.hcounter() < 1096
  ) address = latch.cgramAddress;
  if constexpr(Byte == 0) {
    return cgram[address] >> 0;
  }
  if constexpr(Byte == 1) {
    return cgram[address] >> 8;
  }
}

auto PPUfast::writeCGRAM(uint8 address, uint15 data) -> void {
  if(!io.displayDisable
  && cpu.vcounter() > 0 && cpu.vcounter() < vdisp()
  && cpu.hcounter() >= 88 && cpu.hcounter() < 1096
  ) address = latch.cgramAddress;
  cgram[address] = data;
}

auto PPUfast::readIO(uint address, uint8 data) -> uint8 {
  cpu.synchronize(ppufast);

  switch(address & 0xffff) {

  case 0x2104: case 0x2105: case 0x2106: case 0x2108:
  case 0x2109: case 0x210a: case 0x2114: case 0x2115:
  case 0x2116: case 0x2118: case 0x2119: case 0x211a:
  case 0x2124: case 0x2125: case 0x2126: case 0x2128:
  case 0x2129: case 0x212a: {
    return latch.ppu1.mdr;
  }

  case 0x2134: {  //MPYL
    uint24 result = (int16)io.mode7.a * (int8)(io.mode7.b >> 8);
    return latch.ppu1.mdr = bit8(result,0);
  }

  case 0x2135: {  //MPYM
    uint24 result = (int16)io.mode7.a * (int8)(io.mode7.b >> 8);
    return latch.ppu1.mdr = bit8(result,1);
  }

  case 0x2136: {  //MPYH
    uint24 result = (int16)io.mode7.a * (int8)(io.mode7.b >> 8);
    return latch.ppu1.mdr = bit8(result,2);
  }

  case 0x2137: {  //SLHV
    if(cbit1(cpu.pio(),7)) latchCounters();
    return data;  //CPU MDR
  }

  case 0x2138: {  //OAMDATAREAD
    data = readOAM(io.oamAddress++);
    oamSetFirstObject();
    return latch.ppu1.mdr = data;
  }

  case 0x2139: {  //VMDATALREAD
    data = latch.vram >> 0;
    if(io.vramIncrementMode == 0) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return latch.ppu1.mdr = data;
  }

  case 0x213a: {  //VMDATAHREAD
    data = latch.vram >> 8;
    if(io.vramIncrementMode == 1) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return latch.ppu1.mdr = data;
  }

  case 0x213b: {  //CGDATAREAD
    if(io.cgramAddressLatch++ == 0) {
      bits(latch.ppu2.mdr,0-7) = readCGRAM<0>(io.cgramAddress);
    } else {
      bits(latch.ppu2.mdr,0-6) = readCGRAM<1>(io.cgramAddress++);
    }
    return latch.ppu2.mdr;
  }

  case 0x213c: {  //OPHCT
    if(latch.hcounter == 0) {
      latch.hcounter = 1;
      bits(latch.ppu2.mdr,0-7) = bits(io.hcounter,0-7);
    } else {
      latch.hcounter = 0;
      bit1(latch.ppu2.mdr,0) = bit1(io.hcounter,8);
    }
    return latch.ppu2.mdr;
  }

  case 0x213d: {  //OPVCT
    if(latch.vcounter == 0) {
      latch.vcounter = 1;
      bits(latch.ppu2.mdr,0-7) = bits(io.vcounter,0-7);
    } else {
      latch.vcounter = 0;
      bit1(latch.ppu2.mdr,0) = bit1(io.vcounter,8);
    }
    return latch.ppu2.mdr;
  }

  case 0x213e: {  //STAT77
    bits(latch.ppu1.mdr,0-3) = 1;  //PPU1 version
    bit1(latch.ppu1.mdr,5) = 0;
    bit1(latch.ppu1.mdr,6) = io.obj.rangeOver;
    bit1(latch.ppu1.mdr,7) = io.obj.timeOver;
    return latch.ppu1.mdr;
  }

  case 0x213f: {  //STAT78
    latch.hcounter = 0;
    latch.vcounter = 0;
    bits(latch.ppu2.mdr,0-3) = 3;  //PPU2 version
    bit1(latch.ppu2.mdr,4) = Region::PAL();  //0 = NTSC, 1 = PAL
    if(!cbit1(cpu.pio(),7)) {
      bit1(latch.ppu2.mdr,6) = 1;
    } else {
      bit1(latch.ppu2.mdr,6) = latch.counters;
      latch.counters = 0;
    }
    bit1(latch.ppu2.mdr,7) = field();
    return latch.ppu2.mdr;
  }

  }

  return data;
}

auto PPUfast::writeIO(uint address, uint8 data) -> void {
  cpu.synchronize(ppufast);

  switch(address & 0xffff) {

  case 0x2100: {  //INIDISP
    if(io.displayDisable && cpu.vcounter() == vdisp()) oamAddressReset();
    io.displayBrightness = bits(data,0-3);
    io.displayDisable    = bit1(data,7);
    return;
  }

  case 0x2101: {  //OBSEL
    io.obj.tiledataAddress = bits(data,0-2) << 13;
    io.obj.nameselect      = bits(data,3-4);
    io.obj.baseSize        = bits(data,5-7);
    return;
  }

  case 0x2102: {  //OAMADDL
    io.oamBaseAddress = (io.oamBaseAddress & 0x0200) | data << 1;
    oamAddressReset();
    return;
  }

  case 0x2103: {  //OAMADDH
    io.oamBaseAddress = bit1(data,0) << 9 | (io.oamBaseAddress & 0x01fe);
    io.oamPriority    = bit1(data,7);
    oamAddressReset();
    return;
  }

  case 0x2104: {  //OAMDATA
    uint1 latchBit = io.oamAddress & 1;
    uint10 address = io.oamAddress++;
    if(latchBit == 0) latch.oam = data;
    if(bit1(address,9)) {
      writeOAM(address, data);
    } else if(latchBit == 1) {
      writeOAM((address & ~1) + 0, latch.oam);
      writeOAM((address & ~1) + 1, data);
    }
    oamSetFirstObject();
    return;
  }

  case 0x2105: {  //BGMODE
    io.bgMode       = bits(data,0-2);
    io.bgPriority   = bit1(data,3);
    io.bg1.tileSize = bit1(data,4);
    io.bg2.tileSize = bit1(data,5);
    io.bg3.tileSize = bit1(data,6);
    io.bg4.tileSize = bit1(data,7);
    updateVideoMode();
    return;
  }

  case 0x2106: {  //MOSAIC
    io.bg1.mosaicEnable = bit1(data,0);
    io.bg2.mosaicEnable = bit1(data,1);
    io.bg3.mosaicEnable = bit1(data,2);
    io.bg4.mosaicEnable = bit1(data,3);
    io.mosaicSize       = bits(data,4-7);
    return;
  }

  case 0x2107: {  //BG1SC
    io.bg1.screenSize    = bits(data,0-1);
    io.bg1.screenAddress = bits(data,2-7) << 10;
    return;
  }

  case 0x2108: {  //BG2SC
    io.bg2.screenSize    = bits(data,0-1);
    io.bg2.screenAddress = bits(data,2-7) << 10;
    return;
  }

  case 0x2109: {  //BG3SC
    io.bg3.screenSize    = bits(data,0-1);
    io.bg3.screenAddress = bits(data,2-7) << 10;
    return;
  }

  case 0x210a: {  //BG4SC
    io.bg4.screenSize    = bits(data,0-1);
    io.bg4.screenAddress = bits(data,2-7) << 10;
    return;
  }

  case 0x210b: {  //BG12NBA
    io.bg1.tiledataAddress = bits(data,0-3) << 12;
    io.bg2.tiledataAddress = bits(data,4-7) << 12;
    return;
  }

  case 0x210c: {  //BG34NBA
    io.bg3.tiledataAddress = bits(data,0-3) << 12;
    io.bg4.tiledataAddress = bits(data,4-7) << 12;
    return;
  }

  case 0x210d: {  //BG1HOFS
    io.mode7.hoffset = data << 8 | latch.mode7;
    latch.mode7 = data;

    io.bg1.hoffset = data << 8 | (latch.ppu1.bgofs & ~7) | (latch.ppu2.bgofs & 7);
    latch.ppu1.bgofs = data;
    latch.ppu2.bgofs = data;
    return;
  }

  case 0x210e: {  //BG1VOFS
    io.mode7.voffset = data << 8 | latch.mode7;
    latch.mode7 = data;

    io.bg1.voffset = data << 8 | latch.ppu1.bgofs;
    latch.ppu1.bgofs = data;
    return;
  }

  case 0x210f: {  //BG2HOFS
    io.bg2.hoffset = data << 8 | (latch.ppu1.bgofs & ~7) | (latch.ppu2.bgofs & 7);
    latch.ppu1.bgofs = data;
    latch.ppu2.bgofs = data;
    return;
  }

  case 0x2110: {  //BG2VOFS
    io.bg2.voffset = data << 8 | latch.ppu1.bgofs;
    latch.ppu1.bgofs = data;
    return;
  }

  case 0x2111: {  //BG3HOFS
    io.bg3.hoffset = data << 8 | (latch.ppu1.bgofs & ~7) | (latch.ppu2.bgofs & 7);
    latch.ppu1.bgofs = data;
    latch.ppu2.bgofs = data;
    return;
  }

  case 0x2112: {  //BG3VOFS
    io.bg3.voffset = data << 8 | latch.ppu1.bgofs;
    latch.ppu1.bgofs = data;
    return;
  }

  case 0x2113: {  //BG4HOFS
    io.bg4.hoffset = data << 8 | (latch.ppu1.bgofs & ~7) | (latch.ppu2.bgofs & 7);
    latch.ppu1.bgofs = data;
    latch.ppu2.bgofs = data;
    return;
  }

  case 0x2114: {  //BG4VOFS
    io.bg4.voffset = data << 8 | latch.ppu1.bgofs;
    latch.ppu1.bgofs = data;
    return;
  }

  case 0x2115: {  //VMAIN
    static const uint size[4] = {1, 32, 128, 128};
    io.vramIncrementSize = size[data & 3];
    io.vramMapping       = bits(data,2-3);
    io.vramIncrementMode = bit1(data,7);
    return;
  }

  case 0x2116: {  //VMADDL
    io.vramAddress = io.vramAddress & 0xff00 | data << 0;
    latch.vram = readVRAM();
    return;
  }

  case 0x2117: {  //VMADDH
    io.vramAddress = io.vramAddress & 0x00ff | data << 8;
    latch.vram = readVRAM();
    return;
  }

  case 0x2118: {  //VMDATAL
    writeVRAM<0>(data);
    if(io.vramIncrementMode == 0) io.vramAddress += io.vramIncrementSize;
    return;
  }

  case 0x2119: {  //VMDATAH
    writeVRAM<1>(data);
    if(io.vramIncrementMode == 1) io.vramAddress += io.vramIncrementSize;
    return;
  }

  case 0x211a: {  //M7SEL
    io.mode7.hflip  = bit1(data,0);
    io.mode7.vflip  = bit1(data,1);
    io.mode7.repeat = bits(data,6-7);
    return;
  }

  case 0x211b: {  //M7A
    io.mode7.a = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211c: {  //M7B
    io.mode7.b = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211d: {  //M7C
    io.mode7.c = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211e: {  //M7D
    io.mode7.d = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211f: {  //M7X
    io.mode7.x = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x2120: {  //M7Y
    io.mode7.y = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x2121: {  //CGADD
    io.cgramAddress = data;
    io.cgramAddressLatch = 0;
    return;
  }

  case 0x2122: {  //CGDATA
    if(io.cgramAddressLatch++ == 0) {
      latch.cgram = data;
    } else {
      writeCGRAM(io.cgramAddress++, bits(data,0-6) << 8 | latch.cgram);
    }
    return;
  }

  case 0x2123: {  //W12SEL
    io.bg1.window.oneInvert = bit1(data,0);
    io.bg1.window.oneEnable = bit1(data,1);
    io.bg1.window.twoInvert = bit1(data,2);
    io.bg1.window.twoEnable = bit1(data,3);
    io.bg2.window.oneInvert = bit1(data,4);
    io.bg2.window.oneEnable = bit1(data,5);
    io.bg2.window.twoInvert = bit1(data,6);
    io.bg2.window.twoEnable = bit1(data,7);
    return;
  }

  case 0x2124: {  //W34SEL
    io.bg3.window.oneInvert = bit1(data,0);
    io.bg3.window.oneEnable = bit1(data,1);
    io.bg3.window.twoInvert = bit1(data,2);
    io.bg3.window.twoEnable = bit1(data,3);
    io.bg4.window.oneInvert = bit1(data,4);
    io.bg4.window.oneEnable = bit1(data,5);
    io.bg4.window.twoInvert = bit1(data,6);
    io.bg4.window.twoEnable = bit1(data,7);
    return;
  }

  case 0x2125: {  //WOBJSEL
    io.obj.window.oneInvert = bit1(data,0);
    io.obj.window.oneEnable = bit1(data,1);
    io.obj.window.twoInvert = bit1(data,2);
    io.obj.window.twoEnable = bit1(data,3);
    io.col.window.oneInvert = bit1(data,4);
    io.col.window.oneEnable = bit1(data,5);
    io.col.window.twoInvert = bit1(data,6);
    io.col.window.twoEnable = bit1(data,7);
    return;
  }

  case 0x2126: {  //WH0
    io.window.oneLeft = data;
    return;
  }

  case 0x2127: {  //WH1
    io.window.oneRight = data;
    return;
  }

  case 0x2128: {  //WH2
    io.window.twoLeft = data;
    return;
  }

  case 0x2129: {  //WH3
    io.window.twoRight = data;
    return;
  }

  case 0x212a: {  //WBGLOG
    io.bg1.window.mask = bits(data,0-1);
    io.bg2.window.mask = bits(data,2-3);
    io.bg3.window.mask = bits(data,4-5);
    io.bg4.window.mask = bits(data,6-7);
    return;
  }

  case 0x212b: {  //WOBJLOG
    io.obj.window.mask = bits(data,0-1);
    io.col.window.mask = bits(data,2-3);
    return;
  }

  case 0x212c: {  //TM
    io.bg1.aboveEnable = bit1(data,0);
    io.bg2.aboveEnable = bit1(data,1);
    io.bg3.aboveEnable = bit1(data,2);
    io.bg4.aboveEnable = bit1(data,3);
    io.obj.aboveEnable = bit1(data,4);
    return;
  }

  case 0x212d: {  //TS
    io.bg1.belowEnable = bit1(data,0);
    io.bg2.belowEnable = bit1(data,1);
    io.bg3.belowEnable = bit1(data,2);
    io.bg4.belowEnable = bit1(data,3);
    io.obj.belowEnable = bit1(data,4);
    return;
  }

  case 0x212e: {  //TMW
    io.bg1.window.aboveEnable = bit1(data,0);
    io.bg2.window.aboveEnable = bit1(data,1);
    io.bg3.window.aboveEnable = bit1(data,2);
    io.bg4.window.aboveEnable = bit1(data,3);
    io.obj.window.aboveEnable = bit1(data,4);
    return;
  }

  case 0x212f: {  //TSW
    io.bg1.window.belowEnable = bit1(data,0);
    io.bg2.window.belowEnable = bit1(data,1);
    io.bg3.window.belowEnable = bit1(data,2);
    io.bg4.window.belowEnable = bit1(data,3);
    io.obj.window.belowEnable = bit1(data,4);
    return;
  }

  case 0x2130: {  //CGWSEL
    io.col.directColor      = bit1(data,0);
    io.col.blendMode        = bit1(data,1);
    io.col.window.belowMask = bits(data,4-5);
    io.col.window.aboveMask = bits(data,6-7);
    return;
  }

  case 0x2131: {  //CGADDSUB
    io.col.enable[Source::BG1 ] = bit1(data,0);
    io.col.enable[Source::BG2 ] = bit1(data,1);
    io.col.enable[Source::BG3 ] = bit1(data,2);
    io.col.enable[Source::BG4 ] = bit1(data,3);
    io.col.enable[Source::OBJ1] = 0;
    io.col.enable[Source::OBJ2] = bit1(data,4);
    io.col.enable[Source::COL ] = bit1(data,5);
    io.col.halve                = bit1(data,6);
    io.col.mathMode             = bit1(data,7);
    return;
  }

  case 0x2132: {  //COLDATA
    if(bit1(data,5)) bits(io.col.fixedColor, 0- 4) = data & 31;
    if(bit1(data,6)) bits(io.col.fixedColor, 5- 9) = data & 31;
    if(bit1(data,7)) bits(io.col.fixedColor,10-14) = data & 31;
    return;
  }

  case 0x2133: {  //SETINI
    io.interlace     = bit1(data,0);
    io.obj.interlace = bit1(data,1);
    io.overscan      = bit1(data,2);
    io.pseudoHires   = bit1(data,3);
    io.extbg         = bit1(data,6);
    updateVideoMode();
    return;
  }

  }
}

auto PPUfast::updateVideoMode() -> void {
  ppubase.display.vdisp = !io.overscan ? 225 : 240;

  switch(io.bgMode) {
  case 0:
    io.bg1.tileMode = TileMode::BPP2;
    io.bg2.tileMode = TileMode::BPP2;
    io.bg3.tileMode = TileMode::BPP2;
    io.bg4.tileMode = TileMode::BPP2;
    memory::assign(io.bg1.priority, 8, 11);
    memory::assign(io.bg2.priority, 7, 10);
    memory::assign(io.bg3.priority, 2,  5);
    memory::assign(io.bg4.priority, 1,  4);
    memory::assign(io.obj.priority, 3,  6, 9, 12);
    break;

  case 1:
    io.bg1.tileMode = TileMode::BPP4;
    io.bg2.tileMode = TileMode::BPP4;
    io.bg3.tileMode = TileMode::BPP2;
    io.bg4.tileMode = TileMode::Inactive;
    if(io.bgPriority) {
      memory::assign(io.bg1.priority, 5,  8);
      memory::assign(io.bg2.priority, 4,  7);
      memory::assign(io.bg3.priority, 1, 10);
      memory::assign(io.obj.priority, 2,  3, 6,  9);
    } else {
      memory::assign(io.bg1.priority, 6,  9);
      memory::assign(io.bg2.priority, 5,  8);
      memory::assign(io.bg3.priority, 1,  3);
      memory::assign(io.obj.priority, 2,  4, 7, 10);
    }
    break;

  case 2:
    io.bg1.tileMode = TileMode::BPP4;
    io.bg2.tileMode = TileMode::BPP4;
    io.bg3.tileMode = TileMode::Inactive;
    io.bg4.tileMode = TileMode::Inactive;
    memory::assign(io.bg1.priority, 3, 7);
    memory::assign(io.bg2.priority, 1, 5);
    memory::assign(io.obj.priority, 2, 4, 6, 8);
    break;

  case 3:
    io.bg1.tileMode = TileMode::BPP8;
    io.bg2.tileMode = TileMode::BPP4;
    io.bg3.tileMode = TileMode::Inactive;
    io.bg4.tileMode = TileMode::Inactive;
    memory::assign(io.bg1.priority, 3, 7);
    memory::assign(io.bg2.priority, 1, 5);
    memory::assign(io.obj.priority, 2, 4, 6, 8);
    break;

  case 4:
    io.bg1.tileMode = TileMode::BPP8;
    io.bg2.tileMode = TileMode::BPP2;
    io.bg3.tileMode = TileMode::Inactive;
    io.bg4.tileMode = TileMode::Inactive;
    memory::assign(io.bg1.priority, 3, 7);
    memory::assign(io.bg2.priority, 1, 5);
    memory::assign(io.obj.priority, 2, 4, 6, 8);
    break;

  case 5:
    io.bg1.tileMode = TileMode::BPP4;
    io.bg2.tileMode = TileMode::BPP2;
    io.bg3.tileMode = TileMode::Inactive;
    io.bg4.tileMode = TileMode::Inactive;
    memory::assign(io.bg1.priority, 3, 7);
    memory::assign(io.bg2.priority, 1, 5);
    memory::assign(io.obj.priority, 2, 4, 6, 8);
    break;

  case 6:
    io.bg1.tileMode = TileMode::BPP4;
    io.bg2.tileMode = TileMode::Inactive;
    io.bg3.tileMode = TileMode::Inactive;
    io.bg4.tileMode = TileMode::Inactive;
    memory::assign(io.bg1.priority, 2, 5);
    memory::assign(io.obj.priority, 1, 3, 4, 6);
    break;

  case 7:
    if(!io.extbg) {
      io.bg1.tileMode = TileMode::Mode7;
      io.bg2.tileMode = TileMode::Inactive;
      io.bg3.tileMode = TileMode::Inactive;
      io.bg4.tileMode = TileMode::Inactive;
      memory::assign(io.bg1.priority, 2);
      memory::assign(io.obj.priority, 1, 3, 4, 5);
    } else {
      io.bg1.tileMode = TileMode::Mode7;
      io.bg2.tileMode = TileMode::Mode7;
      io.bg3.tileMode = TileMode::Inactive;
      io.bg4.tileMode = TileMode::Inactive;
      memory::assign(io.bg1.priority, 3);
      memory::assign(io.bg2.priority, 1, 5);
      memory::assign(io.obj.priority, 2, 4, 6, 7);
    }
    break;
  }
}
