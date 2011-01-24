#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC0::mmio_read(uint16 addr) {
  if(within<0x0000, 0x7fff>(addr)) {
    return cartridge.rom_read(addr);
  }

  if(within<0xa000, 0xbfff>(addr)) {
    return cartridge.ram_read(addr & 0x1fff);
  }

  return 0x00;
}

void Cartridge::MBC0::mmio_write(uint16 addr, uint8 data) {
  if(within<0xa000, 0xbfff>(addr)) {
    cartridge.ram_write(addr & 0x1fff, data);
    return;
  }
}

void Cartridge::MBC0::power() {
}

#endif
