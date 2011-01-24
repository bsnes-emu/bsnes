#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC2::mmio_read(uint16 addr) {
  if(within<0x0000, 0x3fff>(addr)) {
    return cartridge.rom_read(addr);
  }

  if(within<0x4000, 0x7fff>(addr)) {
    return cartridge.rom_read((rom_select << 14) | (addr & 0x3fff));
  }

  if(within<0xa000, 0xa1ff>(addr)) {
    if(ram_enable) return cartridge.ram_read(addr & 0x1ff);
    return 0x00;
  }

  return 0x00;
}

void Cartridge::MBC2::mmio_write(uint16 addr, uint8 data) {
  if(within<0x0000, 0x1fff>(addr)) {
    if(!(addr & 0x0100)) ram_enable = (data & 0x0f) == 0x0a;
    return;
  }

  if(within<0x2000, 0x3fff>(addr)) {
    if( (addr & 0x0100)) rom_select = (data & 0x0f) + ((data & 0x0f) == 0);
    return;
  }

  if(within<0xa000, 0xa1ff>(addr)) {
    if(ram_enable) cartridge.ram_write(addr & 0x1ff, data & 0x0f);
    return;
  }
}

void Cartridge::MBC2::power() {
  ram_enable = false;
  rom_select = 0x01;
}

#endif
