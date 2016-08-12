template<> auto M68K::read<Byte>(uint32 addr) -> uint32 {
  step(4);
  return readByte(addr);
}

template<> auto M68K::read<Word>(uint32 addr) -> uint32 {
  step(4);
  return readWord(addr);
}

template<> auto M68K::read<Long>(uint32 addr) -> uint32 {
  step(4);
  uint32 data = readWord(addr + 0) << 16;
  step(4);
  return data | readWord(addr + 2) <<  0;
}

//

template<> auto M68K::write<Byte>(uint32 addr, uint32 data) -> void {
  step(4);
  return writeByte(addr, data);
}

template<> auto M68K::write<Word>(uint32 addr, uint32 data) -> void {
  step(4);
  return writeWord(addr, data);
}

template<> auto M68K::write<Long>(uint32 addr, uint32 data) -> void {
  step(4);
  writeWord(addr + 0, data >> 16);
  step(4);
  writeWord(addr + 2, data >>  0);
}

template<> auto M68K::write<Byte, Reverse>(uint32 addr, uint32 data) -> void {
  step(4);
  return writeByte(addr, data);
}

template<> auto M68K::write<Word, Reverse>(uint32 addr, uint32 data) -> void {
  step(4);
  return writeWord(addr, data);
}

template<> auto M68K::write<Long, Reverse>(uint32 addr, uint32 data) -> void {
  step(4);
  writeWord(addr + 2, data >>  0);
  step(4);
  writeWord(addr + 0, data >> 16);
}

//

template<> auto M68K::readPC<Byte>() -> uint32 {
  step(4);
  auto data = readWord(r.pc);
  r.pc += 2;
  return (uint8)data;
}

template<> auto M68K::readPC<Word>() -> uint32 {
  step(4);
  auto data = readWord(r.pc);
  r.pc += 2;
  return data;
}

template<> auto M68K::readPC<Long>() -> uint32 {
  step(4);
  auto hi = readWord(r.pc);
  r.pc += 2;
  step(4);
  auto lo = readWord(r.pc);
  r.pc += 2;
  return hi << 16 | lo << 0;
}

//

template<uint Size> auto M68K::pop() -> uint32 {
  auto data = read<Size>((uint32)r.a[7]);
  r.a[7] += bytes<Size>();
  return data;
}

template<uint Size> auto M68K::push(uint32 data) -> void {
  r.a[7] -= bytes<Size>();
  return write<Size, Reverse>((uint32)r.a[7], data);
}
