//ModRM functions
//d7-d6 => mod
//d5-d3 => reg
//d2-d0 => mem

auto V30MZ::getReg(Size size, uint8 modRM) -> uint16 {
  if(size == Byte) return r.byte(modRM >> 3);
  if(size == Word) return r.word(modRM >> 3);
  unreachable;
}

auto V30MZ::setReg(Size size, uint8 modRM, uint16 data) -> void {
  if(size == Byte) r.byte(modRM >> 3) = data;
  if(size == Word) r.word(modRM >> 3) = data;
}

//

auto V30MZ::getSeg(uint8 modRM) -> uint16 {
  return r.segment(modRM >> 3);
}

auto V30MZ::setSeg(uint8 modRM, uint16 data) -> void {
  r.segment(modRM >> 3) = data;
}

//

auto V30MZ::getMemAddress(uint8 modRM) -> uint32 {
  if((modRM & 0xc7) == 0x06) return r.ds << 16 | fetch(Word);

  uint16 s = 0, a = 0;
  if((modRM & 0xc0) == 0x40) a = (int8)fetch(Byte);
  if((modRM & 0xc0) == 0x80) a = (int16)fetch(Word);

  switch(modRM & 7) {
  case 0: s = r.ds; a += r.bx + r.si; break;
  case 1: s = r.ds; a += r.bx + r.di; break;
  case 2: s = r.ss; a += r.bp + r.si; break;
  case 3: s = r.ss; a += r.bp + r.di; break;
  case 4: s = r.ds; a += r.si; break;
  case 5: s = r.ds; a += r.di; break;
  case 6: s = r.ss; a += r.bp; break;
  case 7: s = r.ds; a += r.bx; break;
  }

  return s << 16 | a;
}

auto V30MZ::getMem(Size size, uint8 modRM) -> uint16 {
  if(modRM >= 0xc0) return getReg(size, modRM << 3);
  auto addr = getMemAddress(modRM);
  return read(size, addr >> 16, addr);
}

auto V30MZ::setMem(Size size, uint8 modRM, uint16 data) -> void {
  if(modRM >= 0xc0) return setReg(size, modRM << 3, data);
  auto addr = getMemAddress(modRM);
  return write(size, addr >> 16, addr, data);
}
