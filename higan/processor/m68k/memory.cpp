auto M68K::readByte(uint32 addr) -> uint8 {
  step(4);
  return read(addr);
}

auto M68K::readWord(uint32 addr) -> uint16 {
  step(4);
  uint16 data  = read(addr + 0) << 8;
  return data |= read(addr + 1) << 0;
}

auto M68K::readLong(uint32 addr) -> uint32 {
  uint32 data  = readWord(addr + 0) << 16;
  return data |= readWord(addr + 2) <<  0;
}

//

auto M68K::readWordPC() -> uint16 {
  uint16 data = readWord(r.pc);
  r.pc += 2;
  return data;
}

auto M68K::readLongPC() -> uint32 {
  uint32 data  = readWordPC() << 16;
  return data |= readWordPC() <<  0;
}

//

auto M68K::readAbsolute(uint2 size, uint32 addr) -> uint32 {
  if(size == 0) return readByte(addr);
  if(size == 1) return readWord(addr);
  if(size == 2) return readLong(addr);
  return 0;
}
