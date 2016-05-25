auto PPU::read(uint24 addr, uint8 data) -> uint8 {
  cpu.synchronizePPU();

  switch(addr & 0xffff) {

  case 0x2104: case 0x2105: case 0x2106: case 0x2108:
  case 0x2109: case 0x210a: case 0x2114: case 0x2115:
  case 0x2116: case 0x2118: case 0x2119: case 0x211a:
  case 0x2124: case 0x2125: case 0x2126: case 0x2128:
  case 0x2129: case 0x212a: {
    return regs.ppu1_mdr;
  }

  //MPYL
  case 0x2134: {
    uint result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
    regs.ppu1_mdr = (result >>  0);
    return regs.ppu1_mdr;
  }

  //MPYM
  case 0x2135: {
    uint result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
    regs.ppu1_mdr = (result >>  8);
    return regs.ppu1_mdr;
  }

  //MPYH
  case 0x2136: {
    uint result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
    regs.ppu1_mdr = (result >> 16);
    return regs.ppu1_mdr;
  }

  //SLHV
  case 0x2137: {
    if(cpu.pio() & 0x80) latchCounters();
    return cpu.r.mdr;
  }

  //OAMDATAREAD
  case 0x2138: {
    uint10 addr = regs.oam_addr++;
    if(!regs.display_disable && vcounter() < vdisp()) addr = regs.oam_iaddr;
    if(addr & 0x0200) addr &= 0x021f;

    regs.ppu1_mdr = oamRead(addr);
    sprite.set_first_sprite();
    return regs.ppu1_mdr;
  }

  //VMDATALREAD
  case 0x2139: {
    uint16 addr = getVramAddress() + 0;
    regs.ppu1_mdr = regs.vram_readbuffer >> 0;
    if(regs.vram_incmode == 0) {
      addr &= ~1;
      regs.vram_readbuffer  = vramRead(addr + 0) << 0;
      regs.vram_readbuffer |= vramRead(addr + 1) << 8;
      regs.vram_addr += regs.vram_incsize;
    }
    return regs.ppu1_mdr;
  }

  //VMDATAHREAD
  case 0x213a: {
    uint16 addr = getVramAddress() + 1;
    regs.ppu1_mdr = regs.vram_readbuffer >> 8;
    if(regs.vram_incmode == 1) {
      addr &= ~1;
      regs.vram_readbuffer  = vramRead(addr + 0) << 0;
      regs.vram_readbuffer |= vramRead(addr + 1) << 8;
      regs.vram_addr += regs.vram_incsize;
    }
    return regs.ppu1_mdr;
  }

  //CGDATAREAD
  case 0x213b: {
    bool latch = regs.cgram_addr & 1;
    uint9 addr = regs.cgram_addr++;
    if(!regs.display_disable
    && vcounter() > 0 && vcounter() < vdisp()
    && hcounter() >= 88 && hcounter() < 1096
    ) addr = regs.cgram_iaddr;

    if(latch == 0) {
      regs.ppu2_mdr  = cgramRead(addr);
    } else {
      regs.ppu2_mdr &= 0x80;
      regs.ppu2_mdr |= cgramRead(addr);
    }
    return regs.ppu2_mdr;
  }

  //OPHCT
  case 0x213c: {
    if(regs.latch_hcounter == 0) {
      regs.ppu2_mdr  = (regs.hcounter >> 0);
    } else {
      regs.ppu2_mdr &= 0xfe;
      regs.ppu2_mdr |= (regs.hcounter >> 8) & 1;
    }
    regs.latch_hcounter ^= 1;
    return regs.ppu2_mdr;
  }

  //OPVCT
  case 0x213d: {
    if(regs.latch_vcounter == 0) {
      regs.ppu2_mdr  = (regs.vcounter >> 0);
    } else {
      regs.ppu2_mdr &= 0xfe;
      regs.ppu2_mdr |= (regs.vcounter >> 8) & 1;
    }
    regs.latch_vcounter ^= 1;
    return regs.ppu2_mdr;
  }

  //STAT77
  case 0x213e: {
    regs.ppu1_mdr &= 0x10;
    regs.ppu1_mdr |= sprite.regs.time_over << 7;
    regs.ppu1_mdr |= sprite.regs.range_over << 6;
    regs.ppu1_mdr |= ppu1_version & 0x0f;
    return regs.ppu1_mdr;
  }

  //STAT78
  case 0x213f: {
    regs.latch_hcounter = 0;
    regs.latch_vcounter = 0;

    regs.ppu2_mdr &= 0x20;
    regs.ppu2_mdr |= field() << 7;
    if((cpu.pio() & 0x80) == 0) {
      regs.ppu2_mdr |= 0x40;
    } else if(regs.counters_latched) {
      regs.ppu2_mdr |= 0x40;
      regs.counters_latched = false;
    }
    regs.ppu2_mdr |= (system.region() == System::Region::NTSC ? 0 : 1) << 4;
    regs.ppu2_mdr |= ppu2_version & 0x0f;
    return regs.ppu2_mdr;
  }

  }

  return data;
}

auto PPU::write(uint24 addr, uint8 data) -> void {
  cpu.synchronizePPU();

  switch(addr & 0xffff) {

  //INIDISP
  case 0x2100: {
    if(regs.display_disable && vcounter() == vdisp()) sprite.address_reset();
    regs.display_disable = data & 0x80;
    regs.display_brightness = data & 0x0f;
    return;
  }

  //OBSEL
  case 0x2101: {
    sprite.regs.base_size = (data >> 5) & 7;
    sprite.regs.nameselect = (data >> 3) & 3;
    sprite.regs.tiledata_addr = (data & 3) << 14;
    return;
  }

  //OAMADDL
  case 0x2102: {
    regs.oam_baseaddr = (regs.oam_baseaddr & 0x0200) | (data << 1);
    sprite.address_reset();
    return;
  }

  //OAMADDH
  case 0x2103: {
    regs.oam_priority = data & 0x80;
    regs.oam_baseaddr = ((data & 0x01) << 9) | (regs.oam_baseaddr & 0x01fe);
    sprite.address_reset();
    return;
  }

  //OAMDATA
  case 0x2104: {
    bool latch = regs.oam_addr & 1;
    uint10 addr = regs.oam_addr++;
    if(!regs.display_disable && vcounter() < vdisp()) addr = regs.oam_iaddr;
    if(addr & 0x0200) addr &= 0x021f;

    if(latch == 0) regs.oam_latchdata = data;
    if(addr & 0x0200) {
      oamWrite(addr, data);
    } else if(latch == 1) {
      oamWrite((addr & ~1) + 0, regs.oam_latchdata);
      oamWrite((addr & ~1) + 1, data);
    }
    sprite.set_first_sprite();
    return;
  }

  //BGMODE
  case 0x2105: {
    bg4.regs.tile_size = (data & 0x80);
    bg3.regs.tile_size = (data & 0x40);
    bg2.regs.tile_size = (data & 0x20);
    bg1.regs.tile_size = (data & 0x10);
    regs.bg3_priority = (data & 0x08);
    regs.bgmode = (data & 0x07);
    updateVideoMode();
    return;
  }

  //MOSAIC
  case 0x2106: {
    unsigned mosaic_size = (data >> 4) & 15;
    bg4.regs.mosaic = (data & 0x08 ? mosaic_size : 0);
    bg3.regs.mosaic = (data & 0x04 ? mosaic_size : 0);
    bg2.regs.mosaic = (data & 0x02 ? mosaic_size : 0);
    bg1.regs.mosaic = (data & 0x01 ? mosaic_size : 0);
    return;
  }

  //BG1SC
  case 0x2107: {
    bg1.regs.screen_addr = (data & 0x7c) << 9;
    bg1.regs.screen_size = data & 3;
    return;
  }

  //BG2SC
  case 0x2108: {
    bg2.regs.screen_addr = (data & 0x7c) << 9;
    bg2.regs.screen_size = data & 3;
    return;
  }

  //BG3SC
  case 0x2109: {
    bg3.regs.screen_addr = (data & 0x7c) << 9;
    bg3.regs.screen_size = data & 3;
    return;
  }

  //BG4SC
  case 0x210a: {
    bg4.regs.screen_addr = (data & 0x7c) << 9;
    bg4.regs.screen_size = data & 3;
    return;
  }

  //BG12NBA
  case 0x210b: {
    bg1.regs.tiledata_addr = (data & 0x07) << 13;
    bg2.regs.tiledata_addr = (data & 0x70) <<  9;
    return;
  }

  //BG34NBA
  case 0x210c: {
    bg3.regs.tiledata_addr = (data & 0x07) << 13;
    bg4.regs.tiledata_addr = (data & 0x70) <<  9;
    return;
  }

  //BG1HOFS
  case 0x210d: {
    regs.mode7_hoffset = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;

    bg1.regs.hoffset = (data << 8) | (regs.bgofs_latchdata & ~7) | ((bg1.regs.hoffset >> 8) & 7);
    regs.bgofs_latchdata = data;
    return;
  }

  //BG1VOFS
  case 0x210e: {
    regs.mode7_voffset = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;

    bg1.regs.voffset = (data << 8) | regs.bgofs_latchdata;
    regs.bgofs_latchdata = data;
    return;
  }

  //BG2HOFS
  case 0x210f: {
    bg2.regs.hoffset = (data << 8) | (regs.bgofs_latchdata & ~7) | ((bg2.regs.hoffset >> 8) & 7);
    regs.bgofs_latchdata = data;
    return;
  }

  //BG2VOFS
  case 0x2110: {
    bg2.regs.voffset = (data << 8) | regs.bgofs_latchdata;
    regs.bgofs_latchdata = data;
    return;
  }

  //BG3HOFS
  case 0x2111: {
    bg3.regs.hoffset = (data << 8) | (regs.bgofs_latchdata & ~7) | ((bg3.regs.hoffset >> 8) & 7);
    regs.bgofs_latchdata = data;
    return;
  }

  //BG3VOFS
  case 0x2112: {
    bg3.regs.voffset = (data << 8) | regs.bgofs_latchdata;
    regs.bgofs_latchdata = data;
    return;
  }

  //BG4HOFS
  case 0x2113: {
    bg4.regs.hoffset = (data << 8) | (regs.bgofs_latchdata & ~7) | ((bg4.regs.hoffset >> 8) & 7);
    regs.bgofs_latchdata = data;
    return;
  }

  //BG4VOFS
  case 0x2114: {
    bg4.regs.voffset = (data << 8) | regs.bgofs_latchdata;
    regs.bgofs_latchdata = data;
    return;
  }

  //VMAIN
  case 0x2115: {
    regs.vram_incmode = data & 0x80;
    regs.vram_mapping = (data >> 2) & 3;
    switch(data & 3) {
    case 0: regs.vram_incsize =   1; break;
    case 1: regs.vram_incsize =  32; break;
    case 2: regs.vram_incsize = 128; break;
    case 3: regs.vram_incsize = 128; break;
    }
    return;
  }

  //VMADDL
  case 0x2116: {
    regs.vram_addr &= 0xff00;
    regs.vram_addr |= (data << 0);
    uint16 addr = getVramAddress();
    regs.vram_readbuffer  = vramRead(addr + 0) << 0;
    regs.vram_readbuffer |= vramRead(addr + 1) << 8;
    return;
  }

  //VMADDH
  case 0x2117: {
    regs.vram_addr &= 0x00ff;
    regs.vram_addr |= (data << 8);
    uint16 addr = getVramAddress();
    regs.vram_readbuffer  = vramRead(addr + 0) << 0;
    regs.vram_readbuffer |= vramRead(addr + 1) << 8;
    return;
  }

  //VMDATAL
  case 0x2118: {
    uint16 addr = getVramAddress() + 0;
    vramWrite(addr, data);
    if(regs.vram_incmode == 0) regs.vram_addr += regs.vram_incsize;
    return;
  }

  //VMDATAH
  case 0x2119: {
    uint16 addr = getVramAddress() + 1;
    vramWrite(addr, data);
    if(regs.vram_incmode == 1) regs.vram_addr += regs.vram_incsize;
    return;
  }

  //M7SEL
  case 0x211a: {
    regs.mode7_repeat = (data >> 6) & 3;
    regs.mode7_vflip = data & 0x02;
    regs.mode7_hflip = data & 0x01;
    return;
  }

  //M7A
  case 0x211b: {
    regs.m7a = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;
    return;
  }

  //M7B
  case 0x211c: {
    regs.m7b = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;
    return;
  }

  //M7C
  case 0x211d: {
    regs.m7c = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;
    return;
  }

  //M7D
  case 0x211e: {
    regs.m7d = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;
    return;
  }

  //M7X
  case 0x211f: {
    regs.m7x = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;
    return;
  }

  //M7Y
  case 0x2120: {
    regs.m7y = (data << 8) | regs.mode7_latchdata;
    regs.mode7_latchdata = data;
    return;
  }

  //CGADD
  case 0x2121: {
    regs.cgram_addr = data << 1;
    return;
  }

  //CGDATA
  case 0x2122: {
    bool latch = regs.cgram_addr & 1;
    uint9 addr = regs.cgram_addr++;
    if(!regs.display_disable
    && vcounter() > 0 && vcounter() < vdisp()
    && hcounter() >= 88 && hcounter() < 1096
    ) addr = regs.cgram_iaddr;

    if(latch == 0) {
      regs.cgram_latchdata = data;
    } else {
      cgramWrite((addr & ~1) + 0, regs.cgram_latchdata);
      cgramWrite((addr & ~1) + 1, data & 0x7f);
    }
    return;
  }

  //W12SEL
  case 0x2123: {
    window.regs.bg2_two_enable = data & 0x80;
    window.regs.bg2_two_invert = data & 0x40;
    window.regs.bg2_one_enable = data & 0x20;
    window.regs.bg2_one_invert = data & 0x10;
    window.regs.bg1_two_enable = data & 0x08;
    window.regs.bg1_two_invert = data & 0x04;
    window.regs.bg1_one_enable = data & 0x02;
    window.regs.bg1_one_invert = data & 0x01;
    return;
  }

  //W34SEL
  case 0x2124: {
    window.regs.bg4_two_enable = data & 0x80;
    window.regs.bg4_two_invert = data & 0x40;
    window.regs.bg4_one_enable = data & 0x20;
    window.regs.bg4_one_invert = data & 0x10;
    window.regs.bg3_two_enable = data & 0x08;
    window.regs.bg3_two_invert = data & 0x04;
    window.regs.bg3_one_enable = data & 0x02;
    window.regs.bg3_one_invert = data & 0x01;
    return;
  }

  //WOBJSEL
  case 0x2125: {
    window.regs.col_two_enable = data & 0x80;
    window.regs.col_two_invert = data & 0x40;
    window.regs.col_one_enable = data & 0x20;
    window.regs.col_one_invert = data & 0x10;
    window.regs.oam_two_enable = data & 0x08;
    window.regs.oam_two_invert = data & 0x04;
    window.regs.oam_one_enable = data & 0x02;
    window.regs.oam_one_invert = data & 0x01;
    return;
  }

  //WH0
  case 0x2126: {
    window.regs.one_left = data;
    return;
  }

  //WH1
  case 0x2127: {
    window.regs.one_right = data;
    return;
  }

  //WH2
  case 0x2128: {
    window.regs.two_left = data;
    return;
  }

  //WH3
  case 0x2129: {
    window.regs.two_right = data;
    return;
  }

  //WBGLOG
  case 0x212a: {
    window.regs.bg4_mask = (data >> 6) & 3;
    window.regs.bg3_mask = (data >> 4) & 3;
    window.regs.bg2_mask = (data >> 2) & 3;
    window.regs.bg1_mask = (data >> 0) & 3;
    return;
  }

  //WOBJLOG
  case 0x212b: {
    window.regs.col_mask = (data >> 2) & 3;
    window.regs.oam_mask = (data >> 0) & 3;
    return;
  }

  //TM
  case 0x212c: {
    sprite.regs.main_enable = data & 0x10;
    bg4.regs.main_enable = data & 0x08;
    bg3.regs.main_enable = data & 0x04;
    bg2.regs.main_enable = data & 0x02;
    bg1.regs.main_enable = data & 0x01;
    return;
  }

  //TS
  case 0x212d: {
    sprite.regs.sub_enable = data & 0x10;
    bg4.regs.sub_enable = data & 0x08;
    bg3.regs.sub_enable = data & 0x04;
    bg2.regs.sub_enable = data & 0x02;
    bg1.regs.sub_enable = data & 0x01;
    return;
  }

  //TMW
  case 0x212e: {
    window.regs.oam_main_enable = data & 0x10;
    window.regs.bg4_main_enable = data & 0x08;
    window.regs.bg3_main_enable = data & 0x04;
    window.regs.bg2_main_enable = data & 0x02;
    window.regs.bg1_main_enable = data & 0x01;
    return;
  }

  //TSW
  case 0x212f: {
    window.regs.oam_sub_enable = data & 0x10;
    window.regs.bg4_sub_enable = data & 0x08;
    window.regs.bg3_sub_enable = data & 0x04;
    window.regs.bg2_sub_enable = data & 0x02;
    window.regs.bg1_sub_enable = data & 0x01;
    return;
  }

  //CGWSEL
  case 0x2130: {
    window.regs.col_main_mask = (data >> 6) & 3;
    window.regs.col_sub_mask = (data >> 4) & 3;
    screen.regs.addsub_mode = data & 0x02;
    screen.regs.direct_color = data & 0x01;
    return;
  }

  //CGADDSUB
  case 0x2131: {
    screen.regs.color_mode = data & 0x80;
    screen.regs.color_halve = data & 0x40;
    screen.regs.back_color_enable = data & 0x20;
    screen.regs.oam_color_enable = data & 0x10;
    screen.regs.bg4_color_enable = data & 0x08;
    screen.regs.bg3_color_enable = data & 0x04;
    screen.regs.bg2_color_enable = data & 0x02;
    screen.regs.bg1_color_enable = data & 0x01;
    return;
  }

  //COLDATA
  case 0x2132: {
    if(data & 0x80) screen.regs.color_b = data & 0x1f;
    if(data & 0x40) screen.regs.color_g = data & 0x1f;
    if(data & 0x20) screen.regs.color_r = data & 0x1f;
    return;
  }

  //SETINI
  case 0x2133: {
    regs.mode7_extbg = data & 0x40;
    regs.pseudo_hires = data & 0x08;
    regs.overscan = data & 0x04;
    sprite.regs.interlace = data & 0x02;
    regs.interlace = data & 0x01;
    updateVideoMode();
    return;
  }

  }
}

auto PPU::latchCounters() -> void {
  cpu.synchronizePPU();
  regs.hcounter = hdot();
  regs.vcounter = vcounter();
  regs.counters_latched = true;
}

auto PPU::updateVideoMode() -> void {
  switch(regs.bgmode) {
  case 0:
    bg1.regs.mode = Background::Mode::BPP2; bg1.regs.priority0 = 8; bg1.regs.priority1 = 11;
    bg2.regs.mode = Background::Mode::BPP2; bg2.regs.priority0 = 7; bg2.regs.priority1 = 10;
    bg3.regs.mode = Background::Mode::BPP2; bg3.regs.priority0 = 2; bg3.regs.priority1 =  5;
    bg4.regs.mode = Background::Mode::BPP2; bg4.regs.priority0 = 1; bg4.regs.priority1 =  4;
    sprite.regs.priority0 = 3; sprite.regs.priority1 = 6; sprite.regs.priority2 = 9; sprite.regs.priority3 = 12;
    break;

  case 1:
    bg1.regs.mode = Background::Mode::BPP4;
    bg2.regs.mode = Background::Mode::BPP4;
    bg3.regs.mode = Background::Mode::BPP2;
    bg4.regs.mode = Background::Mode::Inactive;
    if(regs.bg3_priority) {
      bg1.regs.priority0 = 5; bg1.regs.priority1 =  8;
      bg2.regs.priority0 = 4; bg2.regs.priority1 =  7;
      bg3.regs.priority0 = 1; bg3.regs.priority1 = 10;
      sprite.regs.priority0 = 2; sprite.regs.priority1 = 3; sprite.regs.priority2 = 6; sprite.regs.priority3 = 9;
    } else {
      bg1.regs.priority0 = 6; bg1.regs.priority1 =  9;
      bg2.regs.priority0 = 5; bg2.regs.priority1 =  8;
      bg3.regs.priority0 = 1; bg3.regs.priority1 =  3;
      sprite.regs.priority0 = 2; sprite.regs.priority1 = 4; sprite.regs.priority2 = 7; sprite.regs.priority3 = 10;
    }
    break;

  case 2:
    bg1.regs.mode = Background::Mode::BPP4;
    bg2.regs.mode = Background::Mode::BPP4;
    bg3.regs.mode = Background::Mode::Inactive;
    bg4.regs.mode = Background::Mode::Inactive;
    bg1.regs.priority0 = 3; bg1.regs.priority1 = 7;
    bg2.regs.priority0 = 1; bg2.regs.priority1 = 5;
    sprite.regs.priority0 = 2; sprite.regs.priority1 = 4; sprite.regs.priority2 = 6; sprite.regs.priority3 = 8;
    break;

  case 3:
    bg1.regs.mode = Background::Mode::BPP8;
    bg2.regs.mode = Background::Mode::BPP4;
    bg3.regs.mode = Background::Mode::Inactive;
    bg4.regs.mode = Background::Mode::Inactive;
    bg1.regs.priority0 = 3; bg1.regs.priority1 = 7;
    bg2.regs.priority0 = 1; bg2.regs.priority1 = 5;
    sprite.regs.priority0 = 2; sprite.regs.priority1 = 4; sprite.regs.priority2 = 6; sprite.regs.priority3 = 8;
    break;

  case 4:
    bg1.regs.mode = Background::Mode::BPP8;
    bg2.regs.mode = Background::Mode::BPP2;
    bg3.regs.mode = Background::Mode::Inactive;
    bg4.regs.mode = Background::Mode::Inactive;
    bg1.regs.priority0 = 3; bg1.regs.priority1 = 7;
    bg2.regs.priority0 = 1; bg2.regs.priority1 = 5;
    sprite.regs.priority0 = 2; sprite.regs.priority1 = 4; sprite.regs.priority2 = 6; sprite.regs.priority3 = 8;
    break;

  case 5:
    bg1.regs.mode = Background::Mode::BPP4;
    bg2.regs.mode = Background::Mode::BPP2;
    bg3.regs.mode = Background::Mode::Inactive;
    bg4.regs.mode = Background::Mode::Inactive;
    bg1.regs.priority0 = 3; bg1.regs.priority1 = 7;
    bg2.regs.priority0 = 1; bg2.regs.priority1 = 5;
    sprite.regs.priority0 = 2; sprite.regs.priority1 = 4; sprite.regs.priority2 = 6; sprite.regs.priority3 = 8;
    break;

  case 6:
    bg1.regs.mode = Background::Mode::BPP4;
    bg2.regs.mode = Background::Mode::Inactive;
    bg3.regs.mode = Background::Mode::Inactive;
    bg4.regs.mode = Background::Mode::Inactive;
    bg1.regs.priority0 = 2; bg1.regs.priority1 = 5;
    sprite.regs.priority0 = 1; sprite.regs.priority1 = 3; sprite.regs.priority2 = 4; sprite.regs.priority3 = 6;
    break;

  case 7:
    if(regs.mode7_extbg == false) {
      bg1.regs.mode = Background::Mode::Mode7;
      bg2.regs.mode = Background::Mode::Inactive;
      bg3.regs.mode = Background::Mode::Inactive;
      bg4.regs.mode = Background::Mode::Inactive;
      bg1.regs.priority0 = 2; bg1.regs.priority1 = 2;
      sprite.regs.priority0 = 1; sprite.regs.priority1 = 3; sprite.regs.priority2 = 4; sprite.regs.priority3 = 5;
    } else {
      bg1.regs.mode = Background::Mode::Mode7;
      bg2.regs.mode = Background::Mode::Mode7;
      bg3.regs.mode = Background::Mode::Inactive;
      bg4.regs.mode = Background::Mode::Inactive;
      bg1.regs.priority0 = 3; bg1.regs.priority1 = 3;
      bg2.regs.priority0 = 1; bg2.regs.priority1 = 5;
      sprite.regs.priority0 = 2; sprite.regs.priority1 = 4; sprite.regs.priority2 = 6; sprite.regs.priority3 = 7;
    }
    break;
  }
}
