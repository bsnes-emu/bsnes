auto CPU::ramRead(uint addr) -> uint8 {
  uint mask = system.monochrome() ? 0x3fff : 0xffff;
  return iram[addr & mask];
}

auto CPU::ramWrite(uint addr, uint8 data) -> void {
  uint mask = system.monochrome() ? 0x3fff : 0xffff;
  iram[addr & mask] = data;
}
