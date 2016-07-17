auto M68K::testCondition(uint4 condition) -> bool {
  switch(condition) {
  case  0: return true;   //RA
  case  1: return false;  //NV,SR
  case  2: return !r.c && !r.z;  //HI
  case  3: return  r.c ||  r.z;  //LS
  case  4: return !r.c;  //CC,HS
  case  5: return  r.c;  //CS,LO
  case  6: return !r.z;  //NE
  case  7: return  r.z;  //EQ
  case  8: return !r.v;  //VC
  case  9: return  r.v;  //VS
  case 10: return !r.n;  //PL
  case 11: return  r.n;  //MI
  case 12: return  r.n ==  r.v;  //GE
  case 13: return  r.n !=  r.v;  //LT
  case 14: return  r.n ==  r.v && !r.z;  //GT
  case 15: return  r.n !=  r.v ||  r.z;  //LE
  }
  unreachable;
}

//

template<> auto M68K::clip<Byte>(uint32 data) -> uint32 { return data & 0xff; }
template<> auto M68K::clip<Word>(uint32 data) -> uint32 { return data & 0xffff; }
template<> auto M68K::clip<Long>(uint32 data) -> uint32 { return data & 0xffffffff; }

template<> auto M68K::sign<Byte>(uint32 data) -> int32 { return  (int8)data; }
template<> auto M68K::sign<Word>(uint32 data) -> int32 { return (int16)data; }
template<> auto M68K::sign<Long>(uint32 data) -> int32 { return (int32)data; }

template<uint Size> auto M68K::carry(uint32 result, uint32 source) -> bool {
  return clip<Size>(result) < clip<Size>(source);
}

template<uint Size> auto M68K::overflow(uint32 result, uint32 source, uint32 target) -> bool {
  return sign<Size>((target ^ source) & (target ^ result)) < 0;
}

template<uint Size> auto M68K::zero(uint32 result) -> bool {
  return clip<Size>(result) == 0;
}

template<uint Size> auto M68K::negative(uint32 result) -> bool {
  return sign<Size>(result) < 0;
}

auto M68K::zero(uint2 size, uint32 result) -> bool {
  static const uint32 mask[4] = {0xff, 0xffff, 0xffffffff};
  return (result & mask[size]) == 0;
}

auto M68K::negative(uint2 size, uint32 result) -> bool {
  static const uint32 mask[4] = {0x80, 0x8000, 0x80000000};
  return result & mask[size];
}

//

template<uint Size> auto M68K::instructionADD(uint3 reg, uint1 direction, EA ea) -> void {
  uint32 source;
  uint32 target;
  uint32 result;

  if(direction == 0) {
    source = read<Size>(ea);
    target = r.d(reg);
    result = source + target;
    r.d(reg) = result;
  } else {
    source = r.d(reg);
    target = read<Size>(ea);
    result = source + target;
    write<Size>(ea, result);
  }

  r.c = carry<Size>(result, source);
  r.v = overflow<Size>(result, source, target);
  r.z = zero<Size>(result);
  r.n = negative<Size>(result);
  r.x = r.c;
}

auto M68K::instructionANDI(uint8 ea) -> void {
  auto result = modify(ea, readPC(ea), [&](auto x, auto y) -> uint32 {
    return x & y;
  });

  r.c = 0;
  r.v = 0;
  r.z = zero(ea, result);
  r.n = negative(ea, result);
}

auto M68K::instructionBCC(uint4 condition, uint8 displacement) -> void {
  auto word = readPC();
  if(displacement) displacement = (int8)displacement, r.pc -= 2;
  else displacement = (int16)word;
  if(condition == 1) {
    condition = 0;
  //pushLong(r.pc);
  }
  if(testCondition(condition)) r.pc += displacement;
}

auto M68K::instructionLEA(uint3 target, uint8 ea) -> void {
  r.a(target) = address(ea);
}

auto M68K::instructionMOVE(uint8 to, uint8 from) -> void {
  auto data = read(from);
  write(to, data);

  r.c = 0;
  r.v = 0;
  r.z = zero(from, data);
  r.n = negative(from, data);
}

auto M68K::instructionMOVEA(uint3 to, uint8 from) -> void {
  auto data = read(from);
  if(from & 1) data = (int16)data;
  r.a(to) = data;
}

auto M68K::instructionMOVEM(uint1 direction, uint8 ea) -> void {
  auto list = readPC();
  auto addr = address(ea);

  for(uint n : range(8)) {
    if(list.bit(0 + n)) {
      r.d(n) = readAbsolute(ea, addr);
      addr += 2 + (ea & 2);
    }
  }

  for(uint n : range(8)) {
    if(list.bit(8 + n)) {
      r.a(n) = readAbsolute(ea, addr);
      addr += 2 + (ea & 2);
    }
  }

  flush(ea, addr);
}

auto M68K::instructionMOVEQ(uint3 target, uint8 immediate) -> void {
  r.d(target) = immediate;

  r.c = 0;
  r.v = 0;
  r.z = immediate == 0;
  r.n = negative(Byte, immediate);
}

auto M68K::instructionMOVE_USP(uint1 direction, uint3 reg) -> void {
  if(!r.s) trap();  //todo: proper trap
  if(direction == 0) {
    r.usp = r.a(reg);
  } else {
    r.a(reg) = r.usp;
  }
}

auto M68K::instructionNOP() -> void {
}

auto M68K::instructionTST(uint8 ea) -> void {
  auto data = read(ea);

  r.c = 0;
  r.v = 0;
  r.z = zero(ea, data);
  r.n = negative(ea, data);
}
