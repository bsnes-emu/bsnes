auto PPUfast::latchCounters() -> void {
  io.hcounter = cpu.hdot();
  io.vcounter = cpu.vcounter();
  latch.counters = 1;
}

auto PPUfast::vramAddress() const -> uint15 {  //uint15 for 64K VRAM; uint16 for 128K VRAM
  uint16 address = io.vramAddress;
  switch(io.vramMapping) {
  case 0: return address;
  case 1: return address.bits( 8,15) <<  8 | address.bits(0,4) << 3 | address.bits(5,7);
  case 2: return address.bits( 9,15) <<  9 | address.bits(0,5) << 3 | address.bits(6,8);
  case 3: return address.bits(10,15) << 10 | address.bits(0,6) << 3 | address.bits(7,9);
  }
  unreachable;
}

auto PPUfast::readVRAM() -> uint16 {
  if(!io.displayDisable && cpu.vcounter() < vdisp()) return 0x0000;
  auto address = vramAddress();
  return vram[address];
}

auto PPUfast::writeVRAM(uint1 byte, uint8 data) -> void {
  if(!io.displayDisable && cpu.vcounter() < vdisp()) return;
  Line::flush();
  auto address = vramAddress();
  vram[address].byte(byte) = data;
  updateTiledata(address);
}

auto PPUfast::updateTiledata(uint15 address) -> void {
  auto word = vram[address];
  auto line2bpp = tilecache[TileMode::BPP2] + (address.bits(3,14) << 6) + (address.bits(0,2) << 3);
  auto line4bpp = tilecache[TileMode::BPP4] + (address.bits(4,14) << 6) + (address.bits(0,2) << 3);
  auto line8bpp = tilecache[TileMode::BPP8] + (address.bits(5,14) << 6) + (address.bits(0,2) << 3);
  uint plane4bpp = address.bit(3) << 1;
  uint plane8bpp = address.bit(3) << 1 | address.bit(4) << 2;
  for(uint x : range(8)) {
    line2bpp[7 - x].bit(            0) = word.bit(x + 0);
    line2bpp[7 - x].bit(            1) = word.bit(x + 8);
    line4bpp[7 - x].bit(plane4bpp + 0) = word.bit(x + 0);
    line4bpp[7 - x].bit(plane4bpp + 1) = word.bit(x + 8);
    line8bpp[7 - x].bit(plane8bpp + 0) = word.bit(x + 0);
    line8bpp[7 - x].bit(plane8bpp + 1) = word.bit(x + 8);
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

auto PPUfast::readCGRAM(uint1 byte, uint8 address) -> uint8 {
  if(!io.displayDisable
  && cpu.vcounter() > 0 && cpu.vcounter() < vdisp()
  && cpu.hcounter() >= 88 && cpu.hcounter() < 1096
  ) address = latch.cgramAddress;
  return cgram[address].byte(byte);
}

auto PPUfast::writeCGRAM(uint8 address, uint15 data) -> void {
  if(!io.displayDisable
  && cpu.vcounter() > 0 && cpu.vcounter() < vdisp()
  && cpu.hcounter() >= 88 && cpu.hcounter() < 1096
  ) address = latch.cgramAddress;
  cgram[address] = data;
}

auto PPUfast::readIO(uint24 address, uint8 data) -> uint8 {
  cpu.synchronize(ppufast);

  switch((uint16)address) {

  case 0x2104: case 0x2105: case 0x2106: case 0x2108:
  case 0x2109: case 0x210a: case 0x2114: case 0x2115:
  case 0x2116: case 0x2118: case 0x2119: case 0x211a:
  case 0x2124: case 0x2125: case 0x2126: case 0x2128:
  case 0x2129: case 0x212a: {
    return latch.ppu1.mdr;
  }

  case 0x2134: {  //MPYL
    uint24 result = (int16)io.mode7.a * (int8)(io.mode7.b >> 8);
    return latch.ppu1.mdr = result.byte(0);
  }

  case 0x2135: {  //MPYM
    uint24 result = (int16)io.mode7.a * (int8)(io.mode7.b >> 8);
    return latch.ppu1.mdr = result.byte(1);
  }

  case 0x2136: {  //MPYH
    uint24 result = (int16)io.mode7.a * (int8)(io.mode7.b >> 8);
    return latch.ppu1.mdr = result.byte(2);
  }

  case 0x2137: {  //SLHV
    if(cpu.pio().bit(7)) latchCounters();
    return data;  //CPU MDR
  }

  case 0x2138: {  //OAMDATAREAD
    data = readOAM(io.oamAddress++);
    oamSetFirstObject();
    return latch.ppu1.mdr = data;
  }

  case 0x2139: {  //VMDATALREAD
    data = latch.vram.byte(0);
    if(io.vramIncrementMode == 0) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return latch.ppu1.mdr = data;
  }

  case 0x213a: {  //VMDATAHREAD
    data = latch.vram.byte(1);
    if(io.vramIncrementMode == 1) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return latch.ppu1.mdr = data;
  }

  case 0x213b: {  //CGDATAREAD
    if(io.cgramAddressLatch++ == 0) {
      latch.ppu2.mdr.bits(0,7) = readCGRAM(0, io.cgramAddress);
    } else {
      latch.ppu2.mdr.bits(0,6) = readCGRAM(1, io.cgramAddress++);
    }
    return latch.ppu2.mdr;
  }

  case 0x213c: {  //OPHCT
    if(latch.hcounter++ == 0) {
      latch.ppu2.mdr.bits(0,7) = io.hcounter.bits(0,7);
    } else {
      latch.ppu2.mdr.bit(0) = io.hcounter.bit(8);
    }
    return latch.ppu2.mdr;
  }

  case 0x213d: {  //OPVCT
    if(latch.vcounter++ == 0) {
      latch.ppu2.mdr.bits(0,7) = io.vcounter.bits(0,7);
    } else {
      latch.ppu2.mdr.bit(0) = io.vcounter.bit(8);
    }
    return latch.ppu2.mdr;
  }

  case 0x213e: {  //STAT77
    latch.ppu1.mdr.bits(0,3) = 1;  //PPU1 version
    latch.ppu1.mdr.bit(5) = 0;
    latch.ppu1.mdr.bit(6) = io.obj.rangeOver;
    latch.ppu1.mdr.bit(7) = io.obj.timeOver;
    return latch.ppu1.mdr;
  }

  case 0x213f: {  //STAT78
    latch.hcounter = 0;
    latch.vcounter = 0;
    latch.ppu2.mdr.bits(0,3) = 3;  //PPU2 version
    latch.ppu2.mdr.bit(4) = Region::PAL();  //0 = NTSC, 1 = PAL
    if(!cpu.pio().bit(7)) {
      latch.ppu2.mdr.bit(6) = 1;
    } else {
      latch.ppu2.mdr.bit(6) = latch.counters;
      latch.counters = 0;
    }
    latch.ppu2.mdr.bit(7) = field();
    return latch.ppu2.mdr;
  }

  }

  return data;
}

auto PPUfast::writeIO(uint24 address, uint8 data) -> void {
  cpu.synchronize(ppufast);

  switch((uint16)address) {

  case 0x2100: {  //INIDISP
    if(io.displayDisable && cpu.vcounter() == vdisp()) oamAddressReset();
    io.displayBrightness = data.bits(0,3);
    io.displayDisable    = data.bit (7);
    return;
  }

  case 0x2101: {  //OBSEL
    io.obj.tiledataAddress = data.bits(0,2) << 13;
    io.obj.nameselect      = data.bits(3,4);
    io.obj.baseSize        = data.bits(5,7);
    return;
  }

  case 0x2102: {  //OAMADDL
    io.oamBaseAddress = (io.oamBaseAddress & 0x0200) | data << 1;
    oamAddressReset();
    return;
  }

  case 0x2103: {  //OAMADDH
    io.oamBaseAddress = data.bit(0) << 9 | (io.oamBaseAddress & 0x01fe);
    io.oamPriority    = data.bit(7);
    oamAddressReset();
    return;
  }

  case 0x2104: {  //OAMDATA
    uint1 latchBit = io.oamAddress.bit(0);
    uint10 address = io.oamAddress++;
    if(latchBit == 0) latch.oam = data;
    if(address.bit(9)) {
      writeOAM(address, data);
    } else if(latchBit == 1) {
      writeOAM((address & ~1) + 0, latch.oam);
      writeOAM((address & ~1) + 1, data);
    }
    oamSetFirstObject();
    return;
  }

  case 0x2105: {  //BGMODE
    io.bgMode       = data.bits(0,2);
    io.bgPriority   = data.bit (3);
    io.bg1.tileSize = data.bit (4);
    io.bg2.tileSize = data.bit (5);
    io.bg3.tileSize = data.bit (6);
    io.bg4.tileSize = data.bit (7);
    updateVideoMode();
    return;
  }

  case 0x2106: {  //MOSAIC
    io.bg1.mosaicEnable = data.bit (0);
    io.bg2.mosaicEnable = data.bit (1);
    io.bg3.mosaicEnable = data.bit (2);
    io.bg4.mosaicEnable = data.bit (3);
    io.mosaicSize       = data.bits(4,7);
    return;
  }

  case 0x2107: {  //BG1SC
    io.bg1.screenSize    = data.bits(0,1);
    io.bg1.screenAddress = data.bits(2,7) << 10;
    return;
  }

  case 0x2108: {  //BG2SC
    io.bg2.screenSize    = data.bits(0,1);
    io.bg2.screenAddress = data.bits(2,7) << 10;
    return;
  }

  case 0x2109: {  //BG3SC
    io.bg3.screenSize    = data.bits(0,1);
    io.bg3.screenAddress = data.bits(2,7) << 10;
    return;
  }

  case 0x210a: {  //BG4SC
    io.bg4.screenSize    = data.bits(0,1);
    io.bg4.screenAddress = data.bits(2,7) << 10;
    return;
  }

  case 0x210b: {  //BG12NBA
    io.bg1.tiledataAddress = data.bits(0,3) << 12;
    io.bg2.tiledataAddress = data.bits(4,7) << 12;
    return;
  }

  case 0x210c: {  //BG34NBA
    io.bg3.tiledataAddress = data.bits(0,3) << 12;
    io.bg4.tiledataAddress = data.bits(4,7) << 12;
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
    io.vramIncrementSize = size[data.bits(0,1)];
    io.vramMapping       = data.bits(2,3);
    io.vramIncrementMode = data.bit (7);
    return;
  }

  case 0x2116: {  //VMADDL
    io.vramAddress.byte(0) = data;
    latch.vram = readVRAM();
    return;
  }

  case 0x2117: {  //VMADDH
    io.vramAddress.byte(1) = data;
    latch.vram = readVRAM();
    return;
  }

  case 0x2118: {  //VMDATAL
    writeVRAM(0, data);
    if(io.vramIncrementMode == 0) io.vramAddress += io.vramIncrementSize;
    return;
  }

  case 0x2119: {  //VMDATAH
    writeVRAM(1, data);
    if(io.vramIncrementMode == 1) io.vramAddress += io.vramIncrementSize;
    return;
  }

  case 0x211a: {  //M7SEL
    io.mode7.hflip  = data.bit (0);
    io.mode7.vflip  = data.bit (1);
    io.mode7.repeat = data.bits(6,7);
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
      writeCGRAM(io.cgramAddress++, data.bits(0,6) << 8 | latch.cgram);
    }
    return;
  }

  case 0x2123: {  //W12SEL
    io.bg1.window.oneInvert = data.bit(0);
    io.bg1.window.oneEnable = data.bit(1);
    io.bg1.window.twoInvert = data.bit(2);
    io.bg1.window.twoEnable = data.bit(3);
    io.bg2.window.oneInvert = data.bit(4);
    io.bg2.window.oneEnable = data.bit(5);
    io.bg2.window.twoInvert = data.bit(6);
    io.bg2.window.twoEnable = data.bit(7);
    return;
  }

  case 0x2124: {  //W34SEL
    io.bg3.window.oneInvert = data.bit(0);
    io.bg3.window.oneEnable = data.bit(1);
    io.bg3.window.twoInvert = data.bit(2);
    io.bg3.window.twoEnable = data.bit(3);
    io.bg4.window.oneInvert = data.bit(4);
    io.bg4.window.oneEnable = data.bit(5);
    io.bg4.window.twoInvert = data.bit(6);
    io.bg4.window.twoEnable = data.bit(7);
    return;
  }

  case 0x2125: {  //WOBJSEL
    io.obj.window.oneInvert = data.bit(0);
    io.obj.window.oneEnable = data.bit(1);
    io.obj.window.twoInvert = data.bit(2);
    io.obj.window.twoEnable = data.bit(3);
    io.col.window.oneInvert = data.bit(4);
    io.col.window.oneEnable = data.bit(5);
    io.col.window.twoInvert = data.bit(6);
    io.col.window.twoEnable = data.bit(7);
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
    io.bg1.window.mask = data.bits(0,1);
    io.bg2.window.mask = data.bits(2,3);
    io.bg3.window.mask = data.bits(4,5);
    io.bg4.window.mask = data.bits(6,7);
    return;
  }

  case 0x212b: {  //WOBJLOG
    io.obj.window.mask = data.bits(0,1);
    io.col.window.mask = data.bits(2,3);
    return;
  }

  case 0x212c: {  //TM
    io.bg1.aboveEnable = data.bit(0);
    io.bg2.aboveEnable = data.bit(1);
    io.bg3.aboveEnable = data.bit(2);
    io.bg4.aboveEnable = data.bit(3);
    io.obj.aboveEnable = data.bit(4);
    return;
  }

  case 0x212d: {  //TS
    io.bg1.belowEnable = data.bit(0);
    io.bg2.belowEnable = data.bit(1);
    io.bg3.belowEnable = data.bit(2);
    io.bg4.belowEnable = data.bit(3);
    io.obj.belowEnable = data.bit(4);
    return;
  }

  case 0x212e: {  //TMW
    io.bg1.window.aboveEnable = data.bit(0);
    io.bg2.window.aboveEnable = data.bit(1);
    io.bg3.window.aboveEnable = data.bit(2);
    io.bg4.window.aboveEnable = data.bit(3);
    io.obj.window.aboveEnable = data.bit(4);
    return;
  }

  case 0x212f: {  //TSW
    io.bg1.window.belowEnable = data.bit(0);
    io.bg2.window.belowEnable = data.bit(1);
    io.bg3.window.belowEnable = data.bit(2);
    io.bg4.window.belowEnable = data.bit(3);
    io.obj.window.belowEnable = data.bit(4);
    return;
  }

  case 0x2130: {  //CGWSEL
    io.col.directColor      = data.bit (0);
    io.col.blendMode        = data.bit (1);
    io.col.window.belowMask = data.bits(4,5);
    io.col.window.aboveMask = data.bits(6,7);
    return;
  }

  case 0x2131: {  //CGADDSUB
    io.col.enable[Source::BG1 ] = data.bit(0);
    io.col.enable[Source::BG2 ] = data.bit(1);
    io.col.enable[Source::BG3 ] = data.bit(2);
    io.col.enable[Source::BG4 ] = data.bit(3);
    io.col.enable[Source::OBJ1] = 0;
    io.col.enable[Source::OBJ2] = data.bit(4);
    io.col.enable[Source::COL ] = data.bit(5);
    io.col.halve                = data.bit(6);
    io.col.mathMode             = data.bit(7);
    return;
  }

  case 0x2132: {  //COLDATA
    if(data.bit(5)) io.col.fixedColor.bits( 0, 4) = data.bits(0,4);
    if(data.bit(6)) io.col.fixedColor.bits( 5, 9) = data.bits(0,4);
    if(data.bit(7)) io.col.fixedColor.bits(10,14) = data.bits(0,4);
    return;
  }

  case 0x2133: {  //SETINI
    io.interlace     = data.bit(0);
    io.obj.interlace = data.bit(1);
    io.overscan      = data.bit(2);
    io.pseudoHires   = data.bit(3);
    io.extbg         = data.bit(6);
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
