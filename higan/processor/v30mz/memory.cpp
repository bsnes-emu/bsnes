auto V30MZ::read(Size size, uint16 segment, uint16 address) -> uint16 {
  uint16 data = read(segment * 16 + address);
  if(size == Word) data |= read(segment * 16 + ++address) << 8;
  return data;
}

auto V30MZ::write(Size size, uint16 segment, uint16 address, uint16 data) -> void {
  write(segment * 16 + address, data);
  if(size == Word) write(segment * 16 + ++address, data >> 8);
}

//

auto V30MZ::readIP(Size size) -> uint16 {
  uint16 data = read(size, r.cs, r.ip);
  return r.ip += size, data;
}

//

auto V30MZ::readSP() -> uint16 {
  uint16 data = read(Word, r.ss, r.sp);
  return r.sp += Word, data;
}

auto V30MZ::writeSP(uint16 data) -> void {
  r.sp -= Word;
  write(Word, r.ss, r.sp, data);
}

//

auto V30MZ::readModRM(uint8 modRM) -> uint32 {
  if((modRM & 0xc7) == 0x06) return r.ds << 16 | readIP(Word);

  uint16 s = 0, a = 0;
  if((modRM & 0xc0) == 0x40) a = (int8)readIP(Byte);
  if((modRM & 0xc0) == 0x80) a = (int16)readIP(Word);

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

auto V30MZ::readModRM(Size size, uint8 modRM) -> uint16 {
  if(modRM >= 0xc0) return getRegister(size, modRM);
  auto addr = readModRM(modRM);
  return read(size, addr >> 16, addr);
}
