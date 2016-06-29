auto ICD2::readIO(uint24 addr, uint8 data) -> uint8 {
  addr &= 0x40ffff;

  //LY counter
  if(addr == 0x6000) {
    uint y = min((uint8)143, GameBoy::ppu.status.ly);
    return (y & ~7) | writeBank;
  }

  //command ready port
  if(addr == 0x6002) {
    data = packetSize > 0;
    if(data) {
      for(auto n : range(16)) r7000[n] = packet[0][n];
      packetSize--;
      for(auto n : range(packetSize)) packet[n] = packet[n + 1];
    }
    return data;
  }

  //ICD2 revision
  if(addr == 0x600f) {
    return 0x21;
  }

  //command port
  if((addr & 0x40fff0) == 0x7000) {
    return r7000[addr & 15];
  }

  //VRAM port
  if(addr == 0x7800) {
    data = output[readBank * 512 + readAddress];
    readAddress = (readAddress + 1) & 511;
    return data;
  }

  return 0x00;
}

auto ICD2::writeIO(uint24 addr, uint8 data) -> void {
  addr &= 0xffff;

  //VRAM port
  if(addr == 0x6001) {
    readBank = data & 3;
    readAddress = 0;
    return;
  }

  //control port
  //d7: 0 = halt, 1 = reset
  //d5,d4: 0 = 1-player, 1 = 2-player, 2 = 4-player, 3 = ???
  //d1,d0: 0 = frequency divider (clock rate adjust)
  if(addr == 0x6003) {
    if((r6003 & 0x80) == 0x00 && (data & 0x80) == 0x80) {
      reset(true);
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
