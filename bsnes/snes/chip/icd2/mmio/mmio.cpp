#ifdef ICD2_CPP

uint8 ICD2::mmio_read(unsigned addr) {
  if((uint16)addr == 0x2181) return mmio[0]->mmio_read(addr);
  if((uint16)addr == 0x2182) return mmio[1]->mmio_read(addr);
  if((uint16)addr == 0x420b) return mmio[2]->mmio_read(addr);
  return 0x00;
}

void ICD2::mmio_write(unsigned addr, uint8 data) {
  if((uint16)addr == 0x420b && data == 0x10) {
    unsigned offset = (r2182 << 8) | (r2181 << 0);
    r7800 = 0;
    unsigned row = 0;
    if(offset >= 0x5000 && offset <= 0x6540) row = (offset - 0x5000) / 320;
    if(offset >= 0x6800 && offset <= 0x7d40) row = (offset - 0x6800) / 320;

    uint8 *source = GameBoy::lcd.screen + row * 160 * 8;
    memset(vram, 0x00, 320);

    for(unsigned y = row * 8; y < row * 8 + 8; y++) {
      for(unsigned x = 0; x < 160; x++) {
        unsigned pixel = *source++ / 0x55;
        pixel ^= 3;

        unsigned addr = (x / 8 * 16) + ((y & 7) * 2);
        vram[addr + 0] |= ((pixel & 1) >> 0) << (7 - (x & 7));
        vram[addr + 1] |= ((pixel & 2) >> 1) << (7 - (x & 7));
      }
    }
  }

  if((uint16)addr == 0x2181) return mmio[0]->mmio_write(addr, r2181 = data);
  if((uint16)addr == 0x2182) return mmio[1]->mmio_write(addr, r2182 = data);
  if((uint16)addr == 0x420b) return mmio[2]->mmio_write(addr, data);
}

uint8 ICD2::read(unsigned addr) {
  addr &= 0xffff;

  //LY counter
  if(addr == 0x6000) {
    return GameBoy::lcd.status.ly;
  }

  //command ready port
  if(addr == 0x6002) {
    bool data = packetsize > 0;
    if(data) {
      for(unsigned i = 0; i < 16; i++) r7000[i] = packet[0][i];
      packetsize--;
      for(unsigned i = 0; i < packetsize; i++) packet[i] = packet[i + 1];
    }
    return data;
  }

  //ICD2 revision
  if(addr == 0x600f) {
    return 0x21;
  }

  //command port
  if((addr & 0xfff0) == 0x7000) {
    return r7000[addr & 15];
  }

  //VRAM port
  if(addr == 0x7800) {
    uint8 data = vram[r7800];
    r7800 = (r7800 + 1) % 320;
    return data;
  }

  return 0x00;
}

void ICD2::write(unsigned addr, uint8 data) {
  addr &= 0xffff;

  //control port
  //d7: 0 = halt, 1 = reset
  //d5,d4: 0 = 1-player, 1 = 2-player, 2 = 4-player, 3 = ???
  //d1,d0: 0 = frequency divider (clock rate adjust)
  if(addr == 0x6003) {
    if((r6003 & 0x80) == 0x00 && (data & 0x80) == 0x80) {
      reset();
    }
    switch(data & 3) {
      case 0: frequency = cpu.frequency / 4; break;  //fast (glitchy, even on real hardware)
      case 1: frequency = cpu.frequency / 5; break;  //normal
      case 2: frequency = cpu.frequency / 7; break;  //slow
      case 3: frequency = cpu.frequency / 9; break;  //very slow
    }
    r6003 = data;
    return;
  }

  if(addr == 0x6004) { r6004 = data; return; }  //joypad 1
  if(addr == 0x6005) { r6005 = data; return; }  //joypad 2
  if(addr == 0x6006) { r6006 = data; return; }  //joypad 3
  if(addr == 0x6007) { r6007 = data; return; }  //joypad 4
}

#endif
