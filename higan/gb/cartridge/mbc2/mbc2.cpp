auto Cartridge::MBC2::readIO(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.readROM(rom.select << 14 | (uint14)addr);
  }

  if((addr & 0xee00) == 0xa000) {  //$a000-a1ff
    if(ram.enable) return cartridge.readRAM((uint9)addr);
    return 0xff;
  }

  return 0xff;
}

auto Cartridge::MBC2::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {  //$0000-1fff
    if(!addr.bit(8)) ram.enable = data.bits(0,3) == 0x0a;
    return;
  }

  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    if( addr.bit(8)) rom.select = data.bits(0,3) + (data.bits(0,3) == 0);
    return;
  }

  if((addr & 0xee00) == 0xa000) {  //$a000-a1ff
    if(ram.enable) cartridge.writeRAM((uint9)addr, data.bits(0,3));
    return;
  }
}

auto Cartridge::MBC2::power() -> void {
  rom.select = 0x01;
  ram.enable = false;
}
