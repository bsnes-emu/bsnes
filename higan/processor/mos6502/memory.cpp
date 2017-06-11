auto MOS6502::idle() -> void {
  read(PC);
}

auto MOS6502::idlePageCrossed(uint16 x, uint16 y) -> void {
  if(x >> 8 == y >> 8) return;
  read((x & 0xff00) | (y & 0x00ff));
}

auto MOS6502::idlePageAlways(uint16 x, uint16 y) -> void {
  read((x & 0xff00) | (y & 0x00ff));
}

auto MOS6502::opcode() -> uint8 {
  return read(PC++);
}

auto MOS6502::operand() -> uint8 {
  return read(PC++);
}

auto MOS6502::load(uint8 addr) -> uint8 {
  return read(addr);
}

auto MOS6502::store(uint8 addr, uint8 data) -> void {
  write(addr, data);
}

auto MOS6502::push(uint8 data) -> void {
  write(0x0100 | S--, data);
}

auto MOS6502::pull() -> uint8 {
  return read(0x0100 | ++S);
}
