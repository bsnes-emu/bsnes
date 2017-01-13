auto HuC6280::mmu(uint16 addr) const -> uint21 {
  return r.mpr[addr.bits(13,15)] << 13 | addr.bits(0,12);
}

//

auto HuC6280::load(uint16 addr) -> uint8 {
  step(r.cs);
  return read(mmu(addr));
}

auto HuC6280::store(uint16 addr, uint8 data) -> void {
  step(r.cs);
  return write(mmu(addr), data);
}

//

auto HuC6280::io() -> uint8 {
  return load(r.pc);
}

auto HuC6280::opcode() -> uint8 {
  return load(r.pc++);
}

auto HuC6280::operand() -> uint8 {
  return load(r.pc++);
}

//

auto HuC6280::push(uint8 data) -> void {
  store(0x2100 + ++S, data);
}

auto HuC6280::pull() -> uint8 {
  return load(0x2100 + S--);
}
