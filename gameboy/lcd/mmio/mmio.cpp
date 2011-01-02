#ifdef LCD_CPP

uint8 LCD::mmio_read(uint16 addr) {
  if(addr >= 0x8000 && addr <= 0x9fff) return vram[addr & 0x1fff];
  if(addr >= 0xfe00 && addr <= 0xfe9f) return oam[addr & 0xff];

  if(addr == 0xff40) {  //LCDC
    return (status.display_enable << 7)
         | (status.window_tilemap_select << 6)
         | (status.window_display_enable << 5)
         | (status.bg_tiledata_select << 4)
         | (status.bg_tilemap_select << 3)
         | (status.obj_size << 2)
         | (status.obj_enable << 1)
         | (status.bg_enable << 0);
  }

  if(addr == 0xff41) {  //STAT
    unsigned mode;
    if(status.ly >= 144) mode = 1;  //Vblank
    else if(status.lx >= 320) mode = 0;  //Hblank
    else mode = 3;  //LCD transfer

    return (status.interrupt_lyc << 6)
         | (status.interrupt_oam << 5)
         | (status.interrupt_vblank << 4)
         | (status.interrupt_hblank << 3)
         | ((status.ly == status.lyc) << 2)
         | (mode << 0);
  }

  if(addr == 0xff42) {  //SCY
    return status.scy;
  }

  if(addr == 0xff43) {  //SCX
    return status.scx;
  }

  if(addr == 0xff44) {  //LY
    return status.ly;
  }

  if(addr == 0xff45) {  //LYC
    return status.lyc;
  }

  if(addr == 0xff47) {  //BGP
    return (status.bgp[3] << 6)
         | (status.bgp[2] << 4)
         | (status.bgp[1] << 2)
         | (status.bgp[0] << 0);
  }

  if(addr == 0xff48) {  //OBP0
    return (status.obp[0][3] << 6)
         | (status.obp[0][2] << 4)
         | (status.obp[0][1] << 2)
         | (status.obp[0][0] << 0);
  }

  if(addr == 0xff49) {  //OBP1
    return (status.obp[1][3] << 6)
         | (status.obp[1][2] << 4)
         | (status.obp[1][1] << 2)
         | (status.obp[1][0] << 0);
  }

  if(addr == 0xff4a) {  //WY
    return status.wy;
  }

  if(addr == 0xff4b) {  //WX
    return status.wx;
  }

  return 0x00;
}

void LCD::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0x8000 && addr <= 0x9fff) { vram[addr & 0x1fff] = data; return; }
  if(addr >= 0xfe00 && addr <= 0xfe9f) { oam[addr & 0xff] = data; return; }

  if(addr == 0xff40) {  //LCDC
    status.display_enable = data & 0x80;
    status.window_tilemap_select = data & 0x40;
    status.window_display_enable = data & 0x20;
    status.bg_tiledata_select = data & 0x10;
    status.bg_tilemap_select = data & 0x08;
    status.obj_size = data & 0x04;
    status.obj_enable = data & 0x02;
    status.bg_enable = data & 0x01;
    return;
  }

  if(addr == 0xff41) {  //STAT
    status.interrupt_lyc = data & 0x40;
    status.interrupt_oam = data & 0x20;
    status.interrupt_vblank = data & 0x10;
    status.interrupt_hblank = data & 0x08;
    return;
  }

  if(addr == 0xff42) {  //SCY
    status.scy = data;
    return;
  }

  if(addr == 0xff43) {  //SCX
    status.scx = data;
    return;
  }

  if(addr == 0xff44) {  //LY
    status.ly = 0;
    return;
  }

  if(addr == 0xff45) {  //LYC
    status.lyc = data;
    return;
  }

  if(addr == 0xff46) {  //DMA
    for(unsigned n = 0x00; n <= 0x9f; n++) bus.write(0xfe00 + n, bus.read((data << 8) + n));
    return;
  }

  if(addr == 0xff47) {  //BGP
    status.bgp[3] = (data >> 6) & 3;
    status.bgp[2] = (data >> 4) & 3;
    status.bgp[1] = (data >> 2) & 3;
    status.bgp[0] = (data >> 0) & 3;
    return;
  }

  if(addr == 0xff48) {  //OBP0
    status.obp[0][3] = (data >> 6) & 3;
    status.obp[0][2] = (data >> 4) & 3;
    status.obp[0][1] = (data >> 2) & 3;
    status.obp[0][0] = (data >> 0) & 3;
    return;
  }

  if(addr == 0xff49) {  //OBP1
    status.obp[1][3] = (data >> 6) & 3;
    status.obp[1][2] = (data >> 4) & 3;
    status.obp[1][1] = (data >> 2) & 3;
    status.obp[1][0] = (data >> 0) & 3;
    return;
  }

  if(addr == 0xff4a) {  //WY
    status.wy = data;
    return;
  }

  if(addr == 0xff4b) {  //WX
    status.wx = data;
    return;
  }
}

#endif
