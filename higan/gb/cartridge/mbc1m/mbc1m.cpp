auto Cartridge::MBC1M::mmio_read(uint16 addr) -> uint8 {
  if((addr & 0xc000) == 0x0000) {  //$0000-3fff
    if(!modeSelect) return cartridge.rom_read(addr & 0x3fff);
    return cartridge.rom_read((romHi << 4) * 0x4000 + (addr & 0x3fff));
  }

  if((addr & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom_read((romHi << 4 | romLo) * 0x4000 + (addr & 0x3fff));
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    return cartridge.ram_read(addr & 0x1fff);
  }

  return 0xff;
}

auto Cartridge::MBC1M::mmio_write(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x2000) {  //$2000-3fff
    romLo = data.bits(0,3);
  }

  if((addr & 0xe000) == 0x4000) {  //$4000-5fff
    romHi = data.bits(0,1);
  }

  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    modeSelect = data.bit(0);
  }

  if((addr & 0xe000) == 0xa000) {  //$a000-bfff
    cartridge.ram_write(addr & 0x1fff, data);
  }
}

auto Cartridge::MBC1M::power() -> void {
  romHi = 0;
  romLo = 1;
  modeSelect = 0;
}
