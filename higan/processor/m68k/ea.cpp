auto M68K::signExtend(uint2 size, uint32 data) -> int32 {
  if(size == 0) return  (int8)data;
  if(size == 1) return (int16)data;
  if(size == 2) return (int32)data;
  return 0;
}

//

auto M68K::address(EA& ea) -> uint32 {
  if(ea.valid) return ea.address;
  ea.valid = true;

  if(ea.mode == 0) return ea.address = r.d(ea.reg);
  if(ea.mode == 1) return ea.address = r.a(ea.reg);

  if(ea.mode == 7) {
    if(ea.reg == 0) return ea.address = (int16)readWordPC();
    if(ea.reg == 1) return ea.address = readLongPC();
    if(ea.reg == 2) return ea.address = r.pc, ea.address += (int16)readWordPC();
  }

  return ea.address = 0;
}

auto M68K::read(EA& ea) -> uint32 {
  address(ea);
  if(ea.mode < 2 || (ea.mode == 7 && ea.reg == 4)) return ea.address;
  return readAbsolute(ea.size, ea.address);
}

auto M68K::write(EA& ea, uint32 data) -> void {
  address(ea);
}
