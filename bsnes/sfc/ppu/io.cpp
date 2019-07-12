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
  case 1: return bits(address, 8-15) <<  8 | bits(address,0-4) << 3 | bits(address,5-7);
  case 2: return bits(address, 9-15) <<  9 | bits(address,0-5) << 3 | bits(address,6-8);
  case 3: return bits(address,10-15) << 10 | bits(address,0-6) << 3 | bits(address,7-9);
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
  bit8(vram[address],byte) = data;
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

auto PPU::readIO(uint addr, uint8 data) -> uint8 {
  cpu.synchronize(ppu);

  switch(addr & 0xffff) {

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
    if(cbit1(cpu.pio(),7)) latchCounters();
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
    ppu1.mdr = bit8(latch.vram,0);
    if(io.vramIncrementMode == 0) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //VMDATAHREAD
  case 0x213a: {
    ppu1.mdr = bit8(latch.vram,1);
    if(io.vramIncrementMode == 1) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //CGDATAREAD
  case 0x213b: {
    if(io.cgramAddressLatch++ == 0) {
      bits(ppu2.mdr,0-7) = readCGRAM(0, io.cgramAddress);
    } else {
      bits(ppu2.mdr,0-6) = readCGRAM(1, io.cgramAddress++);
    }
    return ppu2.mdr;
  }

  //OPHCT
  case 0x213c: {
    if(latch.hcounter++ == 0) {
      bits(ppu2.mdr,0-7) = bits(io.hcounter,0-7);
    } else {
      bit1(ppu2.mdr,0  ) = bit1(io.hcounter,8  );
    }
    return ppu2.mdr;
  }

  //OPVCT
  case 0x213d: {
    if(latch.vcounter++ == 0) {
      bits(ppu2.mdr,0-7) = bits(io.vcounter,0-7);
    } else {
      bit1(ppu2.mdr,0  ) = bit1(io.vcounter,8  );
    }
    return ppu2.mdr;
  }

  //STAT77
  case 0x213e: {
    bits(ppu1.mdr,0-3) = ppu1.version;
    bit1(ppu1.mdr,  5) = 0;
    bit1(ppu1.mdr,  6) = obj.io.rangeOver;
    bit1(ppu1.mdr,  7) = obj.io.timeOver;
    return ppu1.mdr;
  }

  //STAT78
  case 0x213f: {
    latch.hcounter = 0;
    latch.vcounter = 0;
    bits(ppu2.mdr,0-3) = ppu2.version;
    bit1(ppu2.mdr,  4) = Region::PAL();  //0 = NTSC, 1 = PAL
    if(!cbit1(cpu.pio(),7)) {
      bit1(ppu2.mdr, 6) = 1;
    } else {
      bit1(ppu2.mdr, 6) = latch.counters;
      latch.counters = 0;
    }
    bit1(ppu2.mdr,  7) = field();
    return ppu2.mdr;
  }

  }

  return data;
}

auto PPU::writeIO(uint addr, uint8 data) -> void {
  cpu.synchronize(ppu);

  switch(addr & 0xffff) {

  //INIDISP
  case 0x2100: {
    if(io.displayDisable && vcounter() == vdisp()) obj.addressReset();
    io.displayBrightness = bits(data,0-3);
    io.displayDisable    = bit1(data,7);
    return;
  }

  //OBSEL
  case 0x2101: {
    obj.io.tiledataAddress = bits(data,0-2) << 13;
    obj.io.nameselect      = bits(data,3-4);
    obj.io.baseSize        = bits(data,5-7);
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
    io.oamBaseAddress = bit1(data,0) << 9 | (io.oamBaseAddress & 0x01fe);
    io.oamPriority    = bit1(data,7);
    obj.addressReset();
    return;
  }

  //OAMDATA
  case 0x2104: {
    uint1 latchBit = bit1(io.oamAddress,0);
    uint10 address = io.oamAddress++;
    if(latchBit == 0) latch.oam = data;
    if(bit1(address,9)) {
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
    io.bgMode       = bits(data,0-2);
    io.bgPriority   = bit1(data,  3);
    bg1.io.tileSize = bit1(data,  4);
    bg2.io.tileSize = bit1(data,  5);
    bg3.io.tileSize = bit1(data,  6);
    bg4.io.tileSize = bit1(data,  7);
    updateVideoMode();
    return;
  }

  //MOSAIC
  case 0x2106: {
    bg1.mosaic.enable = bit1(data,0);
    bg2.mosaic.enable = bit1(data,1);
    bg3.mosaic.enable = bit1(data,2);
    bg4.mosaic.enable = bit1(data,3);
    Background::Mosaic::size = bits(data,4-7);
    return;
  }

  //BG1SC
  case 0x2107: {
    bg1.io.screenSize    = bits(data,0-1);
    bg1.io.screenAddress = bits(data,2-7) << 10;
    return;
  }

  //BG2SC
  case 0x2108: {
    bg2.io.screenSize    = bits(data,0-1);
    bg2.io.screenAddress = bits(data,2-7) << 10;
    return;
  }

  //BG3SC
  case 0x2109: {
    bg3.io.screenSize    = bits(data,0-1);
    bg3.io.screenAddress = bits(data,2-7) << 10;
    return;
  }

  //BG4SC
  case 0x210a: {
    bg4.io.screenSize    = bits(data,0-1);
    bg4.io.screenAddress = bits(data,2-7) << 10;
    return;
  }

  //BG12NBA
  case 0x210b: {
    bg1.io.tiledataAddress = bits(data,0-3) << 12;
    bg2.io.tiledataAddress = bits(data,4-7) << 12;
    return;
  }

  //BG34NBA
  case 0x210c: {
    bg3.io.tiledataAddress = bits(data,0-3) << 12;
    bg4.io.tiledataAddress = bits(data,4-7) << 12;
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
    io.vramIncrementSize = size[data & 3];
    io.vramMapping       = bits(data,2-3);
    io.vramIncrementMode = bit1(data,  7);
    return;
  }

  //VMADDL
  case 0x2116: {
    bit8(io.vramAddress,0) = data;
    latch.vram = readVRAM();
    return;
  }

  //VMADDH
  case 0x2117: {
    bit8(io.vramAddress,1) = data;
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
    io.hflipMode7  = bit1(data,  0);
    io.vflipMode7  = bit1(data,  1);
    io.repeatMode7 = bits(data,6-7);
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
      writeCGRAM(io.cgramAddress++, bits(data,0-6) << 8 | latch.cgram);
    }
    return;
  }

  //W12SEL
  case 0x2123: {
    window.io.bg1.oneInvert = bit1(data,0);
    window.io.bg1.oneEnable = bit1(data,1);
    window.io.bg1.twoInvert = bit1(data,2);
    window.io.bg1.twoEnable = bit1(data,3);
    window.io.bg2.oneInvert = bit1(data,4);
    window.io.bg2.oneEnable = bit1(data,5);
    window.io.bg2.twoInvert = bit1(data,6);
    window.io.bg2.twoEnable = bit1(data,7);
    return;
  }

  //W34SEL
  case 0x2124: {
    window.io.bg3.oneInvert = bit1(data,0);
    window.io.bg3.oneEnable = bit1(data,1);
    window.io.bg3.twoInvert = bit1(data,2);
    window.io.bg3.twoEnable = bit1(data,3);
    window.io.bg4.oneInvert = bit1(data,4);
    window.io.bg4.oneEnable = bit1(data,5);
    window.io.bg4.twoInvert = bit1(data,6);
    window.io.bg4.twoEnable = bit1(data,7);
    return;
  }

  //WOBJSEL
  case 0x2125: {
    window.io.obj.oneInvert = bit1(data,0);
    window.io.obj.oneEnable = bit1(data,1);
    window.io.obj.twoInvert = bit1(data,2);
    window.io.obj.twoEnable = bit1(data,3);
    window.io.col.oneInvert = bit1(data,4);
    window.io.col.oneEnable = bit1(data,5);
    window.io.col.twoInvert = bit1(data,6);
    window.io.col.twoEnable = bit1(data,7);
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
    window.io.bg1.mask = bits(data,0-1);
    window.io.bg2.mask = bits(data,2-3);
    window.io.bg3.mask = bits(data,4-5);
    window.io.bg4.mask = bits(data,6-7);
    return;
  }

  //WOBJLOG
  case 0x212b: {
    window.io.obj.mask = bits(data,0-1);
    window.io.col.mask = bits(data,2-3);
    return;
  }

  //TM
  case 0x212c: {
    bg1.io.aboveEnable = bit1(data,0);
    bg2.io.aboveEnable = bit1(data,1);
    bg3.io.aboveEnable = bit1(data,2);
    bg4.io.aboveEnable = bit1(data,3);
    obj.io.aboveEnable = bit1(data,4);
    return;
  }

  //TS
  case 0x212d: {
    bg1.io.belowEnable = bit1(data,0);
    bg2.io.belowEnable = bit1(data,1);
    bg3.io.belowEnable = bit1(data,2);
    bg4.io.belowEnable = bit1(data,3);
    obj.io.belowEnable = bit1(data,4);
    return;
  }

  //TMW
  case 0x212e: {
    window.io.bg1.aboveEnable = bit1(data,0);
    window.io.bg2.aboveEnable = bit1(data,1);
    window.io.bg3.aboveEnable = bit1(data,2);
    window.io.bg4.aboveEnable = bit1(data,3);
    window.io.obj.aboveEnable = bit1(data,4);
    return;
  }

  //TSW
  case 0x212f: {
    window.io.bg1.belowEnable = bit1(data,0);
    window.io.bg2.belowEnable = bit1(data,1);
    window.io.bg3.belowEnable = bit1(data,2);
    window.io.bg4.belowEnable = bit1(data,3);
    window.io.obj.belowEnable = bit1(data,4);
    return;
  }

  //CGWSEL
  case 0x2130: {
    screen.io.directColor   = bit1(data,  0);
    screen.io.blendMode     = bit1(data,  1);
    window.io.col.belowMask = bits(data,4-5);
    window.io.col.aboveMask = bits(data,6-7);
    return;
  }

  //CGADDSUB
  case 0x2131: {
    screen.io.bg1.colorEnable  = bit1(data,0);
    screen.io.bg2.colorEnable  = bit1(data,1);
    screen.io.bg3.colorEnable  = bit1(data,2);
    screen.io.bg4.colorEnable  = bit1(data,3);
    screen.io.obj.colorEnable  = bit1(data,4);
    screen.io.back.colorEnable = bit1(data,5);
    screen.io.colorHalve       = bit1(data,6);
    screen.io.colorMode        = bit1(data,7);
    return;
  }

  //COLDATA
  case 0x2132: {
    if(bit1(data,5)) screen.io.colorRed   = bits(data,0-4);
    if(bit1(data,6)) screen.io.colorGreen = bits(data,0-4);
    if(bit1(data,7)) screen.io.colorBlue  = bits(data,0-4);
    return;
  }

  //SETINI
  case 0x2133: {
    io.interlace     = bit1(data,0);
    obj.io.interlace = bit1(data,1);
    io.overscan      = bit1(data,2);
    io.pseudoHires   = bit1(data,3);
    io.extbg         = bit1(data,6);
    updateVideoMode();
    return;
  }

  }
}

auto PPU::updateVideoMode() -> void {
  display.vdisp = !io.overscan ? 225 : 240;

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
