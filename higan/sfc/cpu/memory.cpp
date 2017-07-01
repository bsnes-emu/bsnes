auto CPU::idle() -> void {
  status.clockCount = 6;
  dmaEdge();
  step(6);
  aluEdge();
}

auto CPU::read(uint24 addr) -> uint8 {
  status.clockCount = speed(addr);
  dmaEdge();
  step(status.clockCount - 4);
  auto data = bus.read(addr, r.mdr);
  step(4);
  aluEdge();
  //$00-3f,80-bf:4000-43ff reads are internal to CPU, and do not update the MDR
  if((addr & 0x40fc00) != 0x4000) r.mdr = data;
  return data;
}

auto CPU::write(uint24 addr, uint8 data) -> void {
  aluEdge();
  status.clockCount = speed(addr);
  dmaEdge();
  step(status.clockCount);
  bus.write(addr, r.mdr = data);
}

auto CPU::speed(uint24 addr) const -> uint {
  if(addr & 0x408000) return addr & 0x800000 ? io.romSpeed : 8;
  if(addr + 0x6000 & 0x4000) return 8;
  if(addr - 0x4000 & 0x7e00) return 6;
  return 12;
}

auto CPU::readDisassembler(uint24 addr) -> uint8 {
  return bus.read(addr, r.mdr);
}
