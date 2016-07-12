auto M68K::signExtend(uint2 size, uint32 data) -> int32 {
  if(size == 0) return  (int8)data;
  if(size == 1) return (int16)data;
  if(size == 2) return (int32)data;
  return 0;
}

auto M68K::readEA(uint2 size, uint3 mode, uint3 reg) -> uint32 {
  if(mode == 7) {
    if(reg == 0) {
      uint32 addr = (int16)readWordPC();
      return readAbsolute(size, addr);
    }

    if(reg == 1) {
      uint32 addr = readLongPC();
      return readAbsolute(size, addr);
    }
  }

  return 0;
}

auto M68K::writeEA(uint2 size, uint3 mode, uint3 reg, uint32 data) -> void {
}
