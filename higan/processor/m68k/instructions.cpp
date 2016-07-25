auto M68K::testCondition(uint4 condition) -> bool {
  switch(condition) {
  case  0: return true;   //T
  case  1: return false;  //F
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

template<> auto M68K::bytes<Byte>() -> uint { return 1; }
template<> auto M68K::bytes<Word>() -> uint { return 2; }
template<> auto M68K::bytes<Long>() -> uint { return 4; }

template<> auto M68K::bits<Byte>() -> uint { return 8; }
template<> auto M68K::bits<Word>() -> uint { return 16; }
template<> auto M68K::bits<Long>() -> uint { return 32; }

template<uint Size> auto M68K::lsb() -> uint32 { return 1; }

template<> auto M68K::msb<Byte>() -> uint32 { return 0x80; }
template<> auto M68K::msb<Word>() -> uint32 { return 0x8000; }
template<> auto M68K::msb<Long>() -> uint32 { return 0x80000000; }

template<> auto M68K::mask<Byte>() -> uint32 { return 0xff; }
template<> auto M68K::mask<Word>() -> uint32 { return 0xffff; }
template<> auto M68K::mask<Long>() -> uint32 { return 0xffffffff; }

template<> auto M68K::clip<Byte>(uint32 data) -> uint32 { return data & 0xff; }
template<> auto M68K::clip<Word>(uint32 data) -> uint32 { return data & 0xffff; }
template<> auto M68K::clip<Long>(uint32 data) -> uint32 { return data & 0xffffffff; }

template<> auto M68K::sign<Byte>(uint32 data) -> int32 { return  (int8)data; }
template<> auto M68K::sign<Word>(uint32 data) -> int32 { return (int16)data; }
template<> auto M68K::sign<Long>(uint32 data) -> int32 { return (int32)data; }

template<uint Size> auto M68K::zero(uint32 result) -> bool {
  return clip<Size>(result) == 0;
}

template<uint Size> auto M68K::negative(uint32 result) -> bool {
  return sign<Size>(result) < 0;
}

//

template<uint Size> auto M68K::instructionADD(DataRegister dr, uint1 direction, EffectiveAddress ea) -> void {
  uint64 source, target, result;

  if(direction == 0) {
    source = read<Size>(ea);
    target = read<Size>(dr);
    result = source + target;
    write<Size>(dr, result);
  } else {
    source = read<Size>(dr);
    target = read<Size>(ea);
    result = source + target;
    write<Size>(ea, result);
  }

  r.c = sign<Size>(result >> 1) < 0;
  r.v = sign<Size>(~(target ^ source) & (target ^ result)) < 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  r.x = r.c;
}

template<uint Size> auto M68K::instructionANDI(EffectiveAddress ea) -> void {
  auto source = readPC<Size>();
  auto target = read<Size, NoUpdate>(ea);
  auto result = target & source;
  write<Size>(ea, result);

  r.c = 0;
  r.v = 0;
  r.z = zero<Size>(result);
  r.n = negative<Size>(result);
}

auto M68K::instructionANDI_TO_CCR() -> void {
  auto data = readPC<Word>();
  writeCCR(readCCR() & data);
}

auto M68K::instructionANDI_TO_SR() -> void {
  if(!supervisor()) return;

  auto data = readPC<Word>();
  writeSR(readSR() & data);
}

template<uint Size> auto M68K::ASL(uint32 result, uint shift) -> uint32 {
  bool carry = false;
  uint32 overflow = 0;
  for(auto _ : range(shift)) {
    carry = result & msb<Size>();
    uint32 before = result;
    result <<= 1;
    overflow |= before ^ result;
  }

  r.c = carry;
  r.v = sign<Size>(overflow) < 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  if(shift) r.x = r.c;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionASL(uint4 shift, DataRegister modify) -> void {
  auto result = ASL<Size>(read<Size>(modify), shift);
  write<Size>(modify, result);
}

template<uint Size> auto M68K::instructionASL(DataRegister shift, DataRegister modify) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = ASL<Size>(read<Size>(modify), count);
  write<Size>(modify, result);
}

auto M68K::instructionASL(EffectiveAddress modify) -> void {
  auto result = ASL<Word>(read<Word, NoUpdate>(modify), 1);
  write<Word>(modify, result);
}

template<uint Size> auto M68K::ASR(uint32 result, uint shift) -> uint32 {
  bool carry = false;
  uint32 overflow = 0;
  for(auto _ : range(shift)) {
    carry = result & lsb<Size>();
    uint32 before = result;
    result = sign<Size>(result) >> 1;
    overflow |= before ^ result;
  }

  r.c = carry;
  r.v = sign<Size>(overflow) < 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  if(shift) r.x = r.c;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionASR(uint4 shift, DataRegister modify) -> void {
  auto result = ASR<Size>(read<Size>(modify), shift);
  write<Size>(modify, result);
}

template<uint Size> auto M68K::instructionASR(DataRegister shift, DataRegister modify) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = ASR<Size>(read<Size>(modify), count);
  write<Size>(modify, result);
}

auto M68K::instructionASR(EffectiveAddress modify) -> void {
  auto result = ASR<Word>(read<Word, NoUpdate>(modify), 1);
  write<Word>(modify, result);
}

auto M68K::instructionBCC(uint4 condition, uint8 displacement) -> void {
  auto extension = readPC<Word>();
  if(condition == 1) push<Long>(r.pc);
  if(condition >= 2 && !testCondition(condition)) {  //0 = BRA; 1 = BSR
    if(displacement) r.pc -= 2;
  } else {
    r.pc -= 2;
    r.pc += displacement ? sign<Byte>(displacement) : sign<Word>(extension);
  }
}

template<uint Size> auto M68K::instructionBTST(DataRegister dr, EffectiveAddress ea) -> void {
  auto bit = read<Size>(dr);
  auto test = read<Size>(ea);
  bit &= bits<Size>() - 1;

  r.z = test.bit(bit) == 0;
}

template<uint Size> auto M68K::instructionBTST(EffectiveAddress ea) -> void {
  auto bit = (uint8)readPC<Word>();
  auto test = read<Size>(ea);
  bit &= bits<Size>() - 1;

  r.z = test.bit(bit) == 0;
}

template<uint Size> auto M68K::instructionCLR(EffectiveAddress ea) -> void {
  read<Size>(ea);
  write<Size>(ea, 0);

  r.c = 0;
  r.v = 0;
  r.z = 1;
  r.n = 0;
}

template<uint Size> auto M68K::instructionCMP(DataRegister dr, EffectiveAddress ea) -> void {
  uint64 source = read<Size>(ea);
  uint64 target = read<Size>(dr);
  uint64 result = target - source;

  r.c = sign<Size>(result >> 1) < 0;
  r.v = sign<Size>((target ^ source) & (target ^ result)) < 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
}

auto M68K::instructionDBCC(uint4 condition, DataRegister dr) -> void {
  auto displacement = readPC<Word>();
  if(!testCondition(condition)) {
    uint16 result = read<Word>(dr);
    write<Word>(dr, result - 1);
    if(result) r.pc -= 2, r.pc += sign<Word>(displacement);
  }
}

auto M68K::instructionEORI_TO_CCR() -> void {
  auto data = readPC<Word>();
  writeCCR(readCCR() ^ data);
}

auto M68K::instructionEORI_TO_SR() -> void {
  if(!supervisor()) return;

  auto data = readPC<Word>();
  writeSR(readSR() ^ data);
}

auto M68K::instructionLEA(AddressRegister ar, EffectiveAddress ea) -> void {
  write<Long>(ar, fetch<Long>(ea));
}

template<uint Size> auto M68K::LSL(uint32 result, uint shift) -> uint32 {
  bool carry = false;
  for(auto _ : range(shift)) {
    carry = result & msb<Size>();
    result <<= 1;
  }

  r.c = carry;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  if(shift) r.x = r.c;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionLSL(uint4 immediate, DataRegister dr) -> void {
  auto result = LSL<Size>(read<Size>(dr), immediate);
  write<Size>(dr, result);
}

template<uint Size> auto M68K::instructionLSL(DataRegister sr, DataRegister dr) -> void {
  auto shift = read<Long>(sr) & 63;
  auto result = LSL<Size>(read<Size>(dr), shift);
  write<Size>(dr, result);
}

auto M68K::instructionLSL(EffectiveAddress ea) -> void {
  auto result = LSL<Word>(read<Word, NoUpdate>(ea), 1);
  write<Word>(ea, result);
}

template<uint Size> auto M68K::LSR(uint32 result, uint shift) -> uint32 {
  bool carry = false;
  for(auto _ : range(shift)) {
    carry = result & lsb<Size>();
    result >>= 1;
  }

  r.c = carry;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  if(shift) r.x = r.c;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionLSR(uint4 immediate, DataRegister dr) -> void {
  auto result = LSR<Size>(read<Size>(dr), immediate);
  write<Size>(dr, result);
}

template<uint Size> auto M68K::instructionLSR(DataRegister shift, DataRegister dr) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = LSR<Size>(read<Size>(dr), count);
  write<Size>(dr, result);
}

auto M68K::instructionLSR(EffectiveAddress ea) -> void {
  auto result = LSR<Word>(read<Word, NoUpdate>(ea), 1);
  write<Word>(ea, result);
}

template<uint Size> auto M68K::instructionMOVE(EffectiveAddress to, EffectiveAddress from) -> void {
  auto data = read<Size>(from);
  write<Size>(to, data);

  r.c = 0;
  r.v = 0;
  r.z = zero<Size>(data);
  r.n = negative<Size>(data);
}

template<uint Size> auto M68K::instructionMOVEA(AddressRegister ar, EffectiveAddress ea) -> void {
  auto data = read<Size>(ea);
  write<Long>(ar, data);
}

template<uint Size> auto M68K::instructionMOVEM(uint1 direction, EffectiveAddress ea) -> void {
  auto list = readPC();
  auto addr = fetch<Long>(ea);

  for(uint n : range(16)) {
    if(!list.bit(n)) continue;

    //pre-decrement mode traverses registers in reverse order {A7-A0, D7-D0}
    uint index = ea.mode == AddressRegisterIndirectWithPreDecrement ? 15 - n : n;

    if(ea.mode == AddressRegisterIndirectWithPreDecrement) addr -= bytes<Size>();

    if(direction == 0) {
      auto data = index < 8 ? read<Size>(DataRegister{index}) : read<Size>(AddressRegister{index});
      write<Size>(addr, data);
    } else {
      auto data = read<Size>(addr);
      data = sign<Size>(data);
      index < 8 ? write<Long>(DataRegister{index}, data) : write<Long>(AddressRegister{index}, data);
    }

    if(ea.mode == AddressRegisterIndirectWithPostIncrement) addr += bytes<Size>();
  }

  flush<Long>(ea, addr);
}

auto M68K::instructionMOVEQ(DataRegister dr, uint8 immediate) -> void {
  write<Long>(dr, immediate);

  r.c = 0;
  r.v = 0;
  r.z = zero<Byte>(immediate);
  r.n = negative<Byte>(immediate);
}

auto M68K::instructionMOVE_FROM_SR(EffectiveAddress ea) -> void {
  auto data = readSR();
  write<Word>(ea, data);
}

auto M68K::instructionMOVE_TO_CCR(EffectiveAddress ea) -> void {
  auto data = read<Byte>(ea);
  writeCCR(data);
}

auto M68K::instructionMOVE_TO_SR(EffectiveAddress ea) -> void {
  if(!supervisor()) return;

  auto data = read<Word>(ea);
  writeSR(data);
}

auto M68K::instructionMOVE_USP(uint1 direction, AddressRegister ar) -> void {
  if(!supervisor()) return;

  if(direction == 0) {
    r.sp = read<Long>(ar);
  } else {
    write<Long>(ar, r.sp);
  }
}

auto M68K::instructionNOP() -> void {
}

auto M68K::instructionORI_TO_CCR() -> void {
  auto data = readPC<Word>();
  writeCCR(readCCR() | data);
}

auto M68K::instructionORI_TO_SR() -> void {
  if(!supervisor()) return;

  auto data = readPC<Word>();
  writeSR(readSR() | data);
}

template<uint Size> auto M68K::ROL(uint32 result, uint shift) -> uint32 {
  bool carry = false;
  for(auto _ : range(shift)) {
    carry = result & msb<Size>();
    result = result << 1 | carry;
  }

  r.c = carry;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionROL(uint4 shift, DataRegister modify) -> void {
  auto result = ROL<Size>(read<Size>(modify), shift);
  write<Size>(modify, result);
}

template<uint Size> auto M68K::instructionROL(DataRegister shift, DataRegister modify) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = ROL<Size>(read<Size>(modify), count);
  write<Size>(modify, result);
}

auto M68K::instructionROL(EffectiveAddress modify) -> void {
  auto result = ROL<Word>(read<Word, NoUpdate>(modify), 1);
  write<Word>(modify, result);
}

template<uint Size> auto M68K::ROR(uint32 result, uint shift) -> uint32 {
  bool carry = false;
  for(auto _ : range(shift)) {
    carry = result & lsb<Size>();
    result >>= 1;
    if(carry) result |= msb<Size>();
  }

  r.c = carry;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionROR(uint4 shift, DataRegister modify) -> void {
  auto result = ROR<Size>(read<Size>(modify), shift);
  write<Size>(modify, result);
}

template<uint Size> auto M68K::instructionROR(DataRegister shift, DataRegister modify) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = ROR<Size>(read<Size>(modify), count);
  write<Size>(modify, result);
}

auto M68K::instructionROR(EffectiveAddress modify) -> void {
  auto result = ROR<Word>(read<Word, NoUpdate>(modify), 1);
  write<Word>(modify, result);
}

template<uint Size> auto M68K::ROXL(uint32 result, uint shift) -> uint32 {
  bool carry = r.x;
  for(auto _ : range(shift)) {
    bool extend = carry;
    carry = result & msb<Size>();
    result = result << 1 | extend;
  }

  r.c = carry;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  r.x = r.c;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionROXL(uint4 shift, DataRegister modify) -> void {
  auto result = ROXL<Size>(read<Size>(modify), shift);
  write<Size>(modify, result);
}

template<uint Size> auto M68K::instructionROXL(DataRegister shift, DataRegister modify) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = ROXL<Size>(read<Size>(modify), count);
  write<Size>(modify, result);
}

auto M68K::instructionROXL(EffectiveAddress modify) -> void {
  auto result = ROXL<Word>(read<Word, NoUpdate>(modify), 1);
  write<Word>(modify, result);
}

template<uint Size> auto M68K::ROXR(uint32 result, uint shift) -> uint32 {
  bool carry = r.x;
  for(auto _ : range(shift)) {
    bool extend = carry;
    carry = result & lsb<Size>();
    result >>= 1;
    if(extend) result |= msb<Size>();
  }

  r.c = carry;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  r.x = r.c;

  return clip<Size>(result);
}

template<uint Size> auto M68K::instructionROXR(uint4 shift, DataRegister modify) -> void {
  auto result = ROXR<Size>(read<Size>(modify), shift);
  write<Size>(modify, result);
}

template<uint Size> auto M68K::instructionROXR(DataRegister shift, DataRegister modify) -> void {
  auto count = read<Long>(shift) & 63;
  auto result = ROXR<Size>(read<Size>(modify), count);
  write<Size>(modify, result);
}

auto M68K::instructionROXR(EffectiveAddress modify) -> void {
  auto result = ROXR<Word>(read<Word, NoUpdate>(modify), 1);
  write<Word>(modify, result);
}

auto M68K::instructionRTS() -> void {
  r.pc = pop<Long>();
}

template<uint Size> auto M68K::instructionSUBQ(uint4 immediate, EffectiveAddress ea) -> void {
  uint64 target = read<Size, NoUpdate>(ea);
  uint64 source = immediate;
  uint64 result = target - source;
  write<Size>(ea, result);

  r.c = sign<Size>(result >> 1) < 0;
  r.v = sign<Size>((target ^ source) & (target ^ result)) < 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
  r.x = r.c;
}

template<uint Size> auto M68K::instructionTST(EffectiveAddress ea) -> void {
  auto data = read<Size>(ea);

  r.c = 0;
  r.v = 0;
  r.z = zero<Size>(data);
  r.n = negative<Size>(data);
}
