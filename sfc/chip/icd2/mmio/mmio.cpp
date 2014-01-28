#ifdef ICD2_CPP

uint8 ICD2::read(unsigned addr) {
  addr &= 0xffff;

  //LY counter
  if(addr == 0x6000) {
    unsigned y = min(143u, GameBoy::ppu.status.ly);
    return (y & ~7) | write_bank;
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
    uint8 data = output[read_bank * 512 + read_addr];
    read_addr = (read_addr + 1) & 511;
    return data;
  }

  return 0x00;
}

void ICD2::write(unsigned addr, uint8 data) {
  addr &= 0xffff;

  //VRAM port
  if(addr == 0x6001) {
    read_bank = data & 3;
    read_addr = 0;
    return;
  }

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
