auto Cartridge::MMM01::readIO(uint16 addr) -> uint8 {
  if((addr & 0x8000) == 0x0000) {  //$0000-7fff
    if(mode == 0) return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(0x8000 + (rom.base << 14) + (uint14)addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(0x8000 + (rom.base << 14) + (rom.select<< 14) + (uint14)addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) return cartridge.readRAM(ram.select << 13 | (uint13)addr);
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MMM01::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    if(mode == 0) {
      mode = 1;
    } else {
      ram.enable= data.bits(0,3) == 0x0a;
    }
  }

  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    if(mode == 0) {
      rom.base = data.bits(0,5);
    } else {
      rom.select = data;
    }
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    if(mode == 1) {
      ram.select = data;
    }
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    //unknown purpose
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) cartridge.writeRAM(ram.select << 13 | (uint13)addr, data);
  }
}

auto Cartridge::MMM01::power() -> void {
  rom.base = 0x00;
  rom.select = 0x01;
  ram.enable = false;
  ram.select = 0x00;
  mode = 0;
}
