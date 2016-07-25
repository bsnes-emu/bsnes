template<> auto M68K::read<Byte>(uint32 addr) -> uint32 {
  step(4);
  return read(0, addr);
}

template<> auto M68K::read<Word>(uint32 addr) -> uint32 {
  step(4);
  return read(1, addr);
}

template<> auto M68K::read<Long>(uint32 addr) -> uint32 {
  step(4);
  uint32 data = read(1, addr + 0) << 16;
  step(4);
  return data | read(1, addr + 2) <<  0;
}

//

template<> auto M68K::write<Byte>(uint32 addr, uint32 data) -> void {
  step(4);
  return write(0, addr, data);
}

template<> auto M68K::write<Word>(uint32 addr, uint32 data) -> void {
  step(4);
  return write(1, addr, data);
}

template<> auto M68K::write<Long>(uint32 addr, uint32 data) -> void {
  step(4);
  write(1, addr + 0, data >> 16);
  step(4);
  write(1, addr + 2, data >>  0);
}

template<> auto M68K::write<Byte, Reverse>(uint32 addr, uint32 data) -> void {
  step(4);
  return write(0, addr, data);
}

template<> auto M68K::write<Word, Reverse>(uint32 addr, uint32 data) -> void {
  step(4);
  return write(1, addr, data);
}

template<> auto M68K::write<Long, Reverse>(uint32 addr, uint32 data) -> void {
  step(4);
  write(1, addr + 2, data >>  0);
  step(4);
  write(1, addr + 0, data >> 16);
}

//

template<> auto M68K::readPC<Byte>() -> uint32 {
  step(4);
  auto data = read(1, r.pc);
  r.pc += 2;
  return (uint8)data;
}

template<> auto M68K::readPC<Word>() -> uint32 {
  step(4);
  auto data = read(1, r.pc);
  r.pc += 2;
  return data;
}

template<> auto M68K::readPC<Long>() -> uint32 {
  step(4);
  auto hi = read(1, r.pc);
  r.pc += 2;
  step(4);
  auto lo = read(1, r.pc);
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
