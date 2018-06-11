auto PPU::latchCounters() -> void {
  if(system.fastPPU()) {
    return ppufast.latchCounters();
  }

  cpu.synchronize(ppu);
  io.hcounter = hdot();
  io.vcounter = vcounter();
  latch.counters = 1;
}

auto PPU::addressVRAM() const -> uint16 {
  uint16 address = io.vramAddress;
  switch(io.vramMapping) {
  case 0: return address;
  case 1: return address.bits( 8,15) <<  8 | address.bits(0,4) << 3 | address.bits(5,7);
  case 2: return address.bits( 9,15) <<  9 | address.bits(0,5) << 3 | address.bits(6,8);
  case 3: return address.bits(10,15) << 10 | address.bits(0,6) << 3 | address.bits(7,9);
  }
  unreachable;
}

auto PPU::readVRAM() -> uint16 {
  if(!io.displayDisable && vcounter() < vdisp()) return 0x0000;
  auto address = addressVRAM();
  return vram[address];
}

auto PPU::writeVRAM(bool byte, uint8 data) -> void {
  if(!io.displayDisable && vcounter() < vdisp()) return;
  auto address = addressVRAM();
  vram[address].byte(byte) = data;
}

auto PPU::readOAM(uint10 addr) -> uint8 {
  if(!io.displayDisable && vcounter() < vdisp()) addr = latch.oamAddress;
  return obj.oam.read(addr);
}

auto PPU::writeOAM(uint10 addr, uint8 data) -> void {
  if(!io.displayDisable && vcounter() < vdisp()) addr = latch.oamAddress;
  obj.oam.write(addr, data);
}

auto PPU::readCGRAM(bool byte, uint8 addr) -> uint8 {
  if(!io.displayDisable
  && vcounter() > 0 && vcounter() < vdisp()
  && hcounter() >= 88 && hcounter() < 1096
  ) addr = latch.cgramAddress;
  return screen.cgram[addr].byte(byte);
}

auto PPU::writeCGRAM(uint8 addr, uint15 data) -> void {
  if(!io.displayDisable
  && vcounter() > 0 && vcounter() < vdisp()
  && hcounter() >= 88 && hcounter() < 1096
  ) addr = latch.cgramAddress;
  screen.cgram[addr] = data;
}

auto PPU::readIO(uint24 addr, uint8 data) -> uint8 {
  cpu.synchronize(ppu);

  switch((uint16)addr) {

  case 0x2104: case 0x2105: case 0x2106: case 0x2108:
  case 0x2109: case 0x210a: case 0x2114: case 0x2115:
  case 0x2116: case 0x2118: case 0x2119: case 0x211a:
  case 0x2124: case 0x2125: case 0x2126: case 0x2128:
  case 0x2129: case 0x212a: {
    return ppu1.mdr;
  }

  //MPYL
  case 0x2134: {
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(0);
  }

  //MPYM
  case 0x2135: {
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(1);
  }

  //MPYH
  case 0x2136: {
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(2);
  }

  //SLHV
  case 0x2137: {
    if(cpu.pio().bit(7)) latchCounters();
    return data;  //CPU MDR
  }

  //OAMDATAREAD
  case 0x2138: {
    ppu1.mdr = readOAM(io.oamAddress++);
    obj.setFirstSprite();
    return ppu1.mdr;
  }

  //VMDATALREAD
  case 0x2139: {
    ppu1.mdr = latch.vram.byte(0);
    if(io.vramIncrementMode == 0) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //VMDATAHREAD
  case 0x213a: {
    ppu1.mdr = latch.vram.byte(1);
    if(io.vramIncrementMode == 1) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //CGDATAREAD
  case 0x213b: {
    if(io.cgramAddressLatch++ == 0) {
      ppu2.mdr.bits(0,7) = readCGRAM(0, io.cgramAddress);
    } else {
      ppu2.mdr.bits(0,6) = readCGRAM(1, io.cgramAddress++);
    }
    return ppu2.mdr;
  }

  //OPHCT
  case 0x213c: {
    if(latch.hcounter++ == 0) {
      ppu2.mdr.bits(0,7) = io.hcounter.bits(0,7);
    } else {
      ppu2.mdr.bit (0  ) = io.hcounter.bit (8  );
    }
    return ppu2.mdr;
  }

  //OPVCT
  case 0x213d: {
    if(latch.vcounter++ == 0) {
      ppu2.mdr.bits(0,7) = io.vcounter.bits(0,7);
    } else {
      ppu2.mdr.bit (0  ) = io.vcounter.bit (8  );
    }
    return ppu2.mdr;
  }

  //STAT77
  case 0x213e: {
    ppu1.mdr.bits(0,3) = ppu1.version;
    ppu1.mdr.bit (  5) = 0;
    ppu1.mdr.bit (  6) = obj.io.rangeOver;
    ppu1.mdr.bit (  7) = obj.io.timeOver;
    return ppu1.mdr;
  }

  //STAT78
  case 0x213f: {
    latch.hcounter = 0;
    latch.vcounter = 0;
    ppu2.mdr.bits(0,3) = ppu2.version;
    ppu2.mdr.bit (  4) = Region::PAL();  //0 = NTSC, 1 = PAL
    if(!cpu.pio().bit(7)) {
      ppu2.mdr.bit( 6) = 1;
    } else {
      ppu2.mdr.bit( 6) = latch.counters;
      latch.counters = 0;
    }
    ppu2.mdr.bit (  7) = field();
    return ppu2.mdr;
  }

  }

  return data;
}

auto PPU::writeIO(uint24 addr, uint8 data) -> void {
  cpu.synchronize(ppu);

  switch((uint16)addr) {

  //INIDISP
  case 0x2100: {
    if(io.displayDisable && vcounter() == vdisp()) obj.addressReset();
    io.displayBrightness = data.bits(0,3);
    io.displayDisable    = data.bit (7);
    return;
  }

  //OBSEL
  case 0x2101: {
    obj.io.tiledataAddress = data.bits(0,2) << 13;
    obj.io.nameselect      = data.bits(3,4);
    obj.io.baseSize        = data.bits(5,7);
    return;
  }

  //OAMADDL
  case 0x2102: {
    io.oamBaseAddress = (io.oamBaseAddress & 0x0200) | data << 1;
    obj.addressReset();
    return;
  }

  //OAMADDH
  case 0x2103: {
    io.oamBaseAddress = data.bit(0) << 9 | (io.oamBaseAddress & 0x01fe);
    io.oamPriority    = data.bit(7);
    obj.addressReset();
    return;
  }

  //OAMDATA
  case 0x2104: {
    uint1 latchBit = io.oamAddress.bit(0);
    uint10 address = io.oamAddress++;
    if(latchBit == 0) latch.oam = data;
    if(address.bit(9)) {
      writeOAM(address, data);
    } else if(latchBit == 1) {
      writeOAM((address & ~1) + 0, latch.oam);
      writeOAM((address & ~1) + 1, data);
    }
    obj.setFirstSprite();
    return;
  }

  //BGMODE
  case 0x2105: {
    io.bgMode       = data.bits(0,2);
    io.bgPriority   = data.bit (  3);
    bg1.io.tileSize = data.bit (  4);
    bg2.io.tileSize = data.bit (  5);
    bg3.io.tileSize = data.bit (  6);
    bg4.io.tileSize = data.bit (  7);
    updateVideoMode();
    return;
  }

  //MOSAIC
  case 0x2106: {
    bg1.mosaic.enable = data.bit(0);
    bg2.mosaic.enable = data.bit(1);
    bg3.mosaic.enable = data.bit(2);
    bg4.mosaic.enable = data.bit(3);
    Background::Mosaic::size = data.bits(4,7);
    return;
  }

  //BG1SC
  case 0x2107: {
    bg1.io.screenSize    = data.bits(0,1);
    bg1.io.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG2SC
  case 0x2108: {
    bg2.io.screenSize    = data.bits(0,1);
    bg2.io.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG3SC
  case 0x2109: {
    bg3.io.screenSize    = data.bits(0,1);
    bg3.io.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG4SC
  case 0x210a: {
    bg4.io.screenSize    = data.bits(0,1);
    bg4.io.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG12NBA
  case 0x210b: {
    bg1.io.tiledataAddress = data.bits(0,3) << 12;
    bg2.io.tiledataAddress = data.bits(4,7) << 12;
    return;
  }

  //BG34NBA
  case 0x210c: {
    bg3.io.tiledataAddress = data.bits(0,3) << 12;
    bg4.io.tiledataAddress = data.bits(4,7) << 12;
    return;
  }

  //BG1HOFS
  case 0x210d: {
    io.hoffsetMode7 = data << 8 | latch.mode7;
    latch.mode7 = data;

    bg1.io.hoffset = data << 8 | (latch.bgofsPPU1 & ~7) | (latch.bgofsPPU2 & 7);
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG1VOFS
  case 0x210e: {
    io.voffsetMode7 = data << 8 | latch.mode7;
    latch.mode7 = data;

    bg1.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //BG2HOFS
  case 0x210f: {
    bg2.io.hoffset = data << 8 | (latch.bgofsPPU1 & ~7) | (latch.bgofsPPU2 & 7);
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG2VOFS
  case 0x2110: {
    bg2.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //BG3HOFS
  case 0x2111: {
    bg3.io.hoffset = data << 8 | (latch.bgofsPPU1 & ~7) | (latch.bgofsPPU2 & 7);
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG3VOFS
  case 0x2112: {
    bg3.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //BG4HOFS
  case 0x2113: {
    bg4.io.hoffset = data << 8 | (latch.bgofsPPU1 & ~7) | (latch.bgofsPPU2 & 7);
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG4VOFS
  case 0x2114: {
    bg4.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //VMAIN
  case 0x2115: {
    static const uint size[4] = {1, 32, 128, 128};
    io.vramIncrementSize = size[data.bits(0,1)];
    io.vramMapping       = data.bits(2,3);
    io.vramIncrementMode = data.bit (  7);
    return;
  }

  //VMADDL
  case 0x2116: {
    io.vramAddress.byte(0) = data;
    latch.vram = readVRAM();
    return;
  }

  //VMADDH
  case 0x2117: {
    io.vramAddress.byte(1) = data;
    latch.vram = readVRAM();
    return;
  }

  //VMDATAL
  case 0x2118: {
    writeVRAM(0, data);
    if(io.vramIncrementMode == 0) io.vramAddress += io.vramIncrementSize;
    return;
  }

  //VMDATAH
  case 0x2119: {
    writeVRAM(1, data);
    if(io.vramIncrementMode == 1) io.vramAddress += io.vramIncrementSize;
    return;
  }

  //M7SEL
  case 0x211a: {
    io.hflipMode7  = data.bit (  0);
    io.vflipMode7  = data.bit (  1);
    io.repeatMode7 = data.bits(6,7);
    return;
  }

  //M7A
  case 0x211b: {
    io.m7a = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7B
  case 0x211c: {
    io.m7b = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7C
  case 0x211d: {
    io.m7c = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7D
  case 0x211e: {
    io.m7d = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7X
  case 0x211f: {
    io.m7x = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7Y
  case 0x2120: {
    io.m7y = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //CGADD
  case 0x2121: {
    io.cgramAddress = data;
    io.cgramAddressLatch = 0;
    return;
  }

  //CGDATA
  case 0x2122: {
    if(io.cgramAddressLatch++ == 0) {
      latch.cgram = data;
    } else {
      writeCGRAM(io.cgramAddress++, data.bits(0,6) << 8 | latch.cgram);
    }
    return;
  }

  //W12SEL
  case 0x2123: {
    window.io.bg1.oneInvert = data.bit(0);
    window.io.bg1.oneEnable = data.bit(1);
    window.io.bg1.twoInvert = data.bit(2);
    window.io.bg1.twoEnable = data.bit(3);
    window.io.bg2.oneInvert = data.bit(4);
    window.io.bg2.oneEnable = data.bit(5);
    window.io.bg2.twoInvert = data.bit(6);
    window.io.bg2.twoEnable = data.bit(7);
    return;
  }

  //W34SEL
  case 0x2124: {
    window.io.bg3.oneInvert = data.bit(0);
    window.io.bg3.oneEnable = data.bit(1);
    window.io.bg3.twoInvert = data.bit(2);
    window.io.bg3.twoEnable = data.bit(3);
    window.io.bg4.oneInvert = data.bit(4);
    window.io.bg4.oneEnable = data.bit(5);
    window.io.bg4.twoInvert = data.bit(6);
    window.io.bg4.twoEnable = data.bit(7);
    return;
  }

  //WOBJSEL
  case 0x2125: {
    window.io.obj.oneInvert = data.bit(0);
    window.io.obj.oneEnable = data.bit(1);
    window.io.obj.twoInvert = data.bit(2);
    window.io.obj.twoEnable = data.bit(3);
    window.io.col.oneInvert = data.bit(4);
    window.io.col.oneEnable = data.bit(5);
    window.io.col.twoInvert = data.bit(6);
    window.io.col.twoEnable = data.bit(7);
    return;
  }

  //WH0
  case 0x2126: {
    window.io.oneLeft = data;
    return;
  }

  //WH1
  case 0x2127: {
    window.io.oneRight = data;
    return;
  }

  //WH2
  case 0x2128: {
    window.io.twoLeft = data;
    return;
  }

  //WH3
  case 0x2129: {
    window.io.twoRight = data;
    return;
  }

  //WBGLOG
  case 0x212a: {
    window.io.bg1.mask = data.bits(0,1);
    window.io.bg2.mask = data.bits(2,3);
    window.io.bg3.mask = data.bits(4,5);
    window.io.bg4.mask = data.bits(6,7);
    return;
  }

  //WOBJLOG
  case 0x212b: {
    window.io.obj.mask = data.bits(0,1);
    window.io.col.mask = data.bits(2,3);
    return;
  }

  //TM
  case 0x212c: {
    bg1.io.aboveEnable = data.bit(0);
    bg2.io.aboveEnable = data.bit(1);
    bg3.io.aboveEnable = data.bit(2);
    bg4.io.aboveEnable = data.bit(3);
    obj.io.aboveEnable = data.bit(4);
    return;
  }

  //TS
  case 0x212d: {
    bg1.io.belowEnable = data.bit(0);
    bg2.io.belowEnable = data.bit(1);
    bg3.io.belowEnable = data.bit(2);
    bg4.io.belowEnable = data.bit(3);
    obj.io.belowEnable = data.bit(4);
    return;
  }

  //TMW
  case 0x212e: {
    window.io.bg1.aboveEnable = data.bit(0);
    window.io.bg2.aboveEnable = data.bit(1);
    window.io.bg3.aboveEnable = data.bit(2);
    window.io.bg4.aboveEnable = data.bit(3);
    window.io.obj.aboveEnable = data.bit(4);
    return;
  }

  //TSW
  case 0x212f: {
    window.io.bg1.belowEnable = data.bit(0);
    window.io.bg2.belowEnable = data.bit(1);
    window.io.bg3.belowEnable = data.bit(2);
    window.io.bg4.belowEnable = data.bit(3);
    window.io.obj.belowEnable = data.bit(4);
    return;
  }

  //CGWSEL
  case 0x2130: {
    screen.io.directColor   = data.bit (  0);
    screen.io.blendMode     = data.bit (  1);
    window.io.col.belowMask = data.bits(4,5);
    window.io.col.aboveMask = data.bits(6,7);
    return;
  }

  //CGADDSUB
  case 0x2131: {
    screen.io.bg1.colorEnable  = data.bit(0);
    screen.io.bg2.colorEnable  = data.bit(1);
    screen.io.bg3.colorEnable  = data.bit(2);
    screen.io.bg4.colorEnable  = data.bit(3);
    screen.io.obj.colorEnable  = data.bit(4);
    screen.io.back.colorEnable = data.bit(5);
    screen.io.colorHalve       = data.bit(6);
    screen.io.colorMode        = data.bit(7);
    return;
  }

  //COLDATA
  case 0x2132: {
    if(data.bit(5)) screen.io.colorRed   = data.bits(0,4);
    if(data.bit(6)) screen.io.colorGreen = data.bits(0,4);
    if(data.bit(7)) screen.io.colorBlue  = data.bits(0,4);
    return;
  }

  //SETINI
  case 0x2133: {
    io.interlace     = data.bit(0);
    obj.io.interlace = data.bit(1);
    io.overscan      = data.bit(2);
    io.pseudoHires   = data.bit(3);
    io.extbg         = data.bit(6);
    updateVideoMode();
    return;
  }

  }
}

auto PPU::updateVideoMode() -> void {
  switch(io.bgMode) {
  case 0:
    bg1.io.mode = Background::Mode::BPP2;
    bg2.io.mode = Background::Mode::BPP2;
    bg3.io.mode = Background::Mode::BPP2;
    bg4.io.mode = Background::Mode::BPP2;
    memory::assign(bg1.io.priority, 8, 11);
    memory::assign(bg2.io.priority, 7, 10);
    memory::assign(bg3.io.priority, 2,  5);
    memory::assign(bg4.io.priority, 1,  4);
    memory::assign(obj.io.priority, 3,  6, 9, 12);
    break;

  case 1:
    bg1.io.mode = Background::Mode::BPP4;
    bg2.io.mode = Background::Mode::BPP4;
    bg3.io.mode = Background::Mode::BPP2;
    bg4.io.mode = Background::Mode::Inactive;
    if(io.bgPriority) {
      memory::assign(bg1.io.priority, 5,  8);
      memory::assign(bg2.io.priority, 4,  7);
      memory::assign(bg3.io.priority, 1, 10);
      memory::assign(obj.io.priority, 2,  3, 6,  9);
    } else {
      memory::assign(bg1.io.priority, 6,  9);
      memory::assign(bg2.io.priority, 5,  8);
      memory::assign(bg3.io.priority, 1,  3);
      memory::assign(obj.io.priority, 2,  4, 7, 10);
    }
    break;

  case 2:
    bg1.io.mode = Background::Mode::BPP4;
    bg2.io.mode = Background::Mode::BPP4;
    bg3.io.mode = Background::Mode::Inactive;
    bg4.io.mode = Background::Mode::Inactive;
    memory::assign(bg1.io.priority, 3, 7);
    memory::assign(bg2.io.priority, 1, 5);
    memory::assign(obj.io.priority, 2, 4, 6, 8);
    break;

  case 3:
    bg1.io.mode = Background::Mode::BPP8;
    bg2.io.mode = Background::Mode::BPP4;
    bg3.io.mode = Background::Mode::Inactive;
    bg4.io.mode = Background::Mode::Inactive;
    memory::assign(bg1.io.priority, 3, 7);
    memory::assign(bg2.io.priority, 1, 5);
    memory::assign(obj.io.priority, 2, 4, 6, 8);
    break;

  case 4:
    bg1.io.mode = Background::Mode::BPP8;
    bg2.io.mode = Background::Mode::BPP2;
    bg3.io.mode = Background::Mode::Inactive;
    bg4.io.mode = Background::Mode::Inactive;
    memory::assign(bg1.io.priority, 3, 7);
    memory::assign(bg2.io.priority, 1, 5);
    memory::assign(obj.io.priority, 2, 4, 6, 8);
    break;

  case 5:
    bg1.io.mode = Background::Mode::BPP4;
    bg2.io.mode = Background::Mode::BPP2;
    bg3.io.mode = Background::Mode::Inactive;
    bg4.io.mode = Background::Mode::Inactive;
    memory::assign(bg1.io.priority, 3, 7);
    memory::assign(bg2.io.priority, 1, 5);
    memory::assign(obj.io.priority, 2, 4, 6, 8);
    break;

  case 6:
    bg1.io.mode = Background::Mode::BPP4;
    bg2.io.mode = Background::Mode::Inactive;
    bg3.io.mode = Background::Mode::Inactive;
    bg4.io.mode = Background::Mode::Inactive;
    memory::assign(bg1.io.priority, 2, 5);
    memory::assign(obj.io.priority, 1, 3, 4, 6);
    break;

  case 7:
    if(!io.extbg) {
      bg1.io.mode = Background::Mode::Mode7;
      bg2.io.mode = Background::Mode::Inactive;
      bg3.io.mode = Background::Mode::Inactive;
      bg4.io.mode = Background::Mode::Inactive;
      memory::assign(bg1.io.priority, 2);
      memory::assign(obj.io.priority, 1, 3, 4, 5);
    } else {
      bg1.io.mode = Background::Mode::Mode7;
      bg2.io.mode = Background::Mode::Mode7;
      bg3.io.mode = Background::Mode::Inactive;
      bg4.io.mode = Background::Mode::Inactive;
      memory::assign(bg1.io.priority, 3);
      memory::assign(bg2.io.priority, 1, 5);
      memory::assign(obj.io.priority, 2, 4, 6, 7);
    }
    break;
  }
}
