auto CPU::op_io() -> void {
  cycle_edge();
  add_clocks(4);
}

auto CPU::op_read(uint16 addr) -> uint8 {
  cycle_edge();
  add_clocks(4);
  //OAM is inaccessible during OAMDMA transfer
  if(oamdma.active && oamdma.clock >= 8 && addr >= 0xfe00 && addr <= 0xfe9f) return 0xff;
  return bus.read(addr);
}

auto CPU::op_write(uint16 addr, uint8 data) -> void {
  cycle_edge();
  add_clocks(4);
  //OAM is inaccessible during OAMDMA transfer
  if(oamdma.active && oamdma.clock >= 8 && addr >= 0xfe00 && addr <= 0xfe9f) return;
  bus.write(addr, data);
}

auto CPU::cycle_edge() -> void {
  if(r.ei) {
    r.ei = false;
    r.ime = 1;
  }
}

//VRAM DMA source can only be ROM or RAM
auto CPU::dma_read(uint16 addr) -> uint8 {
  if(addr < 0x8000) return bus.read(addr);  //0000-7fff
  if(addr < 0xa000) return 0xff;            //8000-9fff
  if(addr < 0xe000) return bus.read(addr);  //a000-dfff
  return 0xff;                              //e000-ffff
}

//VRAM DMA target is always VRAM
auto CPU::dma_write(uint16 addr, uint8 data) -> void {
  addr = 0x8000 | (addr & 0x1fff);  //8000-9fff
  return bus.write(addr, data);
}

auto CPU::debugger_read(uint16 addr) -> uint8 {
  return bus.read(addr);
}
