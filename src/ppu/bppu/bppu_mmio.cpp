void bPPU::latch_counters() {
  regs.hcounter = clock->hcounter();
  regs.vcounter = clock->vcounter();
  regs.counters_latched = true;
}

uint16 bPPU::get_vram_address() {
uint16 addr;
  addr = regs.vram_addr << 1;
  switch(regs.vram_mapping) {
  case 0:break;
  case 1:addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | ((addr >> 5) & 7);break;
  case 2:addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | ((addr >> 6) & 7);break;
  case 3:addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | ((addr >> 7) & 7);break;
  }
  return addr;
}

//INIDISP
void bPPU::mmio_w2100(uint8 value) {
  regs.display_disabled   = !!(value & 0x80);
  regs.display_brightness = value & 15;
}

//OBSEL
void bPPU::mmio_w2101(uint8 value) {
  regs.oam_basesize   = (value >> 5) & 7;
  regs.oam_nameselect = (value >> 3) & 3;
  regs.oam_tdaddr     = (value & 3) << 14;
}

//OAMADDL
void bPPU::mmio_w2102(uint8 value) {
  regs.oam_addrl = value;
  regs.oam_addr  = ((regs.oam_addrh << 8) | regs.oam_addrl) << 1;
}

//OAMADDH
void bPPU::mmio_w2103(uint8 value) {
  regs.oam_addrh = value & 1;
  regs.oam_addr  = ((regs.oam_addrh << 8) | regs.oam_addrl) << 1;
}

//OAMDATA
void bPPU::mmio_w2104(uint8 value) {
  if(regs.oam_addr >= 0x0200) {
    if(!(regs.oam_addr & 1)) {
      regs.oam_latchdata = value;
    }
    oam_write(regs.oam_addr, value);
  } else if(!(regs.oam_addr & 1)) {
    regs.oam_latchdata = value;
  } else {
    oam_write((regs.oam_addr & 0x03fe),     regs.oam_latchdata);
    oam_write((regs.oam_addr & 0x03fe) + 1, value);
  }
  regs.oam_addr++;
  regs.oam_addr &= 0x03ff;
}

//BGMODE
void bPPU::mmio_w2105(uint8 value) {
  regs.bg_tilesize[BG4] = !!(value & 0x80);
  regs.bg_tilesize[BG3] = !!(value & 0x40);
  regs.bg_tilesize[BG2] = !!(value & 0x20);
  regs.bg_tilesize[BG1] = !!(value & 0x10);
  regs.bg3_priority     = !!(value & 0x08);
  regs.bg_mode          = (value & 7);
}

//MOSAIC
void bPPU::mmio_w2106(uint8 value) {
  regs.mosaic_size         = (value >> 4) & 15;
  regs.mosaic_enabled[BG4] = !!(value & 0x08);
  regs.mosaic_enabled[BG3] = !!(value & 0x04);
  regs.mosaic_enabled[BG2] = !!(value & 0x02);
  regs.mosaic_enabled[BG1] = !!(value & 0x01);
}

//BG1SC
void bPPU::mmio_w2107(uint8 value) {
  regs.bg_scaddr[BG1] = (value & 0x7c) << 9;
  regs.bg_scsize[BG1] = value & 3;
}

//BG2SC
void bPPU::mmio_w2108(uint8 value) {
  regs.bg_scaddr[BG2] = (value & 0x7c) << 9;
  regs.bg_scsize[BG2] = value & 3;
}

//BG3SC
void bPPU::mmio_w2109(uint8 value) {
  regs.bg_scaddr[BG3] = (value & 0x7c) << 9;
  regs.bg_scsize[BG3] = value & 3;
}

//BG4SC
void bPPU::mmio_w210a(uint8 value) {
  regs.bg_scaddr[BG4] = (value & 0x7c) << 9;
  regs.bg_scsize[BG4] = value & 3;
}

//BG12NBA
void bPPU::mmio_w210b(uint8 value) {
  regs.bg_tdaddr[BG1] = (value & 0x07) << 13;
  regs.bg_tdaddr[BG2] = (value & 0x70) <<  9;
}

//BG34NBA
void bPPU::mmio_w210c(uint8 value) {
  regs.bg_tdaddr[BG3] = (value & 0x07) << 13;
  regs.bg_tdaddr[BG4] = (value & 0x70) <<  9;
}

//BG1HOFS
void bPPU::mmio_w210d(uint8 value) {
  regs.bg_hofs[BG1] = (value << 8) | (regs.bg_hofs[BG1] >> 8);
}

//BG1VOFS
void bPPU::mmio_w210e(uint8 value) {
  regs.bg_vofs[BG1] = (value << 8) | (regs.bg_vofs[BG1] >> 8);
}

//BG2HOFS
void bPPU::mmio_w210f(uint8 value) {
  regs.bg_hofs[BG2] = (value << 8) | (regs.bg_hofs[BG2] >> 8);
}

//BG2VOFS
void bPPU::mmio_w2110(uint8 value) {
  regs.bg_vofs[BG2] = (value << 8) | (regs.bg_vofs[BG2] >> 8);
}

//BG3HOFS
void bPPU::mmio_w2111(uint8 value) {
  regs.bg_hofs[BG3] = (value << 8) | (regs.bg_hofs[BG3] >> 8);
}

//BG3VOFS
void bPPU::mmio_w2112(uint8 value) {
  regs.bg_vofs[BG3] = (value << 8) | (regs.bg_vofs[BG3] >> 8);
}

//BG4HOFS
void bPPU::mmio_w2113(uint8 value) {
  regs.bg_hofs[BG4] = (value << 8) | (regs.bg_hofs[BG4] >> 8);
}

//BG4VOFS
void bPPU::mmio_w2114(uint8 value) {
  regs.bg_vofs[BG4] = (value << 8) | (regs.bg_vofs[BG4] >> 8);
}

//VMAIN
void bPPU::mmio_w2115(uint8 value) {
  regs.vram_incmode = !!(value & 0x80);
  regs.vram_mapping = (value >> 2) & 3;
  switch(value & 3) {
  case 0:regs.vram_incsize =   1;break;
  case 1:regs.vram_incsize =  32;break;
  case 2:regs.vram_incsize = 128;break;
  case 3:regs.vram_incsize = 128;break;
  }
}

//VMADDL
void bPPU::mmio_w2116(uint8 value) {
  regs.vram_addr = (regs.vram_addr & 0xff00) | value;
}

//VMADDH
void bPPU::mmio_w2117(uint8 value) {
  regs.vram_addr = (value << 8) | (regs.vram_addr & 0x00ff);
}

//VMDATAL
void bPPU::mmio_w2118(uint8 value) {
uint16 addr = get_vram_address();
  vram_write(addr, value);
  if(regs.vram_incmode == 0) {
    regs.vram_addr += regs.vram_incsize;
  }
  bg_tiledata_state[TILE_2BIT][(addr >> 4)] = 1;
  bg_tiledata_state[TILE_4BIT][(addr >> 5)] = 1;
  bg_tiledata_state[TILE_8BIT][(addr >> 6)] = 1;
}

//VMDATAH
void bPPU::mmio_w2119(uint8 value) {
uint16 addr = get_vram_address() + 1;
  vram_write(addr, value);
  if(regs.vram_incmode == 1) {
    regs.vram_addr += regs.vram_incsize;
  }
  bg_tiledata_state[TILE_2BIT][(addr >> 4)] = 1;
  bg_tiledata_state[TILE_4BIT][(addr >> 5)] = 1;
  bg_tiledata_state[TILE_8BIT][(addr >> 6)] = 1;
}

//M7SEL
void bPPU::mmio_w211a(uint8 value) {
  regs.mode7_repeat = (value >> 6) & 3;
  regs.mode7_vflip = !!(value & 0x02);
  regs.mode7_hflip = !!(value & 0x01);
}

//M7A
void bPPU::mmio_w211b(uint8 value) {
  regs.m7a = (value << 8) | (regs.m7a >> 8);
}

//M7B
void bPPU::mmio_w211c(uint8 value) {
  regs.m7b = (value << 8) | (regs.m7b >> 8);
}

//M7C
void bPPU::mmio_w211d(uint8 value) {
  regs.m7c = (value << 8) | (regs.m7c >> 8);
}

//M7D
void bPPU::mmio_w211e(uint8 value) {
  regs.m7d = (value << 8) | (regs.m7d >> 8);
}

//M7X
void bPPU::mmio_w211f(uint8 value) {
  regs.m7x = (value << 8) | (regs.m7x >> 8);
}

//M7Y
void bPPU::mmio_w2120(uint8 value) {
  regs.m7y = (value << 8) | (regs.m7y >> 8);
}

//CGADD
void bPPU::mmio_w2121(uint8 value) {
  regs.cgram_addr = value << 1;
}

//CGDATA
void bPPU::mmio_w2122(uint8 value) {
  cgram_write(regs.cgram_addr, value);
  regs.cgram_addr++;
  regs.cgram_addr &= 0x01ff;
}

//W12SEL
void bPPU::mmio_w2123(uint8 value) {
  regs.bg_window2_enabled[BG2] = !!(value & 0x80);
  regs.bg_window2_invert [BG2] = !!(value & 0x40);
  regs.bg_window1_enabled[BG2] = !!(value & 0x20);
  regs.bg_window1_invert [BG2] = !!(value & 0x10);
  regs.bg_window2_enabled[BG1] = !!(value & 0x08);
  regs.bg_window2_invert [BG1] = !!(value & 0x04);
  regs.bg_window1_enabled[BG1] = !!(value & 0x02);
  regs.bg_window1_invert [BG1] = !!(value & 0x01);
}

//W34SEL
void bPPU::mmio_w2124(uint8 value) {
  regs.bg_window2_enabled[BG4] = !!(value & 0x80);
  regs.bg_window2_invert [BG4] = !!(value & 0x40);
  regs.bg_window1_enabled[BG4] = !!(value & 0x20);
  regs.bg_window1_invert [BG4] = !!(value & 0x10);
  regs.bg_window2_enabled[BG3] = !!(value & 0x08);
  regs.bg_window2_invert [BG3] = !!(value & 0x04);
  regs.bg_window1_enabled[BG3] = !!(value & 0x02);
  regs.bg_window1_invert [BG3] = !!(value & 0x01);
}

//WOBJSEL
void bPPU::mmio_w2125(uint8 value) {
  regs.color_window2_enabled   = !!(value & 0x80);
  regs.color_window2_invert    = !!(value & 0x40);
  regs.color_window1_enabled   = !!(value & 0x20);
  regs.color_window1_invert    = !!(value & 0x10);
  regs.bg_window2_enabled[OAM] = !!(value & 0x08);
  regs.bg_window2_invert [OAM] = !!(value & 0x04);
  regs.bg_window1_enabled[OAM] = !!(value & 0x02);
  regs.bg_window1_invert [OAM] = !!(value & 0x01);
}

//WH0
void bPPU::mmio_w2126(uint8 value) {
  regs.window1_left = value;
}

//WH1
void bPPU::mmio_w2127(uint8 value) {
  regs.window1_right = value;
}

//WH2
void bPPU::mmio_w2128(uint8 value) {
  regs.window2_left = value;
}

//WH3
void bPPU::mmio_w2129(uint8 value) {
  regs.window2_right = value;
}

//WBGLOG
void bPPU::mmio_w212a(uint8 value) {
  regs.bg_window_mask[BG4] = (value >> 6) & 3;
  regs.bg_window_mask[BG3] = (value >> 4) & 3;
  regs.bg_window_mask[BG2] = (value >> 2) & 3;
  regs.bg_window_mask[BG1] = (value     ) & 3;
}

//WOBJLOG
void bPPU::mmio_w212b(uint8 value) {
  regs.color_window_mask   = (value >> 2) & 3;
  regs.bg_window_mask[OAM] = (value     ) & 3;
}

//TM
void bPPU::mmio_w212c(uint8 value) {
  regs.bg_enabled[OAM] = !!(value & 0x10);
  regs.bg_enabled[BG4] = !!(value & 0x08);
  regs.bg_enabled[BG3] = !!(value & 0x04);
  regs.bg_enabled[BG2] = !!(value & 0x02);
  regs.bg_enabled[BG1] = !!(value & 0x01);
}

//TS
void bPPU::mmio_w212d(uint8 value) {
  regs.bgsub_enabled[OAM] = !!(value & 0x10);
  regs.bgsub_enabled[BG4] = !!(value & 0x08);
  regs.bgsub_enabled[BG3] = !!(value & 0x04);
  regs.bgsub_enabled[BG2] = !!(value & 0x02);
  regs.bgsub_enabled[BG1] = !!(value & 0x01);
}

//TMW
void bPPU::mmio_w212e(uint8 value) {
  regs.bg_window_enabled[OAM] = !!(value & 0x10);
  regs.bg_window_enabled[BG4] = !!(value & 0x08);
  regs.bg_window_enabled[BG3] = !!(value & 0x04);
  regs.bg_window_enabled[BG2] = !!(value & 0x02);
  regs.bg_window_enabled[BG1] = !!(value & 0x01);
}

//TSW
void bPPU::mmio_w212f(uint8 value) {
  regs.bgsub_window_enabled[OAM] = !!(value & 0x10);
  regs.bgsub_window_enabled[BG4] = !!(value & 0x08);
  regs.bgsub_window_enabled[BG3] = !!(value & 0x04);
  regs.bgsub_window_enabled[BG2] = !!(value & 0x02);
  regs.bgsub_window_enabled[BG1] = !!(value & 0x01);
}

//CGWSEL
void bPPU::mmio_w2130(uint8 value) {
  regs.color_mask    = (value >> 6) & 3;
  regs.colorsub_mask = (value >> 4) & 3;
  regs.addsub_mode   = !!(value & 0x02);
}

//CGADDSUB
void bPPU::mmio_w2131(uint8 value) {
  regs.color_mode             = !!(value & 0x80);
  regs.color_halve            = !!(value & 0x40);
  regs.bg_color_enabled[BACK] = !!(value & 0x20);
  regs.bg_color_enabled[OAM]  = !!(value & 0x10);
  regs.bg_color_enabled[BG4]  = !!(value & 0x08);
  regs.bg_color_enabled[BG3]  = !!(value & 0x04);
  regs.bg_color_enabled[BG2]  = !!(value & 0x02);
  regs.bg_color_enabled[BG1]  = !!(value & 0x01);
}

//COLDATA
void bPPU::mmio_w2132(uint8 value) {
  if(value & 0x80)regs.color_b = value & 0x1f;
  if(value & 0x40)regs.color_g = value & 0x1f;
  if(value & 0x20)regs.color_r = value & 0x1f;
}

//SETINI
void bPPU::mmio_w2133(uint8 value) {
  regs.mode7_extbg = !!(value & 0x40);
  regs.overscan    = !!(value & 0x04);
  regs.scanlines   = (value & 0x04)?239:224;
  regs.oam_halve   = !!(value & 0x02);
  regs.interlace   = !!(value & 0x01);

  clock->enable_overscan(regs.overscan);
  clock->enable_interlace(regs.interlace);
}

//MPYL
uint8 bPPU::mmio_r2134() {
uint32 r;
  r = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  return (r);
}

//MPYM
uint8 bPPU::mmio_r2135() {
uint32 r;
  r = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  return (r >> 8);
}

//MPYH
uint8 bPPU::mmio_r2136() {
uint32 r;
  r = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  return (r >> 16);
}

//SLHV
uint8 bPPU::mmio_r2137() {
  if(cpu->pio_status() & 0x80) {
    latch_counters();
  }
  return 0x00;
}

//OAMDATAREAD
uint8 bPPU::mmio_r2138() {
uint8 r;
  r = oam_read(regs.oam_addr);
  if(!(regs.oam_addr & 1)) {
    regs.oam_latchdata = r;
  }
  regs.oam_addr++;
  regs.oam_addr &= 0x03ff;
  return r;
}

//VMDATALREAD
uint8 bPPU::mmio_r2139() {
uint16 addr = get_vram_address();
uint8 r = regs.vram_readbuffer;
  if(regs.vram_incmode == 0) {
    addr &= 0xfffe;
    regs.vram_readbuffer  = vram_read(addr);
    regs.vram_readbuffer |= vram_read(addr + 1) << 8;
    regs.vram_addr += regs.vram_incsize;
  }
  return r;
}

//VMDATAHREAD
uint8 bPPU::mmio_r213a() {
uint16 addr = get_vram_address() + 1;
uint8 r = regs.vram_readbuffer >> 8;
  if(regs.vram_incmode == 1) {
    addr &= 0xfffe;
    regs.vram_readbuffer  = vram_read(addr);
    regs.vram_readbuffer |= vram_read(addr + 1) << 8;
    regs.vram_addr += regs.vram_incsize;
  }
  return r;
}

//CGDATAREAD
uint8 bPPU::mmio_r213b() {
uint8 r;
  r = cgram_read(regs.cgram_addr);
  regs.cgram_addr++;
  regs.cgram_addr &= 0x01ff;
  return r;
}

//OPHCT
uint8 bPPU::mmio_r213c() {
uint16 r = regs.hcounter;
  if(regs.latch_hcounter)r >>= 8;
  regs.latch_hcounter ^= 1;
  return r;
}

//OPVCT
uint8 bPPU::mmio_r213d() {
uint16 r = regs.vcounter;
  if(regs.latch_vcounter)r >>= 8;
  regs.latch_vcounter ^= 1;
  return r;
}

//STAT77
uint8 bPPU::mmio_r213e() {
uint8 r = 0x00;
  r |= clock->interlace_field() << 7;
  if(!(cpu->pio_status() & 0x80)) {
    r |= 1 << 6;
  } else if(regs.counters_latched == true) {
    r |= 1 << 6;
    regs.counters_latched = false;
  }
  r |= 0x01; //PPU1 version number
  return r;
}

//STAT78
uint8 bPPU::mmio_r213f() {
uint8 r = 0x00;
  regs.latch_hcounter = 0;
  regs.latch_vcounter = 0;
  r |= (1 << 5);
  r |= 0x03; //PPU2 version number
  return r;
}

uint8 bPPUMMIO::read(uint32 addr) {
  clock->sync();
  switch(addr) {
  case 0x2134:return ppu->mmio_r2134(); //MPYL
  case 0x2135:return ppu->mmio_r2135(); //MPYM
  case 0x2136:return ppu->mmio_r2136(); //MPYH
  case 0x2137:return ppu->mmio_r2137(); //SLHV
  case 0x2138:return ppu->mmio_r2138(); //OAMDATAREAD
  case 0x2139:return ppu->mmio_r2139(); //VMDATALREAD
  case 0x213a:return ppu->mmio_r213a(); //VMDATAHREAD
  case 0x213b:return ppu->mmio_r213b(); //CGDATAREAD
  case 0x213c:return ppu->mmio_r213c(); //OPHCT
  case 0x213d:return ppu->mmio_r213d(); //OPVCT
  case 0x213e:return ppu->mmio_r213e(); //STAT77
  case 0x213f:return ppu->mmio_r213f(); //STAT78

  case 0x2140:case 0x2141:case 0x2142:case 0x2143:
  static uint8 t = 0, counter = 0;
  uint8 x, port = (addr & 3);
    if(rand() & 1) {
      x = rand() & 7;
      if(x == 0) {
        if(!(port & 1))t = cpu->regs.a;
        else           t = cpu->regs.a >> 8;
      }
      else if(x == 1) {
        if(!(port & 1))t = cpu->regs.x;
        else           t = cpu->regs.x >> 8;
      }
      else if(x == 2) {
        if(!(port & 1))t = cpu->regs.y;
        else           t = cpu->regs.y >> 8;
      }
      else if(x == 3)t = 0xaa;
      else if(x == 4)t = 0xbb;
      else if(x == 5)t = 0xcc;
      else { t = counter++; }
    }
    return t;
  }
  return 0x00;
}

void bPPUMMIO::write(uint32 addr, uint8 value) {
  clock->sync();
  switch(addr) {
  case 0x2100:ppu->mmio_w2100(value);return; //INIDISP
  case 0x2101:ppu->mmio_w2101(value);return; //OBSEL
  case 0x2102:ppu->mmio_w2102(value);return; //OAMADDL
  case 0x2103:ppu->mmio_w2103(value);return; //OAMADDH
  case 0x2104:ppu->mmio_w2104(value);return; //OAMDATA
  case 0x2105:ppu->mmio_w2105(value);return; //BGMODE
  case 0x2106:ppu->mmio_w2106(value);return; //MOSAIC
  case 0x2107:ppu->mmio_w2107(value);return; //BG1SC
  case 0x2108:ppu->mmio_w2108(value);return; //BG2SC
  case 0x2109:ppu->mmio_w2109(value);return; //BG3SC
  case 0x210a:ppu->mmio_w210a(value);return; //BG4SC
  case 0x210b:ppu->mmio_w210b(value);return; //BG12NBA
  case 0x210c:ppu->mmio_w210c(value);return; //BG34NBA
  case 0x210d:ppu->mmio_w210d(value);return; //BG1HOFS
  case 0x210e:ppu->mmio_w210e(value);return; //BG1VOFS
  case 0x210f:ppu->mmio_w210f(value);return; //BG2HOFS
  case 0x2110:ppu->mmio_w2110(value);return; //BG2VOFS
  case 0x2111:ppu->mmio_w2111(value);return; //BG3HOFS
  case 0x2112:ppu->mmio_w2112(value);return; //BG3VOFS
  case 0x2113:ppu->mmio_w2113(value);return; //BG4HOFS
  case 0x2114:ppu->mmio_w2114(value);return; //BG4VOFS
  case 0x2115:ppu->mmio_w2115(value);return; //VMAIN
  case 0x2116:ppu->mmio_w2116(value);return; //VMADDL
  case 0x2117:ppu->mmio_w2117(value);return; //VMADDH
  case 0x2118:ppu->mmio_w2118(value);return; //VMDATAL
  case 0x2119:ppu->mmio_w2119(value);return; //VMDATAH
  case 0x211a:ppu->mmio_w211a(value);return; //M7SEL
  case 0x211b:ppu->mmio_w211b(value);return; //M7A
  case 0x211c:ppu->mmio_w211c(value);return; //M7B
  case 0x211d:ppu->mmio_w211d(value);return; //M7C
  case 0x211e:ppu->mmio_w211e(value);return; //M7D
  case 0x211f:ppu->mmio_w211f(value);return; //M7X
  case 0x2120:ppu->mmio_w2120(value);return; //M7Y
  case 0x2121:ppu->mmio_w2121(value);return; //CGADD
  case 0x2122:ppu->mmio_w2122(value);return; //CGDATA
  case 0x2123:ppu->mmio_w2123(value);return; //W12SEL
  case 0x2124:ppu->mmio_w2124(value);return; //W34SEL
  case 0x2125:ppu->mmio_w2125(value);return; //WOBJSEL
  case 0x2126:ppu->mmio_w2126(value);return; //WH0
  case 0x2127:ppu->mmio_w2127(value);return; //WH1
  case 0x2128:ppu->mmio_w2128(value);return; //WH2
  case 0x2129:ppu->mmio_w2129(value);return; //WH3
  case 0x212a:ppu->mmio_w212a(value);return; //WBGLOG
  case 0x212b:ppu->mmio_w212b(value);return; //WOBJLOG
  case 0x212c:ppu->mmio_w212c(value);return; //TM
  case 0x212d:ppu->mmio_w212d(value);return; //TS
  case 0x212e:ppu->mmio_w212e(value);return; //TMW
  case 0x212f:ppu->mmio_w212f(value);return; //TSW
  case 0x2130:ppu->mmio_w2130(value);return; //CGWSEL
  case 0x2131:ppu->mmio_w2131(value);return; //CGADDSUB
  case 0x2132:ppu->mmio_w2132(value);return; //COLDATA
  case 0x2133:ppu->mmio_w2133(value);return; //SETINI
  }
}
