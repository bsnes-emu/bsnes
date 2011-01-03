#ifdef CARTRIDGE_CPP

uint8 Cartridge::MBC0::mmio_read(uint16 addr) {
  if(addr >= 0x0000 && addr <= 0x7fff) return cartridge.rom_read(addr);
  return 0x00;
}

void Cartridge::MBC0::mmio_write(uint16 addr, uint8 data) {
}

void Cartridge::MBC0::power() {
}

#endif
