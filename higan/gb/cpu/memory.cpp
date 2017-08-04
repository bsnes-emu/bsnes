auto CPU::idle() -> void {
  cycleEdge();
  step(4);
}

auto CPU::read(uint16 addr) -> uint8 {
  cycleEdge();
  step(4);
  return bus.read(addr);
}

auto CPU::write(uint16 addr, uint8 data) -> void {
  cycleEdge();
  step(4);
  bus.write(addr, data);
}

auto CPU::cycleEdge() -> void {
  if(r.ei) {
    r.ei = false;
    r.ime = 1;
  }
}

//VRAM DMA source can only be ROM or RAM
auto CPU::readDMA(uint16 addr) -> uint8 {
  if(addr < 0x8000) return bus.read(addr);  //0000-7fff
  if(addr < 0xa000) return 0xff;            //8000-9fff
  if(addr < 0xe000) return bus.read(addr);  //a000-dfff
  return 0xff;                              //e000-ffff
}

//VRAM DMA target is always VRAM
auto CPU::writeDMA(uint16 addr, uint8 data) -> void {
  addr = 0x8000 | (addr & 0x1fff);  //8000-9fff
  return bus.write(addr, data);
}

auto CPU::readDebugger(uint16 addr) -> uint8 {
  return bus.read(addr);
}
