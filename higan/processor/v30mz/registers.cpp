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

auto V30MZ::Registers::byte(uint3 r) -> uint8& {
  switch(r) {
  case 0: return al;
  case 1: return cl;
  case 2: return dl;
  case 3: return bl;
  case 4: return ah;
  case 5: return ch;
  case 6: return dh;
  case 7: return bh;
  } unreachable;
}

auto V30MZ::Registers::word(uint3 r) -> uint16& {
  switch(r) {
  case 0: return ax;
  case 1: return cx;
  case 2: return dx;
  case 3: return bx;
  case 4: return sp;
  case 5: return bp;
  case 6: return si;
  case 7: return di;
  } unreachable;
}

auto V30MZ::Registers::segment(uint2 r) -> uint16& {
  switch(r) {
  case 0: return es;
  case 1: return cs;
  case 2: return ss;
  case 3: return ds;
  } unreachable;
}

//

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
