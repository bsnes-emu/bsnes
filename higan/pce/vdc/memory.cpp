auto VDC::VRAM::read(uint16 addr) -> uint16 {
  if(addr.bit(15)) return 0x0000;  //todo: random data?
  return data[addr];
}

auto VDC::VRAM::write(uint16 addr, uint16 value) -> void {
  if(addr.bit(15)) return;
  data[addr] = value;
}

auto VDC::SATB::read(uint8 addr) -> uint16 {
  return data[addr];
}

auto VDC::SATB::write(uint8 addr, uint16 value) -> void {
  data[addr] = value;
}
