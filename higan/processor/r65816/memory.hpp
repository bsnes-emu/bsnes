alwaysinline auto readPC() -> uint8 {
  return read(r.pc.b << 16 | uint16(r.pc.w++));
}

alwaysinline auto readSP() -> uint8 {
  r.e ? r.s.l++ : r.s.w++;
  return read(r.s.w);
}

alwaysinline auto readSPn() -> uint8 {
  return read(++r.s.w);
}

alwaysinline auto readAddr(uint addr) -> uint8 {
  return read(uint16(addr));
}

alwaysinline auto readLong(uint addr) -> uint8 {
  return read(uint24(addr));
}

alwaysinline auto readDB(uint addr) -> uint8 {
  return read(r.db << 16 | uint16(addr));
}

alwaysinline auto readPB(uint addr) -> uint8 {
  return read(r.pc.b << 16 | uint16(addr));
}

alwaysinline auto readDP(uint addr) -> uint8 {
  if(r.e && r.d.l == 0x00) {
    return read(r.d | uint8(addr));
  } else {
    return read(uint16(r.d + addr));
  }
}

alwaysinline auto readDPn(uint addr) -> uint8 {
  return read(uint16(r.d + addr));
}

alwaysinline auto readSP(uint addr) -> uint8 {
  return read(uint16(r.s + addr));
}

alwaysinline auto writeSP(uint8 data) -> void {
  write(r.s.w, data);
  r.e ? r.s.l-- : r.s.w--;
}

alwaysinline auto writeSPn(uint8 data) -> void {
  write(r.s.w--, data);
}

alwaysinline auto writeAddr(uint addr, uint8 data) -> void {
  write(uint16(addr), data);
}

alwaysinline auto writeLong(uint addr, uint8 data) -> void {
  write(uint24(addr), data);
}

alwaysinline auto writeDB(uint addr, uint8 data) -> void {
  write(r.db << 16 | uint16(addr), data);
}

alwaysinline auto writePB(uint addr, uint8 data) -> void {
  write(r.pc.b << 16 | uint16(addr), data);
}

alwaysinline auto writeDP(uint addr, uint8 data) -> void {
  if(r.e && r.d.l == 0x00) {
    write(r.d | uint8(addr), data);
  } else {
    write(uint16(r.d + addr), data);
  }
}

alwaysinline auto writeSP(uint addr, uint8 data) -> void {
  write(uint16(r.s + addr), data);
}
