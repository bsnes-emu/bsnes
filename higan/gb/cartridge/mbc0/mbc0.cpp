auto Cartridge::MBC0::readIO(uint16 addr) -> uint8 {
  if((addr & 0x8000) == 0x0000) {  //$0000-7fff
    return cartridge.readROM(addr);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    return cartridge.readRAM((uint13)addr);
  }

  return 0xff;
}

auto Cartridge::MBC0::writeIO(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    cartridge.writeRAM((uint13)addr, data);
    return;
  }
}

auto Cartridge::MBC0::power() -> void {
}
