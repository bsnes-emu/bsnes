#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC5::mmio_read(uint16 addr) {
  if(addr >= 0x0000 && addr <= 0x3fff) {
    return cartridge.rom_read(addr);
  }

  if(addr >= 0x4000 && addr <= 0x7fff) {
    return cartridge.rom_read((rom_select << 14) | (addr & 0x3fff));
  }

  if(addr >= 0xa000 && addr <= 0xbfff) {
    if(ram_enable) {
      return cartridge.ram_read((ram_select << 13) | (addr & 0x1fff));
    }
    return 0x00;
  }

  return 0x00;
}

void Cartridge::MBC5::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0x0000 && addr <= 0x1fff) {
    ram_enable = (data & 0x0f) == 0x0a;
  }

  if(addr >= 0x2000 && addr <= 0x2fff) {
    rom_select = (rom_select & 0x0100) | data;
  }

  if(addr >= 0x3000 && addr <= 0x3fff) {
    rom_select = ((data & 1) << 8) | (rom_select & 0x00ff);
  }

  if(addr >= 0x4000 && addr <= 0x5fff) {
    ram_select = data & 0x0f;
  }

  if(addr >= 0xa000 && addr <= 0xbfff) {
    if(ram_enable) {
      cartridge.ram_write((ram_select << 13) | (addr & 0x1fff), data);
    }
  }
}

void Cartridge::MBC5::power() {
  ram_enable = false;
  rom_select = 0x001;
  ram_select = 0x00;
}

#endif
