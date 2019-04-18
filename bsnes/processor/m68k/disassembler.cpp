template<> auto M68K::_read<Byte>(uint32 addr) -> uint32 {
  return bus->readByte(addr);
}

template<> auto M68K::_read<Word>(uint32 addr) -> uint32 {
  return bus->readWord(addr);
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

auto M68K::_readDisplacement(uint32 base) -> uint32 {
  return base + (int16)_readPC<Word>();
}

auto M68K::_readIndex(uint32 base) -> uint32 {
  auto extension = _readPC<Word>();
  auto index = extension & 0x8000
  ? read(AddressRegister{extension >> 12})
  : read(DataRegister{extension >> 12});
  if(extension & 0x800) index = (int16)index;
  return base + index + (int8)extension;
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
  if(ea.mode == 7) return {"$", hex((int16)_readPC<Word>(), 6L)};
  if(ea.mode == 8) return {"$", hex(_readPC<Long>(), 6L)};
  if(ea.mode == 9) return {"$", hex(_pc + (int16)_readPC(), 6L)};
  return "???";
}

template<uint Size> auto M68K::_effectiveAddress(EffectiveAddress& ea) -> string {
  if(ea.mode ==  0) return {_dataRegister(DataRegister{ea.reg})};
  if(ea.mode ==  1) return {_addressRegister(AddressRegister{ea.reg})};
  if(ea.mode ==  2) return {"(", _addressRegister(AddressRegister{ea.reg}), ")"};
  if(ea.mode ==  3) return {"(", _addressRegister(AddressRegister{ea.reg}), ")+"};
  if(ea.mode ==  4) return {"-(", _addressRegister(AddressRegister{ea.reg}), ")"};
  if(ea.mode ==  5) return {"($", hex(_readDisplacement(read(AddressRegister{ea.reg})), 6L), ")"};
  if(ea.mode ==  6) return {"($", hex(_readIndex(read(AddressRegister{ea.reg})), 6L), ")"};
  if(ea.mode ==  7) return {"($", hex((int16)_readPC<Word>(), 6L), ")"};
  if(ea.mode ==  8) return {"($", hex(_readPC<Long>(), 6L), ")"};
  if(ea.mode ==  9) return {"($", hex(_readDisplacement(_pc), 6L), ")"};
  if(ea.mode == 10) return {"($", hex(_readIndex(_pc), 6L), ")"};
  if(ea.mode == 11) return {"#$", hex(_readPC<Size>(), 2 << Size)};
  return "???";  //should never occur
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

auto M68K::disassembleABCD(EffectiveAddress with, EffectiveAddress from) -> string {
  return {"abcd    ", _effectiveAddress<Byte>(from), ",", _effectiveAddress<Byte>(with)};
}

template<uint Size> auto M68K::disassembleADD(EffectiveAddress from, DataRegister with) -> string {
  return {"add", _suffix<Size>(), "   ", _effectiveAddress<Size>(from), ",", _dataRegister(with)};
}

template<uint Size> auto M68K::disassembleADD(DataRegister from, EffectiveAddress with) -> string {
  return {"add", _suffix<Size>(), "   ", _dataRegister(from), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleADDA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"adda", _suffix<Size>(), "  ", _effectiveAddress<Size>(ea), ",", _addressRegister(ar)};
}

template<uint Size> auto M68K::disassembleADDI(EffectiveAddress ea) -> string {
  return {"addi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _effectiveAddress<Size>(ea)};
}

template<uint Size> auto M68K::disassembleADDQ(uint4 immediate, EffectiveAddress with) -> string {
  return {"addq", _suffix<Size>(), "  #", immediate, ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleADDQ(uint4 immediate, AddressRegister with) -> string {
  return {"addq", _suffix<Size>(), "  #", immediate, ",", _addressRegister(with)};
}

template<uint Size> auto M68K::disassembleADDX(EffectiveAddress with, EffectiveAddress from) -> string {
  return {"addx", _suffix<Size>(), "  ", _effectiveAddress<Size>(from), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleAND(EffectiveAddress from, DataRegister with) -> string {
  return {"and", _suffix<Size>(), "   ", _effectiveAddress<Size>(from), ",", _dataRegister(with)};
}

template<uint Size> auto M68K::disassembleAND(DataRegister from, EffectiveAddress with) -> string {
  return {"and", _suffix<Size>(), "   ", _dataRegister(from), ",", _effectiveAddress<Size>(with)};
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

template<uint Size> auto M68K::disassembleBCHG(DataRegister bit, EffectiveAddress with) -> string {
  return {"bchg", _suffix<Size>(), "  ", _dataRegister(bit), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBCHG(EffectiveAddress with) -> string {
  return {"bchg", _suffix<Size>(), "  ", _immediate<Byte>(), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBCLR(DataRegister bit, EffectiveAddress with) -> string {
  return {"bclr", _suffix<Size>(), "  ", _dataRegister(bit), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBCLR(EffectiveAddress with) -> string {
  return {"bclr", _suffix<Size>(), "  ", _immediate<Byte>(), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBSET(DataRegister bit, EffectiveAddress with) -> string {
  return {"bset", _suffix<Size>(), "  ", _dataRegister(bit), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBSET(EffectiveAddress with) -> string {
  return {"bset", _suffix<Size>(), "  ", _immediate<Byte>(), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBTST(DataRegister bit, EffectiveAddress with) -> string {
  return {"btst", _suffix<Size>(), "  ", _dataRegister(bit), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleBTST(EffectiveAddress with) -> string {
  return {"btst", _suffix<Size>(), "  ", _immediate<Byte>(), ",", _effectiveAddress<Size>(with)};
}

auto M68K::disassembleCHK(DataRegister compare, EffectiveAddress maximum) -> string {
  return {"chk", _suffix<Word>(), "   ", _effectiveAddress<Word>(maximum), ",", _dataRegister(compare)};
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

auto M68K::disassembleDIVS(DataRegister with, EffectiveAddress from) -> string {
  return {"divs", _suffix<Word>(), "  ", _effectiveAddress<Word>(from), ",", _dataRegister(with)};
}

auto M68K::disassembleDIVU(DataRegister with, EffectiveAddress from) -> string {
  return {"divu", _suffix<Word>(), "  ", _effectiveAddress<Word>(from), ",", _dataRegister(with)};
}

template<uint Size> auto M68K::disassembleEOR(DataRegister from, EffectiveAddress with) -> string {
  return {"eor", _suffix<Size>(), "   ", _dataRegister(from), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleEORI(EffectiveAddress with) -> string {
  return {"eori", _suffix<Size>(), "  ", _immediate<Size>(), ",", _effectiveAddress<Size>(with)};
}

auto M68K::disassembleEORI_TO_CCR() -> string {
  return {"eori    ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleEORI_TO_SR() -> string {
  return {"eori    ", _immediate<Word>(), ",sr"};
}

auto M68K::disassembleEXG(DataRegister x, DataRegister y) -> string {
  return {"exg     ", _dataRegister(x), ",", _dataRegister(y)};
}

auto M68K::disassembleEXG(AddressRegister x, AddressRegister y) -> string {
  return {"exg     ", _addressRegister(x), ",", _addressRegister(y)};
}

auto M68K::disassembleEXG(DataRegister x, AddressRegister y) -> string {
  return {"exg     ", _dataRegister(x), ",", _addressRegister(y)};
}

template<uint Size> auto M68K::disassembleEXT(DataRegister with) -> string {
  return {"ext", _suffix<Size>(), "   ", _dataRegister(with)};
}

auto M68K::disassembleILLEGAL(uint16 code) -> string {
  if(code.bits(12,15) == 0xa) return {"linea   $", hex(code.bits(0,11), 3L)};
  if(code.bits(12,15) == 0xf) return {"linef   $", hex(code.bits(0,11), 3L)};
  return {"illegal "};
}

auto M68K::disassembleJMP(EffectiveAddress target) -> string {
  return {"jmp     ", _effectiveAddress<Long>(target)};
}

auto M68K::disassembleJSR(EffectiveAddress target) -> string {
  return {"jsr     ", _effectiveAddress<Long>(target)};
}

auto M68K::disassembleLEA(AddressRegister ar, EffectiveAddress ea) -> string {
  return {"lea     ", _address<Long>(ea), ",", _addressRegister(ar)};
}

auto M68K::disassembleLINK(AddressRegister with) -> string {
  return {"link    ", _addressRegister(with), ",", _immediate<Word>()};
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

template<uint Size> auto M68K::disassembleMOVEM_TO_MEM(EffectiveAddress to) -> string {
  string op{"movem", _suffix<Size>(), " "};

  uint16 list = _readPC();
  string regs;
  for(uint n : range(8)) if(list.bit(0 + n)) regs.append(_dataRegister(DataRegister{n}), ",");
  regs.trimRight(",");
  if(regs && list >> 8) regs.append("/");
  for(uint n : range(8)) if(list.bit(8 + n)) regs.append(_addressRegister(AddressRegister{n}), ",");
  regs.trimRight(",");

  return {op, regs, ",", _effectiveAddress<Size>(to)};
}

template<uint Size> auto M68K::disassembleMOVEM_TO_REG(EffectiveAddress from) -> string {
  string op{"movem", _suffix<Size>(), " "};

  uint16 list = _readPC();
  string regs;
  for(uint n : range(8)) if(list.bit(0 + n)) regs.append(_dataRegister(DataRegister{n}), ",");
  regs.trimRight(",");
  if(regs && list >> 8) regs.append("/");
  for(uint n : range(8)) if(list.bit(8 + n)) regs.append(_addressRegister(AddressRegister{n}), ",");
  regs.trimRight(",");

  return {op, _effectiveAddress<Size>(from), ",", regs};
}

template<uint Size> auto M68K::disassembleMOVEP(DataRegister from, EffectiveAddress to) -> string {
  return {"movep", _suffix<Size>(), " ", _dataRegister(from), ",", _effectiveAddress<Size>(to)};
}

template<uint Size> auto M68K::disassembleMOVEP(EffectiveAddress from, DataRegister to) -> string {
  return {"movep", _suffix<Size>(), " ", _effectiveAddress<Size>(from), ",", _dataRegister(to)};
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

auto M68K::disassembleMOVE_FROM_USP(AddressRegister to) -> string {
  return {"move    usp,", _addressRegister(to)};
}

auto M68K::disassembleMOVE_TO_USP(AddressRegister from) -> string {
  return {"move    ", _addressRegister(from), ",usp"};
}

auto M68K::disassembleMULS(DataRegister with, EffectiveAddress from) -> string {
  return {"muls", _suffix<Word>(), "  ", _effectiveAddress<Word>(from), ",", _dataRegister(with)};
}

auto M68K::disassembleMULU(DataRegister with, EffectiveAddress from) -> string {
  return {"mulu", _suffix<Word>(), "  ", _effectiveAddress<Word>(from), ",", _dataRegister(with)};
}

auto M68K::disassembleNBCD(EffectiveAddress with) -> string {
  return {"nbcd    ", _effectiveAddress<Byte>(with)};
}

template<uint Size> auto M68K::disassembleNEG(EffectiveAddress with) -> string {
  return {"neg", _suffix<Size>(), "   ", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleNEGX(EffectiveAddress with) -> string {
  return {"negx", _suffix<Size>(), "  ", _effectiveAddress<Size>(with)};
}

auto M68K::disassembleNOP() -> string {
  return {"nop     "};
}

template<uint Size> auto M68K::disassembleNOT(EffectiveAddress with) -> string {
  return {"not", _suffix<Size>(), "   ", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleOR(EffectiveAddress from, DataRegister with) -> string {
  return {"or", _suffix<Size>(), "    ", _effectiveAddress<Size>(from), ",", _dataRegister(with)};
}

template<uint Size> auto M68K::disassembleOR(DataRegister from, EffectiveAddress with) -> string {
  return {"or", _suffix<Size>(), "    ", _dataRegister(from), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleORI(EffectiveAddress with) -> string {
  return {"ori", _suffix<Size>(), "   ", _immediate<Size>(), ",", _effectiveAddress<Size>(with)};
}

auto M68K::disassembleORI_TO_CCR() -> string {
  return {"ori     ", _immediate<Byte>(), ",ccr"};
}

auto M68K::disassembleORI_TO_SR() -> string {
  return {"ori     ", _immediate<Word>(), ",sr"};
}

auto M68K::disassemblePEA(EffectiveAddress from) -> string {
  return {"pea     ", _effectiveAddress<Long>(from)};
}

auto M68K::disassembleRESET() -> string {
  return {"reset   "};
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

auto M68K::disassembleRTE() -> string {
  return {"rte     "};
}

auto M68K::disassembleRTR() -> string {
  return {"rtr     "};
}

auto M68K::disassembleRTS() -> string {
  return {"rts     "};
}

auto M68K::disassembleSBCD(EffectiveAddress with, EffectiveAddress from) -> string {
  return {"sbcd    ", _effectiveAddress<Byte>(from), ",", _effectiveAddress<Byte>(with)};
}

auto M68K::disassembleSCC(uint4 condition, EffectiveAddress to) -> string {
  return {"s", _condition(condition), "     ", _effectiveAddress<Byte>(to)};
}

auto M68K::disassembleSTOP() -> string {
  return {"stop    ", _immediate<Word>()};
}

template<uint Size> auto M68K::disassembleSUB(EffectiveAddress source, DataRegister target) -> string {
  return {"sub", _suffix<Size>(), "   ", _effectiveAddress<Size>(source), ",", _dataRegister(target)};
}

template<uint Size> auto M68K::disassembleSUB(DataRegister source, EffectiveAddress target) -> string {
  return {"sub", _suffix<Size>(), "   ", _dataRegister(source), ",", _effectiveAddress<Size>(target)};
}

template<uint Size> auto M68K::disassembleSUBA(AddressRegister to, EffectiveAddress from) -> string {
  return {"suba", _suffix<Size>(), "  ", _addressRegister(to), ",", _effectiveAddress<Size>(from)};
}

template<uint Size> auto M68K::disassembleSUBI(EffectiveAddress with) -> string {
  return {"subi", _suffix<Size>(), "  ", _immediate<Size>(), ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleSUBQ(uint4 immediate, EffectiveAddress with) -> string {
  return {"subq", _suffix<Size>(), "  #", immediate, ",", _effectiveAddress<Size>(with)};
}

template<uint Size> auto M68K::disassembleSUBQ(uint4 immediate, AddressRegister with) -> string {
  return {"subq", _suffix<Size>(), "  #", immediate, ",", _addressRegister(with)};
}

template<uint Size> auto M68K::disassembleSUBX(EffectiveAddress with, EffectiveAddress from) -> string {
  return {"subx", _suffix<Size>(), "  ", _effectiveAddress<Size>(from), ",", _effectiveAddress<Size>(with)};
}

auto M68K::disassembleSWAP(DataRegister with) -> string {
  return {"swap    ", _dataRegister(with)};
}

auto M68K::disassembleTAS(EffectiveAddress with) -> string {
  return {"tas     ", _effectiveAddress<Byte>(with)};
}

auto M68K::disassembleTRAP(uint4 vector) -> string {
  return {"trap    #", vector};
}

auto M68K::disassembleTRAPV() -> string {
  return {"trapv   "};
}

template<uint Size> auto M68K::disassembleTST(EffectiveAddress ea) -> string {
  return {"tst", _suffix<Size>(), "   ", _effectiveAddress<Size>(ea)};
}

auto M68K::disassembleUNLK(AddressRegister with) -> string {
  return {"unlk    ", _addressRegister(with)};
}
