auto HuC6280::load8(uint8 addr) -> uint8 {
  step(r.cs);
  return read(r.mpr[1], addr);
}

auto HuC6280::load16(uint16 addr) -> uint8 {
  step(r.cs);
  return read(r.mpr[addr.bits(13,15)], addr.bits(0,12));
}

auto HuC6280::store8(uint8 addr, uint8 data) -> void {
  step(r.cs);
  return write(r.mpr[1], addr, data);
}

auto HuC6280::store16(uint16 addr, uint8 data) -> void {
  step(r.cs);
  return write(r.mpr[addr.bits(13,15)], addr.bits(0,12), data);
}

//

auto HuC6280::io() -> uint8 {
  step(r.cs);
  return 0xff;
}

auto HuC6280::opcode() -> uint8 {
  return load16(PC++);
}

auto HuC6280::operand() -> uint8 {
  return load16(PC++);
}

//

auto HuC6280::push(uint8 data) -> void {
  step(r.cs);
  write(r.mpr[1], 0x0100 | S, data);
  S--;
}

auto HuC6280::pull() -> uint8 {
  step(r.cs);
  S++;
  return read(r.mpr[1], 0x0100 | S);
}
