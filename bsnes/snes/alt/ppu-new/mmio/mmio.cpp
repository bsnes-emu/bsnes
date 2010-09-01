#ifdef PPU_CPP

void PPU::latch_counters() {}
bool PPU::interlace() const { return false; }
bool PPU::overscan() const { return false; }
bool PPU::hires() const { return false; }

uint16 PPU::get_vram_addr() {
  uint16 addr = regs.vram_addr;
  switch(regs.vram_mapping) {
    case 0: break;
    case 1: addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | ((addr >> 5) & 7); break;
    case 2: addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | ((addr >> 6) & 7); break;
    case 3: addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | ((addr >> 7) & 7); break;
  }
  return (addr << 1);
}

uint8 PPU::vram_read(unsigned addr) {
  if(regs.display_disable) return memory::vram[addr];
  if(cpu.vcounter() >= display.height) return memory::vram[addr];
  return 0x00;
}

void PPU::vram_write(unsigned addr, uint8 data) {
  if(regs.display_disable) { memory::vram[addr] = data; return; }
  if(cpu.vcounter() >= display.height) { memory::vram[addr] = data; return; }
}

uint8 PPU::oam_read(unsigned addr) {
  if(addr & 0x0200) addr &= 0x021f;
  if(regs.display_disable) return memory::oam[addr];
  if(cpu.vcounter() >= display.height) return memory::oam[addr];
  return memory::oam[0x0218];
}

void PPU::oam_write(unsigned addr, uint8 data) {
  if(addr & 0x0200) addr &= 0x021f;
  if(regs.display_disable) { memory::oam[addr] = data; return; }
  if(cpu.vcounter() >= display.height) { memory::oam[addr] = data; return; }
  memory::oam[0x0218] = data;
}

uint8 PPU::cgram_read(unsigned addr) {
  return memory::cgram[addr];
}

void PPU::cgram_write(unsigned addr, uint8 data) {
  memory::cgram[addr] = data;
}

uint8 PPU::mmio_read(unsigned addr) {
  cpu.synchronize_ppu();

  switch(addr & 0xffff) {
    case 0x2104: case 0x2105: case 0x2106: case 0x2108: case 0x2109: case 0x210a:
    case 0x2114: case 0x2115: case 0x2116: case 0x2118: case 0x2119: case 0x211a:
    case 0x2124: case 0x2125: case 0x2126: case 0x2128: case 0x2129: case 0x212a: {
      return regs.ppu1_mdr;
    }

    case 0x2134: {  //MPYL
      unsigned result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
      regs.ppu1_mdr = result >>  0;
      return regs.ppu1_mdr;
    }

    case 0x2135: {  //MPYM
      unsigned result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
      regs.ppu1_mdr = result >>  8;
      return regs.ppu1_mdr;
    }

    case 0x2136: {  //MPYH
      unsigned result = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
      regs.ppu1_mdr = result >> 16;
      return regs.ppu1_mdr;
    }

    case 0x2137: {  //SLHV
      if(cpu.pio() & 0x80) latch_counters();
      return cpu.regs.mdr;
    }

    case 0x2138: {  //OAMDATAREAD
      regs.ppu1_mdr = oam_read(regs.oam_addr);
      regs.oam_addr = (regs.oam_addr + 1) & 0x03ff;
      //TODO: handle OAM address reset here
      return regs.ppu1_mdr;
    }

    case 0x2139: {  //VMDATALREAD
      regs.ppu1_mdr = regs.vram_readbuffer >> 0;
      if(regs.vram_incmode == 0) {
        uint16 addr = get_vram_addr();
        regs.vram_readbuffer  = vram_read(addr + 0) << 0;
        regs.vram_readbuffer |= vram_read(addr + 1) << 8;
        regs.vram_addr += regs.vram_incsize;
      }
      return regs.ppu1_mdr;
    }

    case 0x213a: {  //VMDATAHREAD
      regs.ppu1_mdr = regs.vram_readbuffer >> 8;
      if(regs.vram_incmode == 1) {
        uint16 addr = get_vram_addr();
        regs.vram_readbuffer  = vram_read(addr + 0) << 0;
        regs.vram_readbuffer |= vram_read(addr + 1) << 8;
        regs.vram_addr += regs.vram_incsize;
      }
      return regs.ppu1_mdr;
    }

    case 0x213b: {  //CGDATAREAD
      if((regs.cgram_addr & 1) == 0) {
        regs.ppu2_mdr = cgram_read(regs.cgram_addr);
      } else {
        regs.ppu2_mdr = (regs.ppu2_mdr & 0x80) | (cgram_read(regs.cgram_addr) & 0x7f);
      }
      regs.cgram_addr = (regs.cgram_addr + 1) & 0x01ff;
      return regs.ppu2_mdr;
    }
  }

  return cpu.regs.mdr;
}

void PPU::mmio_write(unsigned addr, uint8 data) {
  cpu.synchronize_ppu();

  switch(addr & 0xffff) {
    case 0x2100: {  //INIDISP
      //TODO: handle OAM address reset here
      regs.display_disable = data & 0x80;
      regs.display_brightness = data & 0x0f;
      screen.light_table = screen.light_tables[regs.display_brightness];
      return;
    }

    case 0x2102: {  //OAMADDL
      regs.oam_baseaddr = (regs.oam_baseaddr & 0x0100) | (data << 0);
      //TODO: handle OAM address reset here
      return;
    }

    case 0x2103: {  //OAMADDH
      regs.oam_priority = data & 0x80;
      regs.oam_baseaddr = ((data & 1) << 8) | (regs.oam_baseaddr & 0x00ff);
      //TODO: handle OAM address reset here
      return;
    }

    case 0x2104: {  //OAMDATA
      if(regs.oam_addr & 0x0200) {
        oam_write(regs.oam_addr, data);
      } else if((regs.oam_addr & 1) == 0) {
        regs.oam_latchdata = data;
      } else {
        oam_write((regs.oam_addr & ~1) + 0, regs.oam_latchdata);
        oam_write((regs.oam_addr & ~1) + 1, data);
      }
      regs.oam_addr = (regs.oam_addr + 1) & 0x03ff;
      //TODO: handle OAM address reset here
      return;
    }

    case 0x2115: {  //VMAIN
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

    case 0x2116: {  //VMADDL
      regs.vram_addr = (regs.vram_addr & 0xff00) | (data << 0);
      uint16 addr = get_vram_addr();
      regs.vram_readbuffer  = vram_read(addr + 0) << 0;
      regs.vram_readbuffer |= vram_read(addr + 1) << 8;
      return;
    }

    case 0x2117: {  //VMADDH
      regs.vram_addr = (data << 8) | (regs.vram_addr & 0x00ff);
      uint16 addr = get_vram_addr();
      regs.vram_readbuffer  = vram_read(addr + 0) << 0;
      regs.vram_readbuffer |= vram_read(addr + 1) << 8;
      return;
    }

    case 0x2118: {  //VMDATAL
      vram_write(get_vram_addr() + 0, data);
      if(regs.vram_incmode == 0) regs.vram_addr += regs.vram_incsize;
      return;
    }

    case 0x2119: {  //VMDATAH
      vram_write(get_vram_addr() + 1, data);
      if(regs.vram_incmode == 1) regs.vram_addr += regs.vram_incsize;
      return;
    }

    case 0x211a: {  //M7SEL
      regs.mode7_repeat = (data >> 6) & 3;
      regs.mode7_vflip = data & 0x02;
      regs.mode7_hflip = data & 0x01;
      return;
    }

    case 0x211b: {  //M7A
      regs.m7a = (data << 8) | regs.mode7_latchdata;
      regs.mode7_latchdata = data;
      return;
    }

    case 0x211c: {  //M7B
      regs.m7b = (data << 8) | regs.mode7_latchdata;
      regs.mode7_latchdata = data;
      return;
    }

    case 0x211d: {  //M7C
      regs.m7c = (data << 8) | regs.mode7_latchdata;
      regs.mode7_latchdata = data;
      return;
    }

    case 0x211e: {  //M7D
      regs.m7d = (data << 8) | regs.mode7_latchdata;
      regs.mode7_latchdata = data;
      return;
    }

    case 0x211f: {  //M7X
      regs.m7x = (data << 8) | regs.mode7_latchdata;
      regs.mode7_latchdata = data;
      return;
    }

    case 0x2120: {  //M7Y
      regs.m7y = (data << 8) | regs.mode7_latchdata;
      regs.mode7_latchdata = data;
      return;
    }

    case 0x2121: {  //CGADD
      regs.cgram_addr = data << 1;
      return;
    }

    case 0x2122: {  //CGDATA
      if((regs.cgram_addr & 1) == 0) {
        regs.cgram_latchdata = data;
      } else {
        cgram_write((regs.cgram_addr & ~1) + 0, regs.cgram_latchdata);
        cgram_write((regs.cgram_addr & ~1) + 1, data);
      }
      regs.cgram_addr = (regs.cgram_addr + 1) & 0x01ff;
      return;
    }
  }
}

void PPU::mmio_reset() {
  //internal
  regs.ppu1_mdr = 0;
  regs.ppu2_mdr = 0;

  regs.vram_readbuffer = 0;
  regs.oam_latchdata = 0;
  regs.cgram_latchdata = 0;
  regs.mode7_latchdata = 0;

  //$2100
  regs.display_disable = true;
  regs.display_brightness = 0;
  screen.light_table = screen.light_tables[regs.display_brightness];

  //$2102-$2103
  regs.oam_baseaddr = 0;
  regs.oam_addr = 0;
  regs.oam_priority = 0;

  //$2115
  regs.vram_incmode = 0;
  regs.vram_mapping = 0;
  regs.vram_incsize = 1;

  //$2116-$2117
  regs.vram_addr = 0;

  //$211a
  regs.mode7_repeat = 0;
  regs.mode7_vflip = 0;
  regs.mode7_hflip = 0;

  //$211b-$2120
  regs.m7a = 0;
  regs.m7b = 0;
  regs.m7c = 0;
  regs.m7d = 0;
  regs.m7x = 0;
  regs.m7y = 0;

  //$2121
  regs.cgram_addr = 0;
}

#endif
