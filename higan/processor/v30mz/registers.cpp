auto V30MZ::segment(uint16 segment) -> uint16 {
  if(prefix.segment) return prefix.segment();
  return segment;
}

auto V30MZ::getAcc(Size size) -> uint32 {
  if(size == Byte) return r.al;
  if(size == Word) return r.ax;
  if(size == Long) return r.dx << 16 | r.ax;
  unreachable;
}

auto V30MZ::setAcc(Size size, uint32 data) -> void {
  if(size == Byte) r.al = data;
  if(size == Word) r.ax = data;
  if(size == Long) r.ax = data, r.dx = data >> 16;
}

//

V30MZ::Registers::Flags::operator uint16_t() const {
  return m << 15 | 1 << 14 | 1 << 13 | 1 << 12
       | v << 11 | d << 10 | i <<  9 | b <<  8
       | s <<  7 | z <<  6 | h <<  4 | p <<  2
       | 1 <<  1 | c <<  0;
}

auto V30MZ::Registers::Flags::operator=(uint16_t data) {
  m = (uint1)(data >> 15);
  v = (uint1)(data >> 11);
  d = (uint1)(data >> 10);
  i = (uint1)(data >>  9);
  b = (uint1)(data >>  8);
  s = (uint1)(data >>  7);
  z = (uint1)(data >>  6);
  h = (uint1)(data >>  4);
  p = (uint1)(data >>  2);
  c = (uint1)(data >>  0);
  return *this;
}
