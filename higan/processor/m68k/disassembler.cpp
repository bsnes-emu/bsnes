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

auto M68K::_register(Register r) -> string {
  static const string registers[16] = {
    "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
  };
  return registers[r.number];
}

template<uint Size> auto M68K::_immediate() -> string {
  return {"#$", hex(_readPC<Size>(), 2 << Size)};
}

template<uint Size> auto M68K::_address(EA ea) -> string {
  if(ea.mode == 9) return {"$", hex(_pc + (int16)_readPC(), 6L)};
  return "???";
}

template<uint Size> auto M68K::_read(EA ea) -> string {
  if(ea.mode ==  0) return {_register(ea.reg)};
  if(ea.mode ==  1) return {_register(ea.reg)};
  if(ea.mode ==  2) return {"(", _register(ea.reg), ")"};
  if(ea.mode ==  3) return {"(", _register(ea.reg), ")+"};
  if(ea.mode ==  4) return {"-(", _register(ea.reg), ")"};
  if(ea.mode ==  5) return {"($", hex(read(ea.reg) + (int16)_readPC(), 6L), ")"};
  if(ea.mode ==  8) return {"($", hex(_readPC<Long>(), 6L), ")"};
  if(ea.mode == 11) return {"#$", hex(_readPC<Size>(), 2 << Size)};
  return "???";
}

template<uint Size> auto M68K::_write(EA ea) -> string {
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
    "ra", "sr", "hi", "ls", "cc", "cs", "ne", "eq",
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
    hex(r.d0, 8L), " ", hex(r.d1, 8L), " ", hex(r.d2, 8L), " ", hex(r.d3, 8L), " ",
    hex(r.d4, 8L), " ", hex(r.d5, 8L), " ", hex(r.d6, 8L), " ", hex(r.d7, 8L), " ",
    r.t ? "T" : "t",
    r.s ? "S" : "s",
    (uint)r.i,
    r.c ? "C" : "c",
    r.v ? "V" : "v",
    r.z ? "Z" : "z",
    r.n ? "N" : "n",
    r.x ? "X" : "x", "\n",
    hex(r.a0, 8L), " ", hex(r.a1, 8L), " ", hex(r.a2, 8L), " ", hex(r.a3, 8L), " ",
    hex(r.a4, 8L), " ", hex(r.a5, 8L), " ", hex(r.a6, 8L), " ", hex(r.ssp, 8L), " ", hex(r.usp, 8L)
  };
}

//

template<uint Size> auto M68K::disassembleADD(Register rd, uint1 direction, EA ea) -> string {
  string op{"add", _suffix<Size>(), "   "};

  if(direction == 0) {
    return {op, _read<Size>(ea), ",", _register(rd)};
  } else {
    return {op, "", _register(rd), ",", _read<Size>(ea)};
  }
}

template<uint Size> auto M68K::disassembleANDI(EA ea) -> string {
  return {"andi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _read<Size>(ea)};
}

auto M68K::disassembleBCC(uint4 condition, uint8 displacement) -> string {
  return {"b", _condition(condition), "     ", _branch(displacement)};
}

auto M68K::disassembleLEA(Register ra, EA ea) -> string {
  return {"lea     ", _address<Long>(ea), ",", _register(ra)};
}

template<uint Size> auto M68K::disassembleMOVE(EA to, EA from) -> string {
  return {"move", _suffix<Size>(), "  ", _read<Size>(from), ",", _write<Size>(to)};
}

template<uint Size> auto M68K::disassembleMOVEA(Register ra, EA ea) -> string {
  return {"movea   ", _read<Size>(ea), ",", _register(ra)};
}

template<uint Size> auto M68K::disassembleMOVEM(uint1 direction, EA ea) -> string {
  string op{"movem", _suffix<Size>(), " "};

  uint16 list = _readPC();
  string regs;
  for(uint rn : range(16)) if(list.bit(rn)) regs.append(_register(Register{rn}), ",");
  regs.trimRight(",");

  if(direction == 0) {
    return {op, regs, ",", _read<Size>(ea)};
  } else {
    return {op, _read<Size>(ea), ",", regs};
  }
}

auto M68K::disassembleMOVEQ(Register rd, uint8 immediate) -> string {
  return {"moveq   #$", hex(immediate, 2L), ",", _register(rd)};
}

auto M68K::disassembleMOVE_USP(uint1 direction, Register ra) -> string {
  if(direction == 0) {
    return {"move    ", _register(ra), ",usp"};
  } else {
    return {"move    usp,", _register(ra)};
  }
}

auto M68K::disassembleNOP() -> string {
  return {"nop     "};
}

template<uint Size> auto M68K::disassembleTST(EA ea) -> string {
  return {"tst", _suffix<Size>(), "   ", _read<Size>(ea)};
}
