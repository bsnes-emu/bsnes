template<> auto M68K::_read<Byte>(uint32 addr) -> uint32 {
  return read(0, addr);
}

template<> auto M68K::_read<Word>(uint32 addr) -> uint32 {
  return read(1, addr);
}

template<> auto M68K::_read<Long>(uint32 addr) -> uint32 {
  uint32 data = _read<Word>(addr + 0) << 16;
  return data | _read<Word>(addr + 2) <<  0;
}

template<uint Size> auto M68K::_readPC() -> uint32 {
  auto data = _read<Size == Byte ? Word : Size>(_pc);
  _pc += Size == Long ? 4 : 2;
  return clip<Size>(data);
}

auto M68K::_register(DataRegister dr) -> string {
  return {"d", dr.number};
}

auto M68K::_register(AddressRegister ar) -> string {
  return {"a", ar.number};
}

template<uint Size> auto M68K::_immediate() -> string {
  return {"#$", hex(_readPC<Size>(), 2 << Size)};
}

template<uint Size> auto M68K::_address(EffectiveAddress& ea) -> string {
  if(ea.mode == 9) return {"$", hex(_pc + (int16)_readPC(), 6L)};
  return "???";
}

template<uint Size> auto M68K::_read(EffectiveAddress& ea) -> string {
  if(ea.mode ==  0) return {_register(DataRegister{ea.reg})};
  if(ea.mode ==  1) return {_register(AddressRegister{ea.reg})};
  if(ea.mode ==  2) return {"(", _register(AddressRegister{ea.reg}), ")"};
  if(ea.mode ==  3) return {"(", _register(AddressRegister{ea.reg}), ")+"};
  if(ea.mode ==  4) return {"-(", _register(AddressRegister{ea.reg}), ")"};
  if(ea.mode ==  5) return {"($", hex(read(AddressRegister{ea.reg}) + (int16)_readPC(), 6L), ")"};
  if(ea.mode ==  8) return {"($", hex(_readPC<Long>(), 6L), ")"};
  if(ea.mode == 11) return {"#$", hex(_readPC<Size>(), 2 << Size)};
  return "???";
}

template<uint Size> auto M68K::_write(EffectiveAddress& ea) -> string {
  return _read<Size>(ea);
}

auto M68K::_branch(uint8 displacement) -> string {
  uint16 word = _readPC();
  if(displacement) displacement = (int8)displacement, _pc -= 2;
  else displacement = (int16)displacement;
  return {"$", hex(_pc + displacement, 6L)};
}

template<uint Size> auto M68K::_suffix() -> string {
  return Size == Byte ? ".b" : Size == Word ? ".w" : ".l";
}

auto M68K::_condition(uint4 condition) -> string {
  static const string conditions[16] = {
    "t ", "f ", "hi", "ls", "cc", "cs", "ne", "eq",
    "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le",
  };
  return conditions[condition];
}

auto M68K::disassemble(uint32 pc) -> string {
  uint16 opcode;
  return {hex(_pc = pc, 6L), "  ", hex(opcode = _readPC(), 4L), "  ", disassembleTable[opcode]()};
}

auto M68K::disassembleRegisters() -> string {
  return {
    hex(r.d[0], 8L), " ", hex(r.d[1], 8L), " ", hex(r.d[2], 8L), " ", hex(r.d[3], 8L), " ",
    hex(r.d[4], 8L), " ", hex(r.d[5], 8L), " ", hex(r.d[6], 8L), " ", hex(r.d[7], 8L), " ",
    r.t ? "T" : "t",
    r.s ? "S" : "s",
    (uint)r.i,
    r.c ? "C" : "c",
    r.v ? "V" : "v",
    r.z ? "Z" : "z",
    r.n ? "N" : "n",
    r.x ? "X" : "x", "\n",
    hex(r.a[0], 8L), " ", hex(r.a[1], 8L), " ", hex(r.a[2], 8L), " ", hex(r.a[3], 8L), " ",
    hex(r.a[4], 8L), " ", hex(r.a[5], 8L), " ", hex(r.a[6], 8L), " ", hex(r.a[7], 8L), " ", hex(r.sp, 8L)
  };
}

//

template<uint Size> auto M68K::disassembleADD(DataRegister dr, uint1 direction, EffectiveAddress ea) -> string {
  string op{"add", _suffix<Size>(), "   "};

  if(direction == 0) {
    return {op, _read<Size>(ea), ",", _register(dr)};
  } else {
    return {op, "", _register(dr), ",", _read<Size>(ea)};
  }
}

template<uint Size> auto M68K::disassembleANDI(EffectiveAddress ea) -> string {
  return {"andi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _read<Size>(ea)};
}

auto M68K::disassembleANDI_TO_CCR() -> string {
  return {"andi    ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleANDI_TO_SR() -> string {
  return {"andi    ", _immediate<Word>(), ",sr"};
}

auto M68K::disassembleBCC(uint4 condition, uint8 displacement) -> string {
  auto cc = _condition(condition);
  if(condition == 0) cc = "ra";
  if(condition == 1) cc = "sr";
  return {"b", cc, "     ", _branch(displacement)};
}

template<uint Size> auto M68K::disassembleBTST(DataRegister dr, EffectiveAddress ea) -> string {
  return {"btst    ", _register(dr), ",", _read<Size>(ea)};
}

template<uint Size> auto M68K::disassembleBTST(EffectiveAddress ea) -> string {
  return {"btst    ", _immediate<Byte>(), ",", _read<Size>(ea)};
}

template<uint Size> auto M68K::disassembleCLR(EffectiveAddress ea) -> string {
  return {"clr", _suffix<Size>(), "   ", _read<Size>(ea)};
}

template<uint Size> auto M68K::disassembleCMP(DataRegister dr, EffectiveAddress ea) -> string {
  return {"cmp", _suffix<Size>(), "   ", _read<Word>(ea), ",", _register(dr)};
}

auto M68K::disassembleDBCC(uint4 condition, DataRegister dr) -> string {
  auto base = _pc;
  auto displacement = (int16)_readPC();
  return {"db", _condition(condition), "    ", _register(dr), ",$", hex(base + displacement, 6L)};
}

auto M68K::disassembleEORI_TO_CCR() -> string {
  return {"eori    ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleEORI_TO_SR() -> string {
  return {"eori    ", _immediate<Word>(), ",sr"};
}

auto M68K::disassembleLEA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"lea     ", _address<Long>(ea), ",", _register(ar)};
}

template<uint Size> auto M68K::disassembleMOVE(EffectiveAddress to, EffectiveAddress from) -> string {
  return {"move", _suffix<Size>(), "  ", _read<Size>(from), ",", _write<Size>(to)};
}

template<uint Size> auto M68K::disassembleMOVEA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"movea   ", _read<Size>(ea), ",", _register(ar)};
}

template<uint Size> auto M68K::disassembleMOVEM(uint1 direction, EffectiveAddress ea) -> string {
  string op{"movem", _suffix<Size>(), " "};

  uint16 list = _readPC();
  string regs;
  for(uint n : range(8)) if(list.bit(0 + n)) regs.append(_register(DataRegister{n}), ",");
  regs.trimRight(",");
  if(regs && list >> 8) regs.append("/");
  for(uint n : range(8)) if(list.bit(8 + n)) regs.append(_register(AddressRegister{n}), ",");
  regs.trimRight(",");

  if(direction == 0) {
    return {op, regs, ",", _read<Size>(ea)};
  } else {
    return {op, _read<Size>(ea), ",", regs};
  }
}

auto M68K::disassembleMOVEQ(DataRegister dr, uint8 immediate) -> string {
  return {"moveq   #$", hex(immediate, 2L), ",", _register(dr)};
}

auto M68K::disassembleMOVE_FROM_SR(EffectiveAddress ea) -> string {
  return {"move    sr,", _read<Word>(ea)};
}

auto M68K::disassembleMOVE_TO_CCR(EffectiveAddress ea) -> string {
  return {"move    ", _read<Byte>(ea), ",ccr"};
}

auto M68K::disassembleMOVE_TO_SR(EffectiveAddress ea) -> string {
  return {"move    ", _read<Word>(ea), ",sr"};
}

auto M68K::disassembleMOVE_USP(uint1 direction, AddressRegister ar) -> string {
  if(direction == 0) {
    return {"move    ", _register(ar), ",usp"};
  } else {
    return {"move    usp,", _register(ar)};
  }
}

auto M68K::disassembleNOP() -> string {
  return {"nop     "};
}

auto M68K::disassembleORI_TO_CCR() -> string {
  return {"ori     ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleORI_TO_SR() -> string {
  return {"ori     ", _immediate<Word>(), ",sr"};
}

auto M68K::disassembleRTS() -> string {
  return {"rts     "};
}

template<uint Size> auto M68K::disassembleTST(EffectiveAddress ea) -> string {
  return {"tst", _suffix<Size>(), "   ", _read<Size>(ea)};
}
