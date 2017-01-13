auto HuC6280::io() -> uint8 {
  return read(r.pc);
}

auto HuC6280::opcode() -> uint8 {
  return read(r.pc++);
}

auto HuC6280::operand() -> uint8 {
  return read(r.pc++);
}
