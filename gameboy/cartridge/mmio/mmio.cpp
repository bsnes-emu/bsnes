#ifdef CARTRIDGE_CPP

uint8 Cartridge::mmio_read(uint16 addr) {
  if(addr >= 0x0000 && addr <= 0x7fff) return romdata[addr];
}

void Cartridge::mmio_write(uint16 addr, uint8 data) {
}

#endif
