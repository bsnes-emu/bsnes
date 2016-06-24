auto PPU::read(uint24 addr, uint8 data) -> uint8 {
  cpu.synchronizePPU();

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
    uint result = (int16)r.m7a * (int8)(r.m7b >> 8);
    ppu1.mdr = (result >>  0);
    return ppu1.mdr;
  }

  //MPYM
  case 0x2135: {
    uint result = (int16)r.m7a * (int8)(r.m7b >> 8);
    ppu1.mdr = (result >>  8);
    return ppu1.mdr;
  }

  //MPYH
  case 0x2136: {
    uint result = (int16)r.m7a * (int8)(r.m7b >> 8);
    ppu1.mdr = (result >> 16);
    return ppu1.mdr;
  }

  //SLHV
  case 0x2137: {
    if(cpu.pio() & 0x80) latchCounters();
    return cpu.r.mdr;
  }

  //OAMDATAREAD
  case 0x2138: {
    uint10 address = r.oamAddress++;
    if(!r.displayDisable && vcounter() < vdisp()) address = latch.oamAddress;
    if(address & 0x0200) address &= 0x021f;

    ppu1.mdr = oamRead(address);
    obj.setFirstSprite();
    return ppu1.mdr;
  }

  //VMDATALREAD
  case 0x2139: {
    auto address = getVramAddress();
    ppu1.mdr = latch.vram >> 0;
    if(r.vramIncrementMode == 0) {
      latch.vram.byte(0) = vramRead(0, address);
      latch.vram.byte(1) = vramRead(1, address);
      r.vramAddress += r.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //VMDATAHREAD
  case 0x213a: {
    uint16 address = getVramAddress();
    ppu1.mdr = latch.vram >> 8;
    if(r.vramIncrementMode == 1) {
      latch.vram.byte(0) = vramRead(0, address);
      latch.vram.byte(1) = vramRead(1, address);
      r.vramAddress += r.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //CGDATAREAD
  case 0x213b: {
    bool l = r.cgramAddress & 1;
    uint9 address = r.cgramAddress++;
    if(!r.displayDisable
    && vcounter() > 0 && vcounter() < vdisp()
    && hcounter() >= 88 && hcounter() < 1096
    ) address = latch.cgramAddress;

    if(l == 0) {
      ppu2.mdr  = cgramRead(address);
    } else {
      ppu2.mdr &= 0x80;
      ppu2.mdr |= cgramRead(address);
    }
    return ppu2.mdr;
  }

  //OPHCT
  case 0x213c: {
    if(latch.hcounter == 0) {
      ppu2.mdr  = (r.hcounter >> 0);
    } else {
      ppu2.mdr &= 0xfe;
      ppu2.mdr |= (r.hcounter >> 8) & 1;
    }
    latch.hcounter ^= 1;
    return ppu2.mdr;
  }

  //OPVCT
  case 0x213d: {
    if(latch.vcounter == 0) {
      ppu2.mdr  = (r.vcounter >> 0);
    } else {
      ppu2.mdr &= 0xfe;
      ppu2.mdr |= (r.vcounter >> 8) & 1;
    }
    latch.vcounter ^= 1;
    return ppu2.mdr;
  }

  //STAT77
  case 0x213e: {
    ppu1.mdr &= 0x10;
    ppu1.mdr |= obj.r.timeOver << 7;
    ppu1.mdr |= obj.r.rangeOver << 6;
    ppu1.mdr |= ppu1.version & 0x0f;
    return ppu1.mdr;
  }

  //STAT78
  case 0x213f: {
    latch.hcounter = 0;
    latch.vcounter = 0;

    ppu2.mdr &= 0x20;
    ppu2.mdr |= field() << 7;
    if((cpu.pio() & 0x80) == 0) {
      ppu2.mdr |= 0x40;
    } else if(latch.counters) {
      ppu2.mdr |= 0x40;
      latch.counters = false;
    }
    ppu2.mdr |= (system.region() == System::Region::NTSC ? 0 : 1) << 4;
    ppu2.mdr |= ppu2.version & 0x0f;
    return ppu2.mdr;
  }

  }

  return data;
}

auto PPU::write(uint24 addr, uint8 data) -> void {
  cpu.synchronizePPU();

  switch((uint16)addr) {

  //INIDISP
  case 0x2100: {
    if(r.displayDisable && vcounter() == vdisp()) obj.addressReset();
    r.displayBrightness = data.bits(0,3);
    r.displayDisable    = data.bit (7);
    return;
  }

  //OBSEL
  case 0x2101: {
    obj.r.tiledataAddress = data.bits(0,2) << 13;
    obj.r.nameSelect      = data.bits(3,4);
    obj.r.baseSize        = data.bits(5,7);
    return;
  }

  //OAMADDL
  case 0x2102: {
    r.oamBaseAddress = (r.oamBaseAddress & 0x0200) | (data << 1);
    obj.addressReset();
    return;
  }

  //OAMADDH
  case 0x2103: {
    r.oamPriority = data & 0x80;
    r.oamBaseAddress = ((data & 0x01) << 9) | (r.oamBaseAddress & 0x01fe);
    obj.addressReset();
    return;
  }

  //OAMDATA
  case 0x2104: {
    bool l = r.oamAddress & 1;
    uint10 address = r.oamAddress++;
    if(!r.displayDisable && vcounter() < vdisp()) address = latch.oamAddress;
    if(address & 0x0200) address &= 0x021f;

    if(l == 0) latch.oam = data;
    if(address & 0x0200) {
      oamWrite(address, data);
    } else if(l == 1) {
      oamWrite((address & ~1) + 0, latch.oam);
      oamWrite((address & ~1) + 1, data);
    }
    obj.setFirstSprite();
    return;
  }

  //BGMODE
  case 0x2105: {
    r.bgMode       = data.bits(0,2);
    r.bgPriority   = data.bit (3);
    bg1.r.tileSize = data.bit (4);
    bg2.r.tileSize = data.bit (5);
    bg3.r.tileSize = data.bit (6);
    bg4.r.tileSize = data.bit (7);
    updateVideoMode();
    return;
  }

  //MOSAIC
  case 0x2106: {
    uint mosaicSize = data.bits(4,7);
    bg1.r.mosaic = data.bit(0) ? mosaicSize : 0;
    bg2.r.mosaic = data.bit(1) ? mosaicSize : 0;
    bg3.r.mosaic = data.bit(2) ? mosaicSize : 0;
    bg4.r.mosaic = data.bit(3) ? mosaicSize : 0;
    return;
  }

  //BG1SC
  case 0x2107: {
    bg1.r.screenSize    = data.bits(0,1);
    bg1.r.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG2SC
  case 0x2108: {
    bg2.r.screenSize    = data.bits(0,1);
    bg2.r.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG3SC
  case 0x2109: {
    bg3.r.screenSize    = data.bits(0,1);
    bg3.r.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG4SC
  case 0x210a: {
    bg4.r.screenSize    = data.bits(0,1);
    bg4.r.screenAddress = data.bits(2,7) << 10;
    return;
  }

  //BG12NBA
  case 0x210b: {
    bg1.r.tiledataAddress = data.bits(0,3) << 12;
    bg2.r.tiledataAddress = data.bits(4,7) << 12;
    return;
  }

  //BG34NBA
  case 0x210c: {
    bg3.r.tiledataAddress = data.bits(0,3) << 12;
    bg4.r.tiledataAddress = data.bits(4,7) << 12;
    return;
  }

  //BG1HOFS
  case 0x210d: {
    r.hoffsetMode7 = (data << 8) | latch.mode7;
    latch.mode7 = data;

    bg1.r.hoffset = (data << 8) | (latch.bgofs & ~7) | ((bg1.r.hoffset >> 8) & 7);
    latch.bgofs = data;
    return;
  }

  //BG1VOFS
  case 0x210e: {
    r.voffsetMode7 = (data << 8) | latch.mode7;
    latch.mode7 = data;

    bg1.r.voffset = (data << 8) | latch.bgofs;
    latch.bgofs = data;
    return;
  }

  //BG2HOFS
  case 0x210f: {
    bg2.r.hoffset = (data << 8) | (latch.bgofs & ~7) | ((bg2.r.hoffset >> 8) & 7);
    latch.bgofs = data;
    return;
  }

  //BG2VOFS
  case 0x2110: {
    bg2.r.voffset = (data << 8) | latch.bgofs;
    latch.bgofs = data;
    return;
  }

  //BG3HOFS
  case 0x2111: {
    bg3.r.hoffset = (data << 8) | (latch.bgofs & ~7) | ((bg3.r.hoffset >> 8) & 7);
    latch.bgofs = data;
    return;
  }

  //BG3VOFS
  case 0x2112: {
    bg3.r.voffset = (data << 8) | latch.bgofs;
    latch.bgofs = data;
    return;
  }

  //BG4HOFS
  case 0x2113: {
    bg4.r.hoffset = (data << 8) | (latch.bgofs & ~7) | ((bg4.r.hoffset >> 8) & 7);
    latch.bgofs = data;
    return;
  }

  //BG4VOFS
  case 0x2114: {
    bg4.r.voffset = (data << 8) | latch.bgofs;
    latch.bgofs = data;
    return;
  }

  //VMAIN
  case 0x2115: {
    r.vramIncrementMode = data & 0x80;
    r.vramMapping = (data >> 2) & 3;
    switch(data & 3) {
    case 0: r.vramIncrementSize =   1; break;
    case 1: r.vramIncrementSize =  32; break;
    case 2: r.vramIncrementSize = 128; break;
    case 3: r.vramIncrementSize = 128; break;
    }
    return;
  }

  //VMADDL
  case 0x2116: {
    r.vramAddress &= 0xff00;
    r.vramAddress |= (data << 0);
    auto address = getVramAddress();
    latch.vram.byte(0) = vramRead(0, address);
    latch.vram.byte(1) = vramRead(1, address);
    return;
  }

  //VMADDH
  case 0x2117: {
    r.vramAddress &= 0x00ff;
    r.vramAddress |= (data << 8);
    auto address = getVramAddress();
    latch.vram.byte(0) = vramRead(0, address);
    latch.vram.byte(1) = vramRead(1, address);
    return;
  }

  //VMDATAL
  case 0x2118: {
    auto address = getVramAddress();
    vramWrite(0, address, data);
    if(r.vramIncrementMode == 0) r.vramAddress += r.vramIncrementSize;
    return;
  }

  //VMDATAH
  case 0x2119: {
    auto address = getVramAddress();
    vramWrite(1, address, data);
    if(r.vramIncrementMode == 1) r.vramAddress += r.vramIncrementSize;
    return;
  }

  //M7SEL
  case 0x211a: {
    r.hflipMode7  = data.bit (0);
    r.vflipMode7  = data.bit (1);
    r.repeatMode7 = data.bits(6,7);
    return;
  }

  //M7A
  case 0x211b: {
    r.m7a = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7B
  case 0x211c: {
    r.m7b = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7C
  case 0x211d: {
    r.m7c = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7D
  case 0x211e: {
    r.m7d = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7X
  case 0x211f: {
    r.m7x = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7Y
  case 0x2120: {
    r.m7y = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //CGADD
  case 0x2121: {
    r.cgramAddress = data << 1;
    return;
  }

  //CGDATA
  case 0x2122: {
    bool l = r.cgramAddress & 1;
    uint9 address = r.cgramAddress++;
    if(!r.displayDisable
    && vcounter() > 0 && vcounter() < vdisp()
    && hcounter() >= 88 && hcounter() < 1096
    ) address = latch.cgramAddress;

    if(l == 0) {
      latch.cgram = data;
    } else {
      cgramWrite((address & ~1) + 0, latch.cgram);
      cgramWrite((address & ~1) + 1, data & 0x7f);
    }
    return;
  }

  //W12SEL
  case 0x2123: {
    window.r.bg1.oneInvert = data.bit(0);
    window.r.bg1.oneEnable = data.bit(1);
    window.r.bg1.twoInvert = data.bit(2);
    window.r.bg1.twoEnable = data.bit(3);
    window.r.bg2.oneInvert = data.bit(4);
    window.r.bg2.oneEnable = data.bit(5);
    window.r.bg2.twoInvert = data.bit(6);
    window.r.bg2.twoEnable = data.bit(7);
    return;
  }

  //W34SEL
  case 0x2124: {
    window.r.bg3.oneInvert = data.bit(0);
    window.r.bg3.oneEnable = data.bit(1);
    window.r.bg3.twoInvert = data.bit(2);
    window.r.bg3.twoEnable = data.bit(3);
    window.r.bg4.oneInvert = data.bit(4);
    window.r.bg4.oneEnable = data.bit(5);
    window.r.bg4.twoInvert = data.bit(6);
    window.r.bg4.twoEnable = data.bit(7);
    return;
  }

  //WOBJSEL
  case 0x2125: {
    window.r.obj.oneInvert = data.bit(0);
    window.r.obj.oneEnable = data.bit(1);
    window.r.obj.twoInvert = data.bit(2);
    window.r.obj.twoEnable = data.bit(3);
    window.r.col.oneInvert = data.bit(4);
    window.r.col.oneEnable = data.bit(5);
    window.r.col.twoInvert = data.bit(6);
    window.r.col.twoEnable = data.bit(7);
    return;
  }

  //WH0
  case 0x2126: {
    window.r.oneLeft = data;
    return;
  }

  //WH1
  case 0x2127: {
    window.r.oneRight = data;
    return;
  }

  //WH2
  case 0x2128: {
    window.r.twoLeft = data;
    return;
  }

  //WH3
  case 0x2129: {
    window.r.twoRight = data;
    return;
  }

  //WBGLOG
  case 0x212a: {
    window.r.bg1.mask = data.bits(0,1);
    window.r.bg2.mask = data.bits(2,3);
    window.r.bg3.mask = data.bits(4,5);
    window.r.bg4.mask = data.bits(6,7);
    return;
  }

  //WOBJLOG
  case 0x212b: {
    window.r.obj.mask = data.bits(0,1);
    window.r.col.mask = data.bits(2,3);
    return;
  }

  //TM
  case 0x212c: {
    bg1.r.aboveEnable = data.bit(0);
    bg2.r.aboveEnable = data.bit(1);
    bg3.r.aboveEnable = data.bit(2);
    bg4.r.aboveEnable = data.bit(3);
    obj.r.aboveEnable = data.bit(4);
    return;
  }

  //TS
  case 0x212d: {
    bg1.r.belowEnable = data.bit(0);
    bg2.r.belowEnable = data.bit(1);
    bg3.r.belowEnable = data.bit(2);
    bg4.r.belowEnable = data.bit(3);
    obj.r.belowEnable = data.bit(4);
    return;
  }

  //TMW
  case 0x212e: {
    window.r.bg1.aboveEnable = data.bit(0);
    window.r.bg2.aboveEnable = data.bit(1);
    window.r.bg3.aboveEnable = data.bit(2);
    window.r.bg4.aboveEnable = data.bit(3);
    window.r.obj.aboveEnable = data.bit(4);
    return;
  }

  //TSW
  case 0x212f: {
    window.r.bg1.belowEnable = data.bit(0);
    window.r.bg2.belowEnable = data.bit(1);
    window.r.bg3.belowEnable = data.bit(2);
    window.r.bg4.belowEnable = data.bit(3);
    window.r.obj.belowEnable = data.bit(4);
    return;
  }

  //CGWSEL
  case 0x2130: {
    screen.r.directColor   = data.bit (0);
    screen.r.blendMode     = data.bit (1);
    window.r.col.belowMask = data.bits(4,5);
    window.r.col.aboveMask = data.bits(6,7);
    return;
  }

  //CGADDSUB
  case 0x2131: {
    screen.r.bg1.colorEnable  = data.bit(0);
    screen.r.bg2.colorEnable  = data.bit(1);
    screen.r.bg3.colorEnable  = data.bit(2);
    screen.r.bg4.colorEnable  = data.bit(3);
    screen.r.obj.colorEnable  = data.bit(4);
    screen.r.back.colorEnable = data.bit(5);
    screen.r.colorHalve       = data.bit(6);
    screen.r.colorMode        = data.bit(7);
    return;
  }

  //COLDATA
  case 0x2132: {
    if(data.bit(5)) screen.r.colorRed   = data.bits(0,4);
    if(data.bit(6)) screen.r.colorGreen = data.bits(0,4);
    if(data.bit(7)) screen.r.colorBlue  = data.bits(0,4);
    return;
  }

  //SETINI
  case 0x2133: {
    r.interlace     = data.bit(0);
    obj.r.interlace = data.bit(1);
    r.overscan      = data.bit(2);
    r.pseudoHires   = data.bit(3);
    r.extbg         = data.bit(6);
    updateVideoMode();
    return;
  }

  }
}

auto PPU::latchCounters() -> void {
  cpu.synchronizePPU();
  r.hcounter = hdot();
  r.vcounter = vcounter();
  latch.counters = true;
}

auto PPU::updateVideoMode() -> void {
  switch(r.bgMode) {
  case 0:
    bg1.r.mode = Background::Mode::BPP2;
    bg2.r.mode = Background::Mode::BPP2;
    bg3.r.mode = Background::Mode::BPP2;
    bg4.r.mode = Background::Mode::BPP2;
    memory::assign(bg1.r.priority, 8, 11);
    memory::assign(bg2.r.priority, 7, 10);
    memory::assign(bg3.r.priority, 2,  5);
    memory::assign(bg4.r.priority, 1,  4);
    memory::assign(obj.r.priority, 3,  6, 9, 12);
    break;

  case 1:
    bg1.r.mode = Background::Mode::BPP4;
    bg2.r.mode = Background::Mode::BPP4;
    bg3.r.mode = Background::Mode::BPP2;
    bg4.r.mode = Background::Mode::Inactive;
    if(r.bgPriority) {
      memory::assign(bg1.r.priority, 5,  8);
      memory::assign(bg2.r.priority, 4,  7);
      memory::assign(bg3.r.priority, 1, 10);
      memory::assign(obj.r.priority, 2,  3, 6, 9);
    } else {
      memory::assign(bg1.r.priority, 6,  9);
      memory::assign(bg2.r.priority, 5,  8);
      memory::assign(bg3.r.priority, 1,  3);
      memory::assign(obj.r.priority, 2,  4, 7, 10);
    }
    break;

  case 2:
    bg1.r.mode = Background::Mode::BPP4;
    bg2.r.mode = Background::Mode::BPP4;
    bg3.r.mode = Background::Mode::Inactive;
    bg4.r.mode = Background::Mode::Inactive;
    memory::assign(bg1.r.priority, 3, 7);
    memory::assign(bg2.r.priority, 1, 5);
    memory::assign(obj.r.priority, 2, 4, 6, 8);
    break;

  case 3:
    bg1.r.mode = Background::Mode::BPP8;
    bg2.r.mode = Background::Mode::BPP4;
    bg3.r.mode = Background::Mode::Inactive;
    bg4.r.mode = Background::Mode::Inactive;
    memory::assign(bg1.r.priority, 3, 7);
    memory::assign(bg2.r.priority, 1, 5);
    memory::assign(obj.r.priority, 2, 4, 6, 8);
    break;

  case 4:
    bg1.r.mode = Background::Mode::BPP8;
    bg2.r.mode = Background::Mode::BPP2;
    bg3.r.mode = Background::Mode::Inactive;
    bg4.r.mode = Background::Mode::Inactive;
    memory::assign(bg1.r.priority, 3, 7);
    memory::assign(bg2.r.priority, 1, 5);
    memory::assign(obj.r.priority, 2, 4, 6, 8);
    break;

  case 5:
    bg1.r.mode = Background::Mode::BPP4;
    bg2.r.mode = Background::Mode::BPP2;
    bg3.r.mode = Background::Mode::Inactive;
    bg4.r.mode = Background::Mode::Inactive;
    memory::assign(bg1.r.priority, 3, 7);
    memory::assign(bg2.r.priority, 1, 5);
    memory::assign(obj.r.priority, 2, 4, 6, 8);
    break;

  case 6:
    bg1.r.mode = Background::Mode::BPP4;
    bg2.r.mode = Background::Mode::Inactive;
    bg3.r.mode = Background::Mode::Inactive;
    bg4.r.mode = Background::Mode::Inactive;
    memory::assign(bg1.r.priority, 2, 5);
    memory::assign(obj.r.priority, 1, 3, 4, 6);
    break;

  case 7:
    if(!r.extbg) {
      bg1.r.mode = Background::Mode::Mode7;
      bg2.r.mode = Background::Mode::Inactive;
      bg3.r.mode = Background::Mode::Inactive;
      bg4.r.mode = Background::Mode::Inactive;
      memory::assign(bg1.r.priority, 2);
      memory::assign(obj.r.priority, 1, 3, 4, 5);
    } else {
      bg1.r.mode = Background::Mode::Mode7;
      bg2.r.mode = Background::Mode::Mode7;
      bg3.r.mode = Background::Mode::Inactive;
      bg4.r.mode = Background::Mode::Inactive;
      memory::assign(bg1.r.priority, 3);
      memory::assign(bg2.r.priority, 1, 5);
      memory::assign(obj.r.priority, 2, 4, 6, 7);
    }
    break;
  }
}
