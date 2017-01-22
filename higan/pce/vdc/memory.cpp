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

auto VDC::CRAM::read(uint9 addr) -> uint9 {
  return data[addr];
}

auto VDC::CRAM::write(uint9 addr, bool a0, uint8 value) -> void {
  if(!a0) {
    data[addr].bits(0,7) = value.bits(0,7);
  } else {
    data[addr].bit(8) = value.bit(0);
  }
}
