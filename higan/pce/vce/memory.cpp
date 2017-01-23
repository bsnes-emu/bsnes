auto VCE::CRAM::read(uint9 addr) -> uint9 {
  return data[addr];
}

auto VCE::CRAM::write(uint9 addr, bool a0, uint8 value) -> void {
  if(!a0) {
    data[addr].bits(0,7) = value.bits(0,7);
  } else {
    data[addr].bit(8) = value.bit(0);
  }
}
