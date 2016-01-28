V30MZ::Registers::Flags::operator uint16() const {
  return m << 15 | 1 << 14 | 1 << 13 | 1 << 12
       | v << 11 | d << 10 | i <<  9 | b <<  8
       | s <<  7 | z <<  6 | h <<  4 | p <<  2
       | 1 <<  1 | c <<  0;
}

auto V30MZ::Registers::Flags::operator=(uint16 data) {
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
