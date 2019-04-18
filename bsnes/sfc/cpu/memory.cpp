auto CPU::idle() -> void {
  status.clockCount = 6;
  dmaEdge();
  stepIdle(6);
  aluEdge();
}

auto CPU::read(uint24 address) -> uint8 {
  status.clockCount = wait(address);
  dmaEdge();
  r.mar = address;
  step(status.clockCount - 4);
  auto data = bus.read(address, r.mdr);
  step(4);
  aluEdge();
  //$00-3f,80-bf:4000-43ff reads are internal to CPU, and do not update the MDR
  if((address & 0x40fc00) != 0x4000) r.mdr = data;
  return data;
}

auto CPU::write(uint24 address, uint8 data) -> void {
  aluEdge();
  status.clockCount = wait(address);
  dmaEdge();
  r.mar = address;
  step(status.clockCount);
  bus.write(address, r.mdr = data);
}

auto CPU::wait(uint24 address) const -> uint {
  if(address & 0x408000) return address & 0x800000 ? io.romSpeed : 8;
  if(address + 0x6000 & 0x4000) return 8;
  if(address - 0x4000 & 0x7e00) return 6;
  return 12;
}

auto CPU::readDisassembler(uint24 address) -> uint8 {
  return bus.read(address, r.mdr);
}
