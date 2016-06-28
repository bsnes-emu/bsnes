auto Cartridge::MBC1M::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    if(mode == 0) return cartridge.readROM((uint14)addr);
    return cartridge.readROM(rom.hi << 18 | (uint14)addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(rom.hi << 18 | rom.lo << 14 | (uint14)addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    return cartridge.readRAM((uint13)addr);
  }

  return 0xff;
}

auto Cartridge::MBC1M::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    rom.lo = data.bits(0,3);
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    rom.hi = data.bits(0,1);
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    mode = data.bit(0);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    cartridge.writeRAM((uint13)addr, data);
  }
}

auto Cartridge::MBC1M::power() -> void {
  rom.lo = 1;
  rom.hi = 0;
  mode = 0;
}
