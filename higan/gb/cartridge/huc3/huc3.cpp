auto Cartridge::HuC3::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(rom.select << 14 | (uint14)addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) return cartridge.readRAM(ram.select << 13 | (uint13)addr);
    return 0x01;  //does not return open collection
  }

  return 0xff;
}

auto Cartridge::HuC3::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    ram.enable = data.bits(0,3) == 0x0a;
    return;
  }

  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    rom.select = data;
    return;
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    ram.select = data;
    return;
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    //unknown purpose
    return;
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) cartridge.writeRAM(ram.select << 13 | (uint13)addr, data);
    return;
  }
}

auto Cartridge::HuC3::power() -> void {
  rom.select = 0x01;
  ram.enable = false;
  ram.select = 0x00;
}
