#ifdef PPU_CPP

uint8 PPU::mmio_read(unsigned addr) {
  switch(addr & 0xffff) {
  }
  return 0x00;
}

void PPU::mmio_write(unsigned addr, uint8 data) {
  switch(addr & 0xffff) {
    case 0x2100: {
      regs.display_disabled = data & 0x80;
      regs.display_brightness = data & 0x0f;
      return;
    }

    case 0x2101: {
      regs.oam_basesize = (data >> 5) & 7;
      regs.oam_nameselect = (data >> 3) & 3;
      regs.oam_tdaddr = (data & 3) << 14;
      return;
    }

    case 0x2102: {
      regs.oam_baseaddr = (regs.oam_baseaddr & 0xff00) | (data << 0);
      regs.oam_baseaddr &= 0x01ff;
      regs.oam_addr = regs.oam_baseaddr << 1;
      regs.oam_firstsprite = (regs.oam_priority == false ? 0 : (regs.oam_addr >> 2) & 127);
      return;
    }

    case 0x2103: {
      regs.oam_priority = data & 0x80;
      regs.oam_baseaddr = (data << 8) | (regs.oam_baseaddr & 0x00ff);
      regs.oam_baseaddr &= 0x01ff;
      regs.oam_addr = regs.oam_baseaddr << 1;
      regs.oam_firstsprite = (regs.oam_priority == false ? 0 : (regs.oam_addr >> 2) & 127);
      return;
    }

    case 0x2104: {
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
      return;
    }

    case 0x2105: {
      regs.bg_tilesize[BG4] = data & 0x80;
      regs.bg_tilesize[BG3] = data & 0x40;
      regs.bg_tilesize[BG2] = data & 0x20;
      regs.bg_tilesize[BG1] = data & 0x10;
      regs.bg3_priority = data & 0x08;
      regs.bg_mode = data & 0x07;
      return;
    }

    case 0x2106: {
      regs.mosaic_size = (data >> 4) & 15;
      regs.mosaic_enabled[BG4] = data & 0x08;
      regs.mosaic_enabled[BG3] = data & 0x04;
      regs.mosaic_enabled[BG2] = data & 0x02;
      regs.mosaic_enabled[BG1] = data & 0x01;
      return;
    }

    case 0x2107: {
      regs.bg_scaddr[BG1] = (data & 0x7c) << 9;
      regs.bg_scsize[BG1] = (data & 0x03);
      return;
    }

    case 0x2108: {
      regs.bg_scaddr[BG2] = (data & 0x7c) << 9;
      regs.bg_scsize[BG2] = (data & 0x03);
      return;
    }

    case 0x2109: {
      regs.bg_scaddr[BG3] = (data & 0x7c) << 9;
      regs.bg_scsize[BG3] = (data & 0x03);
      return;
    }

    case 0x210a: {
      regs.bg_scaddr[BG4] = (data & 0x7c) << 9;
      regs.bg_scsize[BG4] = (data & 0x03);
      return;
    }

    case 0x210b: {
      regs.bg_tdaddr[BG1] = (data & 0x07) << 13;
      regs.bg_tdaddr[BG2] = (data & 0x70) <<  9;
      return;
    }

    case 0x210c: {
      regs.bg_tdaddr[BG3] = (data & 0x07) << 13;
      regs.bg_tdaddr[BG4] = (data & 0x70) <<  9;
      return;
    }

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

    case 0x2116: {
      regs.vram_addr = (regs.vram_addr & 0xff00) | (data << 0);
      uint16 addr = get_vram_addr();
      regs.vram_readbuffer  = vram_read(addr + 0) << 0;
      regs.vram_readbuffer |= vram_read(addr + 1) << 8;
      return;
    }

    case 0x2117: {
      regs.vram_addr = (data << 8) | (regs.vram_addr & 0x00ff);
      uint16 addr = get_vram_addr();
      regs.vram_readbuffer  = vram_read(addr + 0) << 0;
      regs.vram_readbuffer |= vram_read(addr + 1) << 8;
      return;
    }

    case 0x2118: {
      uint16 addr = get_vram_addr() + 0;
      vram_write(addr, data);
      if(regs.vram_incmode == 0) regs.vram_addr += regs.vram_incsize;
      return;
    }

    case 0x2119: {
      uint16 addr = get_vram_addr() + 1;
      vram_write(addr, data);
      if(regs.vram_incmode == 1) regs.vram_addr += regs.vram_incsize;
      return;
    }

    case 0x2121: {
      regs.cgram_addr = data << 1;
      return;
    }

    case 0x2122: {
      if((regs.cgram_addr & 1) == 0) {
        regs.cgram_latchdata = data;
      } else {
        cgram_write((regs.cgram_addr & ~1) + 0, regs.cgram_latchdata);
        cgram_write((regs.cgram_addr & ~1) + 1, data & 0x7f);
      }
      regs.cgram_addr = (regs.cgram_addr + 1) & 0x01ff;
      return;
    }
  }
}

void PPU::mmio_reset() {
  //internal
  regs.width = 256;
  regs.height = 225;

  //$2100
  regs.display_disabled = true;
  regs.display_brightness = 0;

  //$2101
  regs.oam_basesize = 0;
  regs.oam_nameselect = 0;
  regs.oam_tdaddr = 0;

  //$2102-$2103
  regs.oam_baseaddr = 0;
  regs.oam_addr = 0;
  regs.oam_priority = 0;
  regs.oam_firstsprite = 0;

  //$2104
  regs.oam_latchdata = 0;

  //$2105
  for(unsigned i = 0; i < 4; i++) regs.bg_tilesize[i] = 0;
  regs.bg3_priority = 0;
  regs.bg_mode = 0;

  //$2106
  regs.mosaic_size = 0;
  for(unsigned i = 0; i < 4; i++) regs.mosaic_enabled[i] = 0;

  //$2107-$210a
  for(unsigned i = 0; i < 4; i++) regs.bg_scaddr[i] = 0;
  for(unsigned i = 0; i < 4; i++) regs.bg_scsize[i] = 0;

  //$210b-$210c
  for(unsigned i = 0; i < 4; i++) regs.bg_tdaddr[i] = 0;

  //$2115
  regs.vram_incmode = 0;
  regs.vram_mapping = 0;
  regs.vram_incsize = 0;

  //$2116-$2117
  regs.vram_addr = 0;

  //$2121
  regs.cgram_addr = 0;

  //$2122
  regs.cgram_latchdata = 0;

  //$2139-$213a
  regs.vram_readbuffer = 0;
}

#endif
