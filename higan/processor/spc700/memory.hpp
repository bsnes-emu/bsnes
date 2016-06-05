alwaysinline auto readPC() -> uint8 {
  return read(regs.pc++);
}

alwaysinline auto readSP() -> uint8 {
  return read(0x0100 | ++regs.s);
}

alwaysinline auto writeSP(uint8 data) -> void {
  return write(0x0100 | regs.s--, data);
}

alwaysinline auto readDP(uint8 addr) -> uint8 {
  return read(regs.p.p << 8 | addr);
}

alwaysinline auto writeDP(uint8 addr, uint8 data) -> void {
  return write(regs.p.p << 8 | addr, data);
}
