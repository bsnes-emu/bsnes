#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC5::mmio_read(uint16 addr) {
  if(within<0x0000, 0x3fff>(addr)) {
    return cartridge.rom_read(addr);
  }

  if(within<0x4000, 0x7fff>(addr)) {
    return cartridge.rom_read((rom_select << 14) | (addr & 0x3fff));
  }

  if(within<0xa000, 0xbfff>(addr)) {
    if(ram_enable) return cartridge.ram_read((ram_select << 13) | (addr & 0x1fff));
    return 0x00;
  }

  return 0x00;
}

void Cartridge::MBC5::mmio_write(uint16 addr, uint8 data) {
  if(within<0x0000, 0x1fff>(addr)) {
    ram_enable = (data & 0x0f) == 0x0a;
    return;
  }

  if(within<0x2000, 0x2fff>(addr)) {
    rom_select = (rom_select & 0x0100) | data;
    return;
  }

  if(within<0x3000, 0x3fff>(addr)) {
    rom_select = ((data & 1) << 8) | (rom_select & 0x00ff);
    return;
  }

  if(within<0x4000, 0x5fff>(addr)) {
    ram_select = data & 0x0f;
    return;
  }

  if(within<0xa000, 0xbfff>(addr)) {
    if(ram_enable) cartridge.ram_write((ram_select << 13) | (addr & 0x1fff), data);
    return;
  }
}

void Cartridge::MBC5::power() {
  ram_enable = false;
  rom_select = 0x001;
  ram_select = 0x00;
}

#endif
