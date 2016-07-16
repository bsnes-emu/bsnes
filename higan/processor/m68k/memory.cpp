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

auto M68K::read(uint size, uint32 addr) -> uint32 {
  if(size == Byte) return readByte(addr);
  if(size == Word) return readWord(addr);
  if(size == Long) return readLong(addr);
  return 0;
}

auto M68K::readPC(uint size) -> uint32 {
  uint32 data = readWord(r.pc);
  r.pc += 2;
  if(size == Byte) return (uint8)data;
  if(size == Word) return data;
  data = data << 16 | readWord(r.pc);
  r.pc += 2;
  return data;
}
