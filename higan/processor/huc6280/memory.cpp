auto HuC6280::mmu(uint16 addr) const -> uint21 {
  return r.mpr[addr.bits(13,15)] << 13 | addr.bits(0,12);
}

//

auto HuC6280::load8(uint8 addr) -> uint8 {
  step(r.cs);
  return read(mmu(0x2000 + addr));
}

auto HuC6280::load16(uint16 addr) -> uint8 {
  step(r.cs);
  return read(mmu(addr));
}

auto HuC6280::store8(uint8 addr, uint8 data) -> void {
  step(r.cs);
  return write(mmu(0x2000 + addr), data);
}

auto HuC6280::store16(uint16 addr, uint8 data) -> void {
  step(r.cs);
  return write(mmu(addr), data);
}

auto HuC6280::store21(uint21 addr, uint8 data) -> void {
  step(r.cs);
  return write(addr, data);
}

//

auto HuC6280::io() -> uint8 {
  step(r.cs);
  return read(mmu(PC));
}

auto HuC6280::opcode() -> uint8 {
  step(r.cs);
  return read(mmu(PC++));
}

auto HuC6280::operand() -> uint8 {
  step(r.cs);
  return read(mmu(PC++));
}

//

auto HuC6280::push(uint8 data) -> void {
  step(r.cs);
  write(mmu(0x2100 + S), data);
  S--;
}

auto HuC6280::pull() -> uint8 {
  step(r.cs);
  S++;
  return read(mmu(0x2100 + S));
}
