/*
auto M68K::readPC(uint2 size) -> uint32 {
  step(4);
  uint32 data = read(size != Byte, r.pc);
  r.pc += 2;
  if(size != Long) return data;

  step(4);
  data = data << 16 | read(1, r.pc);
  r.pc += 2;
  return data;
}
*/

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
}

template<> auto M68K::write<Word>(uint32 addr, uint32 data) -> void {
}

template<> auto M68K::write<Long>(uint32 addr, uint32 data) -> void {
}

//

template<> auto M68K::readPC<Byte>() -> uint32 {
  step(4);
  uint32 data = read(1, r.pc);
  r.pc += 2;
  return (uint8)data;
}

template<> auto M68K::readPC<Word>() -> uint32 {
  step(4);
  uint32 data = read(1, r.pc);
  r.pc += 2;
  return data;
}

template<> auto M68K::readPC<Long>() -> uint32 {
  step(4);
  uint32 data = read(1, r.pc) << 16;
  r.pc += 2;
  step(4);
  data |= read(1, r.pc);
  r.pc += 2;
  return data;
}
