auto M68K::_readByte(uint32 addr) -> uint8 {
  return read(addr);
}

auto M68K::_readWord(uint32 addr) -> uint16 {
  uint16 data  = _readByte(addr + 0) << 8;
  return data |= _readByte(addr + 1) << 0;
}

auto M68K::_readLong(uint32 addr) -> uint32 {
  uint32 data  = _readWord(addr + 0) << 16;
  return data |= _readWord(addr + 2) <<  0;
}

auto M68K::_readPC(uint size) -> uint32 {
  uint32 data = _readWord(_pc);
  _pc += 2;
  if(size == Byte) return (uint8)data;
  if(size == Word) return data;
  data = (data << 16) | _readWord(_pc);
  _pc += 2;
  return data;
}

auto M68K::_immediate(uint size) -> string {
  if(size == Byte) return {"#$", hex(_readPC(Byte), 2L)};
  if(size == Word) return {"#$", hex(_readPC(Word), 4L)};
  if(size == Long) return {"#$", hex(_readPC(Long), 8L)};
  return "#???";
}

auto M68K::_address(uint size, EA& ea) -> string {
  if(ea.mode == 7) {
    if(ea.reg == 2) return {"$", hex(_pc + (int16)_readPC(Word), 6L)};
  }
  return "???";
}

auto M68K::_read(uint size, EA& ea) -> string {
  if(ea.mode == 0) return {"d", ea.reg};
  if(ea.mode == 1) return {"a", ea.reg};
  if(ea.mode == 2) return {"(a", ea.reg, ")"};
  if(ea.mode == 3) return {"(a", ea.reg, ")+"};
  if(ea.mode == 4) return {"-(a", ea.reg, ")"};
  if(ea.mode == 5) return {"($", hex(r.a(ea.reg) + (int16)_readPC(Word), 6L), ")"};
  if(ea.mode == 7) {
    if(ea.reg == 1) return {"($", hex(_readPC(Long), 6L), ")"};
    if(ea.reg == 4) {
      if(size == Byte) return {"#$", hex(_readPC(Byte), 2L)};
      if(size == Word) return {"#$", hex(_readPC(Word), 4L)};
      if(size == Long) return {"#$", hex(_readPC(Long), 8L)};
    }
  }
  return "???";
}

auto M68K::_write(uint size, EA& ea) -> string {
  return _read(size, ea);
}

auto M68K::_branch(uint displacement) -> string {
  uint16 word = _readPC();
  if(displacement) displacement = (int8)displacement, _pc -= 2;
  else displacement = (int16)displacement;
  return {"$", hex(_pc + displacement, 6L)};
}

auto M68K::_suffix(uint size) -> string {
  if(size == Byte) return ".b";
  if(size == Word) return ".w";
  if(size == Long) return ".l";
  return ".?";
}

auto M68K::_condition(uint condition) -> string {
  static const string conditions[16] = {
    "ra", "sr", "hi", "ls", "cc", "cs", "ne", "eq",
    "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le",
  };
  return conditions[(uint4)condition];
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

auto M68K::disassembleANDI(uint size, EA modify) -> string {
  return {"andi", _suffix(size), "  ", _immediate(size), ",", _read(size, modify)};
}

auto M68K::disassembleBCC(uint condition, uint displacement) -> string {
  return {"b", _condition(condition), "     ", _branch(displacement)};
}

auto M68K::disassembleLEA(uint target, EA source) -> string {
  return {"lea     ", _address(Long, source), ",a", target};
}

auto M68K::disassembleMOVE(uint size, EA target, EA source) -> string {
  return {"move", _suffix(size), "  ", _read(size, source), ",", _write(size, target)};
}

auto M68K::disassembleMOVEA(uint size, uint target, EA source) -> string {
  return {"movea   ", _read(size, source), ",a", target};
}

auto M68K::disassembleMOVEM(uint direction, uint size, EA source) -> string {
  string op{"movem", _suffix(size), " "};

  uint16 list = _readPC();
  string regs;
  for(auto n : range(8)) if(list.bit(0 + n)) regs.append("d", n, ",");
  regs.trimRight(",");
  if(regs && list >> 8) regs.append("/");
  for(auto n : range(8)) if(list.bit(8 + n)) regs.append("a", n, ",");
  regs.trimRight(",");

  if(direction == 0) {
    return {op, regs, ",", _read(size, source)};
  } else {
    return {op, _read(size, source), ",", regs};
  }
}

auto M68K::disassembleMOVEQ(uint target, uint immediate) -> string {
  return {"moveq   #$", hex(immediate, 2L), ",d", target};
}

auto M68K::disassembleMOVE_USP(uint direction, uint reg) -> string {
  if(direction == 0) {
    return {"move    a", reg, ",usp"};
  } else {
    return {"move    usp,a", reg};
  }
}

auto M68K::disassembleNOP() -> string {
  return {"nop     "};
}

auto M68K::disassembleTST(uint size, EA source) -> string {
  return {"tst", _suffix(size), "   ", _read(size, source)};
}
