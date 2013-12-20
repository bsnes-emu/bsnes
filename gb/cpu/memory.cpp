#ifdef CPU_CPP

void CPU::op_io() {
  cycle_edge();
  add_clocks(4);
}

uint8 CPU::op_read(uint16 addr) {
  cycle_edge();
  add_clocks(4);
  if(oamdma.active && (addr < 0xff80 || addr == 0xffff)) return 0x00;
  return bus.read(addr);
}

void CPU::op_write(uint16 addr, uint8 data) {
  cycle_edge();
  add_clocks(4);
  if(oamdma.active && (addr < 0xff80 || addr == 0xffff)) return;
  bus.write(addr, data);
}

void CPU::cycle_edge() {
  if(r.ei) {
    r.ei = false;
    r.ime = 1;
  }
}

//VRAM DMA source can only be ROM or RAM
uint8 CPU::dma_read(uint16 addr) {
  if(addr < 0x8000) return bus.read(addr);  //0000-7fff
  if(addr < 0xa000) return 0x00;            //8000-9fff
  if(addr < 0xe000) return bus.read(addr);  //a000-dfff
  return 0x00;                              //e000-ffff
}

//VRAM DMA target is always VRAM
void CPU::dma_write(uint16 addr, uint8 data) {
  addr = 0x8000 | (addr & 0x1fff);  //8000-9fff
  return bus.write(addr, data);
}

uint8 CPU::debugger_read(uint16 addr) {
  return bus.read(addr);
}

#endif
