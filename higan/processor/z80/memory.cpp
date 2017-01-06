auto Z80::wait(uint clocks) -> void {
  step(clocks);
}

auto Z80::opcode() -> uint8 {
  step(4);
  return bus->read(r.pc++);
}

auto Z80::operand() -> uint8 {
  step(3);
  return bus->read(r.pc++);
}

auto Z80::operands() -> uint16 {
  uint16 data = operand() << 0;
  return data | operand() << 8;
}

auto Z80::push(uint16 x) -> void {
  write(--SP, x >> 8);
  write(--SP, x >> 0);
}

auto Z80::pop() -> uint16 {
  uint16 data = read(SP++) << 0;
  return data | read(SP++) << 8;
}

auto Z80::displace(uint16& x) -> uint16 {
  if(&x != &r.ix.word && &x != &r.iy.word) return x;
  auto d = operand();
  wait(5);
  return x + (int8)d;
}

auto Z80::read(uint16 addr) -> uint8 {
  step(3);
  return bus->read(addr);
}

auto Z80::write(uint16 addr, uint8 data) -> void {
  step(3);
  return bus->write(addr, data);
}

auto Z80::in(uint8 addr) -> uint8 {
  step(4);
  return bus->in(addr);
}

auto Z80::out(uint8 addr, uint8 data) -> void {
  step(4);
  return bus->out(addr, data);
}
