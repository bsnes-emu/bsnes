#ifdef SPPU_CPP

void sPPU::latch_counters() {
  regs.hcounter = cpu.hdot();
  regs.vcounter = cpu.vcounter();
  regs.counters_latched = true;
}

uint16 sPPU::get_vram_address() {
  uint16 addr = regs.vram_addr;
  switch(regs.vram_mapping) {
    case 0: break;  //direct mapping
    case 1: addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | ((addr >> 5) & 7); break;
    case 2: addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | ((addr >> 6) & 7); break;
    case 3: addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | ((addr >> 7) & 7); break;
  }
  return (addr << 1);
}

uint8 sPPU::vram_read(unsigned addr) {
  return memory::vram[addr];
}

void sPPU::vram_write(unsigned addr, uint8 data) {
  memory::vram[addr] = data;
}

uint8 sPPU::oam_read(unsigned addr) {
  if(addr & 0x0200) addr &= 0x021f;
  return memory::oam[addr];
}

void sPPU::oam_write(unsigned addr, uint8 data) {
  if(addr & 0x0200) addr &= 0x021f;
  memory::oam[addr] = data;
}

uint8 sPPU::cgram_read(unsigned addr) {
  return memory::cgram[addr];
}

void sPPU::cgram_write(unsigned addr, uint8 data) {
  memory::cgram[addr] = data;
}

bool sPPU::interlace() const {
  return false;
}

bool sPPU::overscan() const {
  return false;
}

bool sPPU::hires() const {
  return true;
}

//INIDISP
void sPPU::mmio_w2100(uint8 data) {
  regs.display_disabled = data & 0x80;
  regs.display_brightness = data & 0x0f;
}

void sPPU::mmio_w2101(uint8 data) {
}

//OAMADDL
void sPPU::mmio_w2102(uint8 data) {
  regs.oam_baseaddr &= 0x0100;
  regs.oam_baseaddr |= (data << 0);
  regs.oam_addr = regs.oam_baseaddr << 1;
  regs.oam_firstsprite = (regs.oam_priority == false ? 0 : (regs.oam_addr >> 2) & 127);
}

//OAMADDH
void sPPU::mmio_w2103(uint8 data) {
  regs.oam_priority = data & 0x80;
  regs.oam_baseaddr &= 0x00ff;
  regs.oam_baseaddr |= (data & 1) << 8;
  regs.oam_addr = regs.oam_baseaddr << 1;
  regs.oam_firstsprite = (regs.oam_priority == false ? 0 : (regs.oam_addr >> 2) & 127);
}

//OAMDATA
void sPPU::mmio_w2104(uint8 data) {
  if(regs.oam_addr & 0x0200) {
    oam_write(regs.oam_addr, data);
  } else if((regs.oam_addr & 1) == 0) {
    regs.oam_latchdata = data;
  } else {
    oam_write((regs.oam_addr & ~1) + 0, regs.oam_latchdata);
    oam_write((regs.oam_addr & ~1) + 1, data);
  }

  regs.oam_addr = (regs.oam_addr + 1) & 0x03ff;
  regs.oam_firstsprite = (regs.oam_priority == false ? 0 : (regs.oam_addr >> 2) & 127);
}

void sPPU::mmio_w2105(uint8 data) {
}

void sPPU::mmio_w2106(uint8 data) {
}

void sPPU::mmio_w2107(uint8 data) {
}

void sPPU::mmio_w2108(uint8 data) {
}

void sPPU::mmio_w2109(uint8 data) {
}

void sPPU::mmio_w210a(uint8 data) {
}

void sPPU::mmio_w210b(uint8 data) {
}

void sPPU::mmio_w210c(uint8 data) {
}

void sPPU::mmio_w210d(uint8 data) {
}

void sPPU::mmio_w210e(uint8 data) {
}

void sPPU::mmio_w210f(uint8 data) {
}

void sPPU::mmio_w2110(uint8 data) {
}

void sPPU::mmio_w2111(uint8 data) {
}

void sPPU::mmio_w2112(uint8 data) {
}

void sPPU::mmio_w2113(uint8 data) {
}

void sPPU::mmio_w2114(uint8 data) {
}

//VMAIN
void sPPU::mmio_w2115(uint8 data) {
  regs.vram_incmode = data & 0x80;
  regs.vram_mapping = (data >> 2) & 3;
  switch(data & 3) {
    case 0: regs.vram_incsize =   1; break;
    case 1: regs.vram_incsize =  32; break;
    case 2: regs.vram_incsize = 128; break;
    case 3: regs.vram_incsize = 128; break;
  }
}

//VMADDL
void sPPU::mmio_w2116(uint8 data) {
  regs.vram_addr &= 0xff00;
  regs.vram_addr |= (data << 0);
  uint16 addr = get_vram_address();
  regs.vram_readbuffer  = vram_read(addr + 0) << 0;
  regs.vram_readbuffer |= vram_read(addr + 1) << 8;
}

//VMADDH
void sPPU::mmio_w2117(uint8 data) {
  regs.vram_addr &= 0x00ff;
  regs.vram_addr |= (data << 8);
  uint16 addr = get_vram_address();
  regs.vram_readbuffer  = vram_read(addr + 0) << 0;
  regs.vram_readbuffer |= vram_read(addr + 1) << 8;
}

//VMDATAL
void sPPU::mmio_w2118(uint8 data) {
  uint16 addr = get_vram_address() + 0;
  vram_write(addr, data);
  if(regs.vram_incmode == 0) regs.vram_addr += regs.vram_incsize;
}

//VMDATAH
void sPPU::mmio_w2119(uint8 data) {
  uint16 addr = get_vram_address() + 1;
  vram_write(addr, data);
  if(regs.vram_incmode == 1) regs.vram_addr += regs.vram_incsize;
}

void sPPU::mmio_w211a(uint8 data) {
}

//M7A
void sPPU::mmio_w211b(uint8 data) {
  regs.m7a = (data << 8) | regs.m7_latchdata;
  regs.m7_latchdata = data;
}

//M7B
void sPPU::mmio_w211c(uint8 data) {
  regs.m7b = (data << 8) | regs.m7_latchdata;
  regs.m7_latchdata = data;
}

//M7C
void sPPU::mmio_w211d(uint8 data) {
  regs.m7c = (data << 8) | regs.m7_latchdata;
  regs.m7_latchdata = data;
}

//M7D
void sPPU::mmio_w211e(uint8 data) {
  regs.m7d = (data << 8) | regs.m7_latchdata;
  regs.m7_latchdata = data;
}

//M7X
void sPPU::mmio_w211f(uint8 data) {
  regs.m7x = (data << 8) | regs.m7_latchdata;
  regs.m7_latchdata = data;
}

//M7Y
void sPPU::mmio_w2120(uint8 data) {
  regs.m7y = (data << 8) | regs.m7_latchdata;
  regs.m7_latchdata = data;
}

//CGADD
void sPPU::mmio_w2121(uint8 data) {
  regs.cgram_addr = data << 1;
}

//CGDATA
void sPPU::mmio_w2122(uint8 data) {
  if((regs.cgram_addr & 1) == 0) {
    regs.cgram_latchdata = data;
  } else {
    cgram_write((regs.cgram_addr & ~1) + 0, regs.cgram_latchdata);
    cgram_write((regs.cgram_addr & ~1) + 1, data & 0x7f);
  }
  regs.cgram_addr = (regs.cgram_addr + 1) & 0x01ff;
}

void sPPU::mmio_w2123(uint8 data) {
}

void sPPU::mmio_w2124(uint8 data) {
}

void sPPU::mmio_w2125(uint8 data) {
}

void sPPU::mmio_w2126(uint8 data) {
}

void sPPU::mmio_w2127(uint8 data) {
}

void sPPU::mmio_w2128(uint8 data) {
}

void sPPU::mmio_w2129(uint8 data) {
}

void sPPU::mmio_w212a(uint8 data) {
}

void sPPU::mmio_w212b(uint8 data) {
}

void sPPU::mmio_w212c(uint8 data) {
}

void sPPU::mmio_w212d(uint8 data) {
}

void sPPU::mmio_w212e(uint8 data) {
}

void sPPU::mmio_w212f(uint8 data) {
}

void sPPU::mmio_w2130(uint8 data) {
}

void sPPU::mmio_w2131(uint8 data) {
}

void sPPU::mmio_w2132(uint8 data) {
}

void sPPU::mmio_w2133(uint8 data) {
}

//MPYL
uint8 sPPU::mmio_r2134() {
  unsigned result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  regs.ppu1_mdr = (result >>  0);
  return regs.ppu1_mdr;
}

//MPYM
uint8 sPPU::mmio_r2135() {
  unsigned result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  regs.ppu1_mdr = (result >>  8);
  return regs.ppu1_mdr;
}

//MPYH
uint8 sPPU::mmio_r2136() {
  unsigned result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  regs.ppu1_mdr = (result >> 16);
  return regs.ppu1_mdr;
}

//SLHV
uint8 sPPU::mmio_r2137() {
  if(cpu.pio() & 0x80) latch_counters();
  return cpu.regs.mdr;
}

//OAMDATAREAD
uint8 sPPU::mmio_r2138() {
  regs.ppu1_mdr = oam_read(regs.oam_addr);
  regs.oam_addr = (regs.oam_addr + 1) & 0x03ff;
  regs.oam_firstsprite = (regs.oam_priority == false ? 0 : (regs.oam_addr >> 2) & 127);
  return regs.ppu1_mdr;
}

//VMDATALREAD
uint8 sPPU::mmio_r2139() {
  uint16 addr = get_vram_address() + 0;
  regs.ppu1_mdr = regs.vram_readbuffer >> 0;
  if(regs.vram_incmode == 0) {
    addr &= ~1;
    regs.vram_readbuffer  = vram_read(addr + 0) << 0;
    regs.vram_readbuffer |= vram_read(addr + 1) << 8;
    regs.vram_addr += regs.vram_incsize;
  }
  return regs.ppu1_mdr;
}

//VMDATAHREAD
uint8 sPPU::mmio_r213a() {
  uint16 addr = get_vram_address() + 1;
  regs.ppu1_mdr = regs.vram_readbuffer >> 8;
  if(regs.vram_incmode == 1) {
    addr &= ~1;
    regs.vram_readbuffer  = vram_read(addr + 0) << 0;
    regs.vram_readbuffer |= vram_read(addr + 1) << 8;
    regs.vram_addr += regs.vram_incsize;
  }
  return regs.ppu1_mdr;
}

//CGDATAREAD
uint8 sPPU::mmio_r213b() {
  if((regs.cgram_addr & 1) == 0) {
    regs.ppu2_mdr  = cgram_read(regs.cgram_addr) & 0xff;
  } else {
    regs.ppu2_mdr &= 0x80;
    regs.ppu2_mdr |= cgram_read(regs.cgram_addr) & 0x7f;
  }
  regs.cgram_addr = (regs.cgram_addr + 1) & 0x01ff;
  return regs.ppu2_mdr;
}

//OPHCT
uint8 sPPU::mmio_r213c() {
  if(regs.latch_hcounter == 0) {
    regs.ppu2_mdr  = regs.hcounter & 0xff;
  } else {
    regs.ppu2_mdr &= 0xfe;
    regs.ppu2_mdr |= (regs.hcounter >> 8) & 1;
  }
  regs.latch_hcounter ^= 1;
  return regs.ppu2_mdr;
}

//OPVCT
uint8 sPPU::mmio_r213d() {
  if(regs.latch_vcounter == 0) {
    regs.ppu2_mdr  = regs.vcounter & 0xff;
  } else {
    regs.ppu2_mdr &= 0xfe;
    regs.ppu2_mdr |= (regs.vcounter >> 8) & 1;
  }
  regs.latch_vcounter ^= 1;
  return regs.ppu2_mdr;
}

//STAT77
uint8 sPPU::mmio_r213e() {
  regs.ppu1_mdr &= 0x10;
  //missing time over, range over flags
  regs.ppu1_mdr |= ppu1_version & 0x0f;
  return regs.ppu1_mdr;
}

//STAT78
uint8 sPPU::mmio_r213f() {
  regs.latch_hcounter = 0;
  regs.latch_vcounter = 0;

  regs.ppu2_mdr &= 0x20;
  regs.ppu2_mdr |= cpu.field() << 7;
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

void sPPU::mmio_reset() {
  regs.ppu1_mdr = 0xff;
  regs.ppu2_mdr = 0xff;

  regs.vram_readbuffer = 0x0000;
  regs.oam_latchdata = 0x00;
  regs.cgram_latchdata = 0x00;
  regs.m7_latchdata = 0x00;
  regs.counters_latched = false;
  regs.latch_hcounter = 0;
  regs.latch_vcounter = 0;

  //$2100  INIDISP
  regs.display_disabled = true;
  regs.display_brightness = 0;

  //$2102  OAMADDL
  //$2103  OAMADDH
  regs.oam_baseaddr = 0x0000;
  regs.oam_addr = 0x0000;
  regs.oam_priority = false;
  regs.oam_firstsprite = 0;

  //$2115  VMAIN
  regs.vram_incmode = 1;
  regs.vram_mapping = 0;
  regs.vram_incsize = 1;

  //$2116  VMADDL
  //$2117  VMADDH
  regs.vram_addr = 0x0000;

  //$211b  M7A
  regs.m7a = 0x0000;

  //$211c  M7B
  regs.m7b = 0x0000;

  //$211d  M7C
  regs.m7c = 0x0000;

  //$211e  M7D
  regs.m7d = 0x0000;

  //$211f  M7X
  regs.m7x = 0x0000;

  //$2120  M7Y
  regs.m7y = 0x0000;

  //$2121  CGADD
  regs.cgram_addr = 0x0000;

  //$213c  OPHCT
  regs.hcounter = 0;

  //$213d  OPVCT
  regs.vcounter = 0;
}

uint8 sPPU::mmio_read(unsigned addr) {
  scheduler.sync_cpuppu();

  switch(addr & 0xffff) {
    case 0x2104:
    case 0x2105:
    case 0x2106:
    case 0x2108:
    case 0x2109:
    case 0x210a:
    case 0x2114:
    case 0x2115:
    case 0x2116:
    case 0x2118:
    case 0x2119:
    case 0x211a:
    case 0x2124:
    case 0x2125:
    case 0x2126:
    case 0x2128:
    case 0x2129:
    case 0x212a: return regs.ppu1_mdr;
    case 0x2134: return mmio_r2134();  //MPYL
    case 0x2135: return mmio_r2135();  //MPYM
    case 0x2136: return mmio_r2136();  //MYPH
    case 0x2137: return mmio_r2137();  //SLHV
    case 0x2138: return mmio_r2138();  //OAMDATAREAD
    case 0x2139: return mmio_r2139();  //VMDATALREAD
    case 0x213a: return mmio_r213a();  //VMDATAHREAD
    case 0x213b: return mmio_r213b();  //CGDATAREAD
    case 0x213c: return mmio_r213c();  //OPHCT
    case 0x213d: return mmio_r213d();  //OPVCT
    case 0x213e: return mmio_r213e();  //STAT77
    case 0x213f: return mmio_r213f();  //STAT78
  }

  return cpu.regs.mdr;
}

void sPPU::mmio_write(unsigned addr, uint8 data) {
  scheduler.sync_cpuppu();

  switch(addr & 0xffff) {
    case 0x2100: return mmio_w2100(data);  //INIDISP
    case 0x2101: return mmio_w2101(data);  //OBSEL
    case 0x2102: return mmio_w2102(data);  //OAMADDL
    case 0x2103: return mmio_w2103(data);  //OAMADDH
    case 0x2104: return mmio_w2104(data);  //OAMDATA
    case 0x2105: return mmio_w2105(data);  //BGMODE
    case 0x2106: return mmio_w2106(data);  //MOSAIC
    case 0x2107: return mmio_w2107(data);  //BG1SC
    case 0x2108: return mmio_w2108(data);  //BG2SC
    case 0x2109: return mmio_w2109(data);  //BG3SC
    case 0x210a: return mmio_w210a(data);  //BG4SC
    case 0x210b: return mmio_w210b(data);  //BG12NBA
    case 0x210c: return mmio_w210c(data);  //BG34NBA
    case 0x210d: return mmio_w210d(data);  //BG1HOFS
    case 0x210e: return mmio_w210e(data);  //BG1VOFS
    case 0x210f: return mmio_w210f(data);  //BG2HOFS
    case 0x2110: return mmio_w2110(data);  //BG2VOFS
    case 0x2111: return mmio_w2111(data);  //BG3HOFS
    case 0x2112: return mmio_w2112(data);  //BG3VOFS
    case 0x2113: return mmio_w2113(data);  //BG4HOFS
    case 0x2114: return mmio_w2114(data);  //BG4VOFS
    case 0x2115: return mmio_w2115(data);  //VMAIN
    case 0x2116: return mmio_w2116(data);  //VMADDL
    case 0x2117: return mmio_w2117(data);  //VMADDH
    case 0x2118: return mmio_w2118(data);  //VMDATAL
    case 0x2119: return mmio_w2119(data);  //VMDATAH
    case 0x211a: return mmio_w211a(data);  //M7SEL
    case 0x211b: return mmio_w211b(data);  //M7A
    case 0x211c: return mmio_w211c(data);  //M7B
    case 0x211d: return mmio_w211d(data);  //M7C
    case 0x211e: return mmio_w211e(data);  //M7D
    case 0x211f: return mmio_w211f(data);  //M7X
    case 0x2120: return mmio_w2120(data);  //M7Y
    case 0x2121: return mmio_w2121(data);  //CGADD
    case 0x2122: return mmio_w2122(data);  //CGDATA
    case 0x2123: return mmio_w2123(data);  //W12SEL
    case 0x2124: return mmio_w2124(data);  //W34SEL
    case 0x2125: return mmio_w2125(data);  //WOBJSEL
    case 0x2126: return mmio_w2126(data);  //WH0
    case 0x2127: return mmio_w2127(data);  //WH1
    case 0x2128: return mmio_w2128(data);  //WH2
    case 0x2129: return mmio_w2129(data);  //WH3
    case 0x212a: return mmio_w212a(data);  //WBGLOG
    case 0x212b: return mmio_w212b(data);  //WOBJLOG
    case 0x212c: return mmio_w212c(data);  //TM
    case 0x212d: return mmio_w212d(data);  //TS
    case 0x212e: return mmio_w212e(data);  //TMW
    case 0x212f: return mmio_w212f(data);  //TSW
    case 0x2130: return mmio_w2130(data);  //CGWSEL
    case 0x2131: return mmio_w2131(data);  //CGADDSUB
    case 0x2132: return mmio_w2132(data);  //COLDATA
    case 0x2133: return mmio_w2133(data);  //SETINI
  }
}

#endif
