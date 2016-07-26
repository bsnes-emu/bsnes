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

auto M68K::_dataRegister(DataRegister dr) -> string {
  return {"d", dr.number};
}

auto M68K::_addressRegister(AddressRegister ar) -> string {
  return {"a", ar.number};
}

template<uint Size> auto M68K::_immediate() -> string {
  return {"#$", hex(_readPC<Size>(), 2 << Size)};
}

template<uint Size> auto M68K::_address(EffectiveAddress& ea) -> string {
  if(ea.mode == 9) return {"$", hex(_pc + (int16)_readPC(), 6L)};
  return "???";
}

template<uint Size> auto M68K::_effectiveAddress(EffectiveAddress& ea) -> string {
  if(ea.mode ==  0) return {_dataRegister(DataRegister{ea.reg})};
  if(ea.mode ==  1) return {_addressRegister(AddressRegister{ea.reg})};
  if(ea.mode ==  2) return {"(", _addressRegister(AddressRegister{ea.reg}), ")"};
  if(ea.mode ==  3) return {"(", _addressRegister(AddressRegister{ea.reg}), ")+"};
  if(ea.mode ==  4) return {"-(", _addressRegister(AddressRegister{ea.reg}), ")"};
  if(ea.mode ==  5) return {"($", hex(read(AddressRegister{ea.reg}) + (int16)_readPC(), 6L), ")"};
  if(ea.mode ==  7) return {"($", hex((int16)_readPC<Word>(), 6L), ")"};
  if(ea.mode ==  8) return {"($", hex(_readPC<Long>(), 6L), ")"};
  if(ea.mode == 11) return {"#$", hex(_readPC<Size>(), 2 << Size)};
  return "???";
}

auto M68K::_branch(uint8 displacement) -> string {
  uint16 extension = _readPC();
  _pc -= 2;
  int32 offset = displacement ? sign<Byte>(displacement) : sign<Word>(extension);
  return {"$", hex(_pc + offset, 6L)};
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
    return {op, _effectiveAddress<Size>(ea), ",", _dataRegister(dr)};
  } else {
    return {op, "", _dataRegister(dr), ",", _effectiveAddress<Size>(ea)};
  }
}

template<uint Size> auto M68K::disassembleADDA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"adda", _suffix<Size>(), "  ", _effectiveAddress<Size>(ea), ",", _addressRegister(ar)};
}

template<uint Size> auto M68K::disassembleADDI(EffectiveAddress ea) -> string {
  return {"addi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleADDQ(uint4 immediate, EffectiveAddress modify) -> string {
  return {"addq", _suffix<Size>(), "  #", immediate, ",", _effectiveAddress<Size>(modify)};
}

template<uint Size> auto M68K::disassembleANDI(EffectiveAddress ea) -> string {
  return {"andi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _effectiveAddress<Size>(ea)};
}

auto M68K::disassembleANDI_TO_CCR() -> string {
  return {"andi    ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleANDI_TO_SR() -> string {
  return {"andi    ", _immediate<Word>(), ",sr"};
}

template<uint Size> auto M68K::disassembleASL(uint4 shift, DataRegister modify) -> string {
  return {"asl", _suffix<Size>(), "   #", shift, ",", _dataRegister(modify)};
}

template<uint Size> auto M68K::disassembleASL(DataRegister shift, DataRegister modify) -> string {
  return {"asl", _suffix<Size>(), "   ", _dataRegister(shift), ",", _dataRegister(modify)};
}

auto M68K::disassembleASL(EffectiveAddress modify) -> string {
  return {"asl", _suffix<Word>(), "   ", _effectiveAddress<Word>(modify)};
}

template<uint Size> auto M68K::disassembleASR(uint4 shift, DataRegister modify) -> string {
  return {"asr", _suffix<Size>(), "   #", shift, ",", _dataRegister(modify)};
}

template<uint Size> auto M68K::disassembleASR(DataRegister shift, DataRegister modify) -> string {
  return {"asr", _suffix<Size>(), "   ", _dataRegister(shift), ",", _dataRegister(modify)};
}

auto M68K::disassembleASR(EffectiveAddress modify) -> string {
  return {"asr", _suffix<Word>(), "   ", _effectiveAddress<Word>(modify)};
}

auto M68K::disassembleBCC(uint4 condition, uint8 displacement) -> string {
  auto cc = _condition(condition);
  if(condition == 0) cc = "ra";
  if(condition == 1) cc = "sr";
  return {"b", cc, "     ", _branch(displacement)};
}

template<uint Size> auto M68K::disassembleBTST(DataRegister dr, EffectiveAddress ea) -> string {
  return {"btst    ", _dataRegister(dr), ",", _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleBTST(EffectiveAddress ea) -> string {
  return {"btst    ", _immediate<Byte>(), ",", _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleCLR(EffectiveAddress ea) -> string {
  return {"clr", _suffix<Size>(), "   ", _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleCMP(DataRegister dr, EffectiveAddress ea) -> string {
  return {"cmp", _suffix<Size>(), "   ", _effectiveAddress<Size>(ea), ",", _dataRegister(dr)};
}

template<uint Size> auto M68K::disassembleCMPA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"cmpa", _suffix<Size>(), "  ", _effectiveAddress<Size>(ea), ",", _addressRegister(ar)};
}

template<uint Size> auto M68K::disassembleCMPI(EffectiveAddress ea) -> string {
  return {"cmpi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleCMPM(EffectiveAddress ax, EffectiveAddress ay) -> string {
  return {"cmpm", _suffix<Size>(), "  ", _effectiveAddress<Size>(ay), ",", _effectiveAddress<Size>(ax)};
}

auto M68K::disassembleDBCC(uint4 condition, DataRegister dr) -> string {
  auto base = _pc;
  auto displacement = (int16)_readPC();
  return {"db", _condition(condition), "    ", _dataRegister(dr), ",$", hex(base + displacement, 6L)};
}

auto M68K::disassembleEORI_TO_CCR() -> string {
  return {"eori    ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleEORI_TO_SR() -> string {
  return {"eori    ", _immediate<Word>(), ",sr"};
}

auto M68K::disassembleJSR(EffectiveAddress target) -> string {
  return {"jsr     ", _effectiveAddress<Long>(target)};
}

auto M68K::disassembleLEA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"lea     ", _address<Long>(ea), ",", _addressRegister(ar)};
}

template<uint Size> auto M68K::disassembleLSL(uint4 immediate, DataRegister dr) -> string {
  return {"lsl", _suffix<Size>(), "   #", immediate, ",", _dataRegister(dr)};
}

template<uint Size> auto M68K::disassembleLSL(DataRegister sr, DataRegister dr) -> string {
  return {"lsl", _suffix<Size>(), "   ", _dataRegister(sr), ",", _dataRegister(dr)};
}

auto M68K::disassembleLSL(EffectiveAddress ea) -> string {
  return {"lsl", _suffix<Word>(), "   ", _effectiveAddress<Word>(ea)};
}

template<uint Size> auto M68K::disassembleLSR(uint4 immediate, DataRegister dr) -> string {
  return {"lsr", _suffix<Size>(), "   #", immediate, ",", _dataRegister(dr)};
}

template<uint Size> auto M68K::disassembleLSR(DataRegister shift, DataRegister dr) -> string {
  return {"lsr", _suffix<Size>(), "   ", _dataRegister(shift), ",", _dataRegister(dr)};
}

auto M68K::disassembleLSR(EffectiveAddress ea) -> string {
  return {"lsr", _suffix<Word>(), "   ", _effectiveAddress<Word>(ea)};
}

template<uint Size> auto M68K::disassembleMOVE(EffectiveAddress to, EffectiveAddress from) -> string {
  return {"move", _suffix<Size>(), "  ", _effectiveAddress<Size>(from), ",", _effectiveAddress<Size>(to)};
}

template<uint Size> auto M68K::disassembleMOVEA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"movea   ", _effectiveAddress<Size>(ea), ",", _addressRegister(ar)};
}

template<uint Size> auto M68K::disassembleMOVEM(uint1 direction, EffectiveAddress ea) -> string {
  string op{"movem", _suffix<Size>(), " "};

  uint16 list = _readPC();
  string regs;
  for(uint n : range(8)) if(list.bit(0 + n)) regs.append(_dataRegister(DataRegister{n}), ",");
  regs.trimRight(",");
  if(regs && list >> 8) regs.append("/");
  for(uint n : range(8)) if(list.bit(8 + n)) regs.append(_addressRegister(AddressRegister{n}), ",");
  regs.trimRight(",");

  if(direction == 0) {
    return {op, regs, ",", _effectiveAddress<Size>(ea)};
  } else {
    return {op, _effectiveAddress<Size>(ea), ",", regs};
  }
}

auto M68K::disassembleMOVEQ(DataRegister dr, uint8 immediate) -> string {
  return {"moveq   #$", hex(immediate, 2L), ",", _dataRegister(dr)};
}

auto M68K::disassembleMOVE_FROM_SR(EffectiveAddress ea) -> string {
  return {"move    sr,", _effectiveAddress<Word>(ea)};
}

auto M68K::disassembleMOVE_TO_CCR(EffectiveAddress ea) -> string {
  return {"move    ", _effectiveAddress<Byte>(ea), ",ccr"};
}

auto M68K::disassembleMOVE_TO_SR(EffectiveAddress ea) -> string {
  return {"move    ", _effectiveAddress<Word>(ea), ",sr"};
}

auto M68K::disassembleMOVE_USP(uint1 direction, AddressRegister ar) -> string {
  if(direction == 0) {
    return {"move    ", _addressRegister(ar), ",usp"};
  } else {
    return {"move    usp,", _addressRegister(ar)};
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

template<uint Size> auto M68K::disassembleROL(uint4 shift, DataRegister modify) -> string {
  return {"rol", _suffix<Size>(), "   #", shift, ",", _dataRegister(modify)};
}

template<uint Size> auto M68K::disassembleROL(DataRegister shift, DataRegister modify) -> string {
  return {"rol", _suffix<Size>(), "   ", _dataRegister(shift), ",", _dataRegister(modify)};
}

auto M68K::disassembleROL(EffectiveAddress modify) -> string {
  return {"rol", _suffix<Word>(), "   ", _effectiveAddress<Word>(modify)};
}

template<uint Size> auto M68K::disassembleROR(uint4 shift, DataRegister modify) -> string {
  return {"ror", _suffix<Size>(), "   #", shift, ",", _dataRegister(modify)};
}

template<uint Size> auto M68K::disassembleROR(DataRegister shift, DataRegister modify) -> string {
  return {"ror", _suffix<Size>(), "   ", _dataRegister(shift) ,",", _dataRegister(modify)};
}

auto M68K::disassembleROR(EffectiveAddress modify) -> string {
  return {"ror", _suffix<Word>(), "   ", _effectiveAddress<Word>(modify)};
}

template<uint Size> auto M68K::disassembleROXL(uint4 shift, DataRegister modify) -> string {
  return {"roxl", _suffix<Size>(), "  #", shift, ",", _dataRegister(modify)};
}

template<uint Size> auto M68K::disassembleROXL(DataRegister shift, DataRegister modify) -> string {
  return {"roxl", _suffix<Size>(), "  ", _dataRegister(shift), ",", _dataRegister(modify)};
}

auto M68K::disassembleROXL(EffectiveAddress modify) -> string {
  return {"roxl", _suffix<Word>(), "  ", _effectiveAddress<Word>(modify)};
}

template<uint Size> auto M68K::disassembleROXR(uint4 shift, DataRegister modify) -> string {
  return {"roxr", _suffix<Size>(), "  #", shift, ",", _dataRegister(modify)};
}

template<uint Size> auto M68K::disassembleROXR(DataRegister shift, DataRegister modify) -> string {
  return {"roxr", _suffix<Size>(), "  ", _dataRegister(shift), ",", _dataRegister(modify)};
}

auto M68K::disassembleROXR(EffectiveAddress modify) -> string {
  return {"roxr", _suffix<Word>(), "  ", _effectiveAddress<Word>(modify)};
}

auto M68K::disassembleRTS() -> string {
  return {"rts     "};
}

template<uint Size> auto M68K::disassembleSUBQ(uint4 immediate, EffectiveAddress ea) -> string {
  return {"subq", _suffix<Size>(), "  #", immediate, _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleTST(EffectiveAddress ea) -> string {
  return {"tst", _suffix<Size>(), "   ", _effectiveAddress<Size>(ea)};
}
