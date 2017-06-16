auto SPC700::fetch() -> uint8 {
  return read(PC++);
}

auto SPC700::pull() -> uint8 {
  return read(0x0100 | ++S);
}

auto SPC700::push(uint8 data) -> void {
  return write(0x0100 | S--, data);
}

auto SPC700::load(uint8 addr) -> uint8 {
  return read(PF << 8 | addr);
}

auto SPC700::store(uint8 addr, uint8 data) -> void {
  return write(PF << 8 | addr, data);
}
