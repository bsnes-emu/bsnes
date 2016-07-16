auto M68K::sign(uint size, uint32 data) -> int32 {
  if(size == Byte) return  (int8)data;
  if(size == Word) return (int16)data;
  if(size == Long) return (int32)data;
  return 0;
}

//

auto M68K::address(uint size, EA& ea) -> uint32 {
  if(ea.valid) return ea.address;
  ea.valid = true;

  if(ea.mode == 0) return ea.address = r.d(ea.reg);
  if(ea.mode == 1) return ea.address = r.a(ea.reg);
  if(ea.mode == 2) return ea.address = r.a(ea.reg);
  if(ea.mode == 3) return ea.address = r.a(ea.reg);
  if(ea.mode == 4) return ea.address = r.a(ea.reg);
  if(ea.mode == 5) return ea.address = r.a(ea.reg) + (int16)readPC(Word);
  if(ea.mode == 7) {
    if(ea.reg == 0) return ea.address = (int16)readPC(Word);
    if(ea.reg == 1) return ea.address = readPC(Long);
    if(ea.reg == 2) return ea.address = r.pc, ea.address += (int16)readPC(Word);
    if(ea.reg == 4) {
      if(size == Byte) return ea.address = readPC(Byte);
      if(size == Word) return ea.address = readPC(Word);
      if(size == Long) return ea.address = readPC(Long);
    }
  }

  return ea.address = 0;
}

auto M68K::read(uint size, EA& ea) -> uint32 {
  address(size, ea);

  if(ea.mode == 0) return r.d(ea.reg);
  if(ea.mode == 1) return r.a(ea.reg);
  if(ea.mode == 2) return read(size, ea.address);
  if(ea.mode == 3) {
    auto data = read(size, ea.address);
    ea.address += size, r.a(ea.reg) += size;
    return data;
  }
  if(ea.mode == 4) {
    ea.address -= size, r.a(ea.reg) -= size;
    return read(size, ea.address);
  }
  if(ea.mode == 5) return read(size, ea.address);
  if(ea.mode == 7) {
    if(ea.reg == 0) return read(size, ea.address);
    if(ea.reg == 1) return read(size, ea.address);
    if(ea.reg == 2) return read(size, ea.address);
    if(ea.reg == 4) return ea.address;
  }

  return 0;
}

auto M68K::write(uint size, EA& ea, uint32 data) -> void {
  address(size, ea);

  if(ea.mode == 0) {
    r.d(ea.reg) = data;
    return;
  }

  if(ea.mode == 1) {
    r.a(ea.reg) = data;
    return;
  }
}
