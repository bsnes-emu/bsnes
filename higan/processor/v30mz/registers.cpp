auto V30MZ::getRegister(Size size, uint8 modRM) -> uint16 {
  static uint8*  byte[] = {&r.al, &r.cl, &r.dl, &r.bl, &r.ah, &r.ch, &r.dh, &r.bh};
  static uint16* word[] = {&r.ax, &r.cx, &r.dx, &r.bx, &r.sp, &r.bp, &r.si, &r.di};
  if(size == Byte) return *byte[(modRM >> 3) & 7];
  if(size == Word) return *word[(modRM >> 3) & 7];
  throw;
}

auto V30MZ::setRegister(Size size, uint8 modRM, uint16 data) -> void {
  static uint8*  byte[] = {&r.al, &r.cl, &r.dl, &r.bl, &r.ah, &r.ch, &r.dh, &r.bh};
  static uint16* word[] = {&r.ax, &r.cx, &r.dx, &r.bx, &r.sp, &r.bp, &r.si, &r.di};
  if(size == Byte) *byte[(modRM >> 3) & 7] = data;
  if(size == Word) *word[(modRM >> 3) & 7] = data;
}

//

auto V30MZ::getSegment(uint8 modRM) -> uint16 {
  static uint16* word[] = {&r.es, &r.cs, &r.ss, &r.ds};
  return *word[(modRM >> 3) & 3];
}

auto V30MZ::setSegment(uint8 modRM, uint16 data) -> void {
  static uint16* word[] = {&r.es, &r.cs, &r.ss, &r.ds};
  *word[(modRM >> 3) & 3] = data;
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
