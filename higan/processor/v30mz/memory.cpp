auto V30MZ::readbIP() -> uint8 {
  return read((r.cs << 4) + r.ip++);
}

auto V30MZ::readwIP() -> uint16 {
  uint16 word = read((r.cs << 4) + r.ip++) << 0;
  return word | read((r.cs << 4) + r.ip++) << 8;
}

//

auto V30MZ::readSP() -> uint16 {
  uint16 word = read((r.ss << 4) + r.sp++) << 0;
  return word | read((r.ss << 4) + r.sp++) << 8;
}

auto V30MZ::writeSP(uint16 data) -> void {
  write((r.ss << 4) + --r.sp, data >> 8);
  write((r.ss << 4) + --r.sp, data >> 0);
}

//

auto V30MZ::readb(uint20 ea) -> uint8 {
  return read(ea++);
}

auto V30MZ::readw(uint20 ea) -> uint16 {
  uint16 word = read(ea++) << 0;
  return word | read(ea++) << 8;
}

//

auto V30MZ::readb(uint16 rs, uint16 ea) -> uint8 {
  return read((rs << 4) + ea++);
}

auto V30MZ::readw(uint16 rs, uint16 ea) -> uint16 {
  uint16 word = read((rs << 4) + ea++) << 0;
  return word | read((rs << 4) + ea++) << 8;
}

//

//todo: return tuple<uint16 seg, uint16 ea>
auto V30MZ::readModRM(uint8 modRM) -> uint20 {
  if((modRM & 0xc7) == 0x06) return (r.ds << 4) + (int16)readwIP();

  int16 displacement = 0;
  if((modRM & 0xc0) == 0x40) displacement = (int8)readbIP();
  if((modRM & 0xc0) == 0x80) displacement = (int16)readwIP();

  switch(modRM & 7) {
  case 0: return (r.ds << 4) + r.bx + r.si + displacement;
  case 1: return (r.ds << 4) + r.bx + r.di + displacement;
  case 2: return (r.ss << 4) + r.bp + r.si + displacement;
  case 3: return (r.ss << 4) + r.bp + r.di + displacement;
  case 4: return (r.ds << 4) + r.si + displacement;
  case 5: return (r.ds << 4) + r.di + displacement;
  case 6: return (r.ss << 4) + r.bp + displacement;
  case 7: return (r.ds << 4) + r.bx + displacement;
  }
  unreachable;
}

auto V30MZ::readbModRM(uint8 modRM) -> uint8 {
  if(modRM >= 0xc0) return regb(modRM);
  return readb(readModRM(modRM));
}

auto V30MZ::readwModRM(uint8 modRM) -> uint16 {
  if(modRM >= 0xc0) return regw(modRM);
  return readw(readModRM(modRM));
}
