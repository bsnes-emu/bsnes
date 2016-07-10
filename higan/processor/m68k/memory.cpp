auto M68K::readByte(uint32 addr) -> uint8 {
  return read(addr);
}

auto M68K::readWord(uint32 addr) -> uint16 {
  uint16 data;
  data |= read(addr + 0) << 8;
  data |= read(addr + 1) << 0;
  return data;
}

auto M68K::readLong(uint32 addr) -> uint32 {
  uint32 data;
  data |= read(addr + 0) << 24;
  data |= read(addr + 1) << 16;
  data |= read(addr + 2) <<  8;
  data |= read(addr + 3) <<  0;
  return data;
}

auto M68K::readQuad(uint32 addr) -> uint64 {
  uint64 data;
  data |= (uint64)read(addr + 0) << 56;
  data |= (uint64)read(addr + 1) << 48;
  data |= (uint64)read(addr + 2) << 40;
  data |= (uint64)read(addr + 3) << 32;
  data |= (uint64)read(addr + 4) << 24;
  data |= (uint64)read(addr + 5) << 16;
  data |= (uint64)read(addr + 6) <<  8;
  data |= (uint64)read(addr + 7) <<  0;
  return data;
}
