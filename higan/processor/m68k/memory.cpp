auto M68K::readAbsolute(uint2 size, uint32 addr) -> uint32 {
  step(4);
  uint32 data = read(size != Byte, addr);
  if(size != Long) return data;

  step(4);
  data = data << 16 | read(1, addr + 2);
  return data;
}

auto M68K::writeAbsolute(uint2 size, uint32 addr, uint32 data) -> void {
  if(size == Long) {
    write(1, addr + 0, data >> 16);
    write(1, addr + 2, data >>  0);
  } else {
    write(size != Byte, addr, data >> 0);
  }
}

//

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
