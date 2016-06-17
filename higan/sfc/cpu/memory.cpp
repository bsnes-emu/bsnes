auto CPU::portRead(uint2 port) const -> uint8 {
  return status.port[port];
}

auto CPU::portWrite(uint2 port, uint8 data) -> void {
  status.port[port] = data;
}

auto CPU::io() -> void {
  status.clockCount = 6;
  dmaEdge();
  addClocks(6);
  aluEdge();
}

auto CPU::read(uint24 addr) -> uint8 {
  status.clockCount = speed(addr);
  dmaEdge();
  addClocks(status.clockCount - 4);
  r.mdr = bus.read(addr, r.mdr);
  addClocks(4);
  aluEdge();
  debugger.read(addr, r.mdr);
  return r.mdr;
}

auto CPU::write(uint24 addr, uint8 data) -> void {
  aluEdge();
  status.clockCount = speed(addr);
  dmaEdge();
  addClocks(status.clockCount);
  bus.write(addr, r.mdr = data);
  debugger.write(addr, r.mdr);
}

auto CPU::speed(uint24 addr) const -> uint {
  if(addr & 0x408000) {
    if(addr & 0x800000) return status.romSpeed;
    return 8;
  }
  if((addr + 0x6000) & 0x4000) return 8;
  if((addr - 0x4000) & 0x7e00) return 6;
  return 12;
}

auto CPU::disassemblerRead(uint24 addr) -> uint8 {
  return bus.read(addr, r.mdr);
}
