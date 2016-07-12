auto M68K::disassemble(uint32 pc) -> string {
  auto readByte = [&](uint32 addr) -> uint8 {
    return read(addr);
  };
  auto readWord = [&](uint32 addr) -> uint16 {
    uint16 data  = read(addr + 0) << 8;
    return data |= read(addr + 1) << 0;
  };
  auto readLong = [&](uint32 addr) -> uint32 {
    uint32 data  = readWord(addr + 0) << 16;
    return data |= readWord(addr + 2) <<  0;
  };
  auto readWordPC = [&]() -> uint16 {
    auto data = readWord(pc);
    pc += 2;
    return data;
  };
  auto readLongPC = [&]() -> uint32 {
    uint32 data  = readWordPC() << 16;
    return data |= readWordPC() <<  0;
  };

  auto size = [&](uint2 size) -> string {
    if(size == 0) return ".b";
    if(size == 1) return ".w";
    if(size == 2) return ".l";
    return ".?";
  };

  auto branch = [&](uint8 displacementByte) -> string {
    uint16 displacementWord = readWordPC();
    if(displacementByte) pc -= 2;
    return {"$", hex(pc + (displacementByte ? (int8_t)displacementByte : ((int16_t)displacementWord - 2)), 6L)};
  };

  auto cc = [&](uint4 condition) -> string {
    switch(condition) {
    case  0: return "ra";
    case  1: return "sr";
    case  2: return "hi";
    case  3: return "ls";
    case  4: return "cc";
    case  5: return "cs";
    case  6: return "ne";
    case  7: return "eq";
    case  8: return "vc";
    case  9: return "vs";
    case 10: return "pl";
    case 11: return "mi";
    case 12: return "ge";
    case 13: return "lt";
    case 14: return "gt";
    case 15: return "le";
    }
    unreachable;
  };

  auto ea = [&](uint2 size, uint3 mode, uint3 reg) -> string {
    if(mode == 3) return {"(a", reg, ")+"};

    if(mode == 7) {
      if(reg == 0) return {"($", hex((int16)readWordPC(), 6L), ")"};
      if(reg == 1) return {"($", hex(readLongPC(), 6L), ")"};
      if(reg == 2) return {"($", hex(pc + (int16)readWordPC(), 6L), ")"};
    }

    return "???";
  };

  auto rd = [&](uint3 reg) -> string {
    static const string name[8] = {"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7"};
    return name[reg];
  };

  auto ra = [&](uint3 reg) -> string {
    static const string name[8] = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "sp"};
    return name[reg];
  };

  string s, name, args;
  s.append(hex(pc, 6L), "  ");

  auto opcode = readWordPC();
  s.append(hex(opcode, 4L), "  ");

  if(0);

  #define match(pattern) else if( \
    (opcode & std::integral_constant<uint16_t, bit::mask(pattern)>::value) \
    == std::integral_constant<uint16_t, bit::test(pattern)>::value \
  )
  #define bit(n) (opcode >> n & 1)
  #define bits(hi, lo) ((opcode >> lo) & ((1 << (hi - lo + 1)) - 1))

  //NOP
  match("0100 1110 0111 0001") {
    name = {"nop"};
  }

  //MOVEM
  match("0100 1-00 1--- ----") {
    name = {"movem", size(1 + bit(6))};
    bool direction = bit(10);
    uint16 list = readWordPC();

    string regs;
    for(auto n : range(8)) if(list & 1 << (0 + n)) regs.append("d", n, ",");
    for(auto n : range(8)) if(list & 1 << (8 + n)) regs.append("a", n, ",");
    regs.trimRight(",");

    if(direction == 0) {
      args = {regs, ",", ea(1 + bit(6), bits(5,3), bits(2,0))};
    } else {
      args = {ea(1 + bit(6), bits(5,3), bits(2,0)), ",", regs};
    }
  }

  //TST
  match("0100 1010 ---- ----") {
    name = {"tst", size(bits(7,6))};
    args = {ea(bits(7,6), bits(5,3), bits(2,0))};
  }

  //LEA
  match("0100 ---1 11-- ----") {
    name = {"lea"};
    args = {ea(Long, bits(5,3), bits(2,0)), ",", ra(bits(11,9))};
  }

  //BCC
  match("0110 ---- ---- ----") {
    name = {"b", cc(bits(11,8))};
    args = {branch(bits(7,0))};
  }

  #undef match
  #undef bit
  #undef bits

  else {
    name = {"???"};
  }

  s.append(name.size(-8), args);
  return s;
}

auto M68K::disassembleRegisters() -> string {
  return {
    hex(r.d0, 8L), " ", hex(r.d1, 8L), " ", hex(r.d2, 8L), " ", hex(r.d3, 8L), " ",
    hex(r.d4, 8L), " ", hex(r.d5, 8L), " ", hex(r.d6, 8L), " ", hex(r.d7, 8L), " ",
    r.c ? "C" : "c",
    r.v ? "V" : "v",
    r.z ? "Z" : "z",
    r.n ? "N" : "n",
    r.x ? "X" : "x", "\n",
    hex(r.a0, 8L), " ", hex(r.a1, 8L), " ", hex(r.a2, 8L), " ", hex(r.a3, 8L), " ",
    hex(r.a4, 8L), " ", hex(r.a5, 8L), " ", hex(r.a6, 8L), " ", hex(r.usp, 8L), " ", hex(r.ssp, 8L)
  };
}
