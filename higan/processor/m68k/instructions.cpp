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

//

template<uint Size> auto M68K::instructionADD(Register rd, uint1 direction, EA ea) -> void {
  uint32 source;
  uint32 target;
  uint32 result;

  if(direction == 0) {
    source = read<Size>(ea);
    target = read<Size>(rd);
    result = source + target;
    write<Size>(rd, result);
  } else {
    source = read<Size>(rd);
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

template<uint Size> auto M68K::instructionANDI(EA ea) -> void {
  auto source = readPC<Size>();
  auto target = read<Size, NoUpdate>(ea);
  auto result = target & source;
  write<Size>(ea, result);

  r.c = 0;
  r.v = 0;
  r.z = zero<Size>(result);
  r.n = negative<Size>(result);
}

auto M68K::instructionBCC(uint4 condition, uint8 displacement) -> void {
  auto extension = readPC();
  if(condition == 1);  //push<Long>(r.pc);
  r.pc -= 2;
  if(!testCondition(condition == 1 ? (uint4)0 : condition)) return;
  r.pc += displacement ? sign<Byte>(displacement) : sign<Word>(extension);
}

auto M68K::instructionLEA(Register ra, EA ea) -> void {
  write<Long>(ra, fetch<Long>(ea));
}

template<uint Size> auto M68K::instructionMOVE(EA to, EA from) -> void {
  auto data = read<Size>(from);
  write<Size>(to, data);

  r.c = 0;
  r.v = 0;
  r.z = zero<Size>(data);
  r.n = negative<Size>(data);
}

template<uint Size> auto M68K::instructionMOVEA(Register ra, EA ea) -> void {
  auto data = read<Size>(ea);
  if(Size == Word) data = (int16)data;
  write<Size>(ra, data);
}

template<uint Size> auto M68K::instructionMOVEM(uint1 direction, EA ea) -> void {
  auto list = readPC();
  auto addr = fetch<Size>(ea);

  for(uint rn : range(16)) {
    if(list.bit(rn)) {
      write<Size>(Register{rn}, read<Size>(addr));
      addr += Size == Long ? 4 : 2;
    }
  }

  flush<Size>(ea, addr);
}

auto M68K::instructionMOVEQ(Register rd, uint8 immediate) -> void {
  write<Byte>(rd, immediate);

  r.c = 0;
  r.v = 0;
  r.z = zero<Byte>(immediate);
  r.n = negative<Byte>(immediate);
}

auto M68K::instructionMOVE_USP(uint1 direction, Register ra) -> void {
  if(!r.s) trap();  //todo: proper trap
  if(direction == 0) {
    r.usp = read<Long>(ra);
  } else {
    write<Long>(ra, r.usp);
  }
}

auto M68K::instructionNOP() -> void {
}

template<uint Size> auto M68K::instructionTST(EA ea) -> void {
  auto data = read<Size>(ea);

  r.c = 0;
  r.v = 0;
  r.z = zero<Size>(data);
  r.n = negative<Size>(data);
}
