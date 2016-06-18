auto R6502::io() -> uint8 {
  return read(regs.pc);
}

auto R6502::readPC() -> uint8 {
  return read(regs.pc++);
}

auto R6502::readSP() -> uint8 {
  return read(0x0100 | ++regs.s);
}

auto R6502::readZP(uint8 addr) -> uint8 {
  return read(addr);
}

//

auto R6502::writeSP(uint8 data) -> void {
  write(0x0100 | regs.s--, data);
}

auto R6502::writeZP(uint8 addr, uint8 data) -> void {
  write(addr, data);
}

//

auto R6502::ioPage(uint16 x, uint16 y) -> void {
  if(x >> 8 == y >> 8) return;
  read((x & 0xff00) | (y & 0x00ff));
}

auto R6502::ioPageAlways(uint16 x, uint16 y) -> void {
  read((x & 0xff00) | (y & 0x00ff));
}
