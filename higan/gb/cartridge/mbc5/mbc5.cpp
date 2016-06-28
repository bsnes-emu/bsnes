auto Cartridge::MBC5::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(rom.select << 14 | (uint14)addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) return cartridge.readRAM(ram.select << 13 | (uint13)addr);
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MBC5::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    ram.enable = data.bits(0,3) == 0x0a;
    return;
  }

  if((addr & 0xf000) == 0x2000) {  //$2000-2fff
    rom.select.byte(0) = data;
    return;
  }

  if((addr & 0xf000) == 0x3000) {  //$3000-3fff
    rom.select.byte(1) = data.bit(0);
    return;
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    ram.select = data.bits(0,3);
    return;
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    if(ram.enable) cartridge.writeRAM(ram.select << 13 | (uint13)addr, data);
    return;
  }
}

auto Cartridge::MBC5::power() -> void {
  rom.select = 0x001;
  ram.enable = false;
  ram.select = 0x00;
}
