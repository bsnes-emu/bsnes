#ifdef CPU_CPP

uint8 CPU::mmio_read(uint16 addr) {
  if(addr >= 0xc000 && addr <= 0xdfff) return wram[addr & 0x1fff];
  if(addr >= 0xe000 && addr <= 0xfdff) return wram[addr & 0x1fff];
  if(addr >= 0xff80 && addr <= 0xfffe) return hram[addr & 0x7f];
}

void CPU::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0xc000 && addr <= 0xdfff) { wram[addr & 0x1fff] = data; return; }
  if(addr >= 0xe000 && addr <= 0xfdff) { wram[addr & 0x1fff] = data; return; }
  if(addr >= 0xff80 && addr <= 0xfffe) { hram[addr & 0x7f] = data; return; }
}

#endif
