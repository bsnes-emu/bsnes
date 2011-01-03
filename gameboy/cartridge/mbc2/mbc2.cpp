#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC2::mmio_read(uint16 addr) {
  if(addr >= 0x0000 && addr <= 0x3fff) {
    return cartridge.rom_read(addr);
  }

  if(addr >= 0x4000 && addr <= 0x7fff) {
    return cartridge.rom_read((rom_select << 14) | (addr & 0x3fff));
  }

  if(addr >= 0xa000 && addr <= 0xa1ff) {
    if(ram_enable) {
      uint8 data = cartridge.ram_read(addr & 0x1ff);
      return (addr & 1) == 0 ? ((data >> 4) & 0x0f) : ((data >> 0) & 0x0f);
    }
    return 0x00;
  }

  return 0x00;
}

void Cartridge::MBC2::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0x0000 && addr <= 0x1fff) {
    if((addr & 0x100) == 0) {
      ram_enable = (data & 0x0f) == 0x0a;
    }
  }

  if(addr >= 0x2000 && addr <= 0x3fff) {
    if(addr & 0x100) {
      rom_select = data & 0x0f;
      if(rom_select == 0) rom_select = 1;
    }
  }

  if(addr >= 0xa000 && addr <= 0xa1ff) {
    if(ram_enable) {
      addr &= 511;
      if((addr & 1) == 0) {
        cartridge.ram_write(addr, (cartridge.ram_read(addr) & 0x0f) | (data << 4));
      } else {
        cartridge.ram_write(addr, (cartridge.ram_read(addr) & 0xf0) | (data << 0));
      }
      return;
    }
  }
}

void Cartridge::MBC2::power() {
  ram_enable = false;
  rom_select = 0x01;
}

#endif
