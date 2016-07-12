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

  auto suffix = [&](uint2 size) -> string {
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
    if(mode == 7) {
      if(reg == 0) return {"($", hex((int16)readWordPC(), 6L), ")", suffix(size)};
      if(reg == 1) return {"($", hex(readLongPC(), 6L), ")", suffix(size)};
    }

    return "???";
  };

  string s, op;
  s.append(hex(pc, 6L), "  ");

  auto opcode = readWordPC();
  s.append(hex(opcode, 4L), "  ");

  if(0);

  //bcc
  else if(opcode >> 12 == 0b0110) {
    op = {"b", cc(opcode >> 8), "   ", branch(opcode >> 0)};
  }

  //nop
  else if(opcode == 0b0100'1110'0111'0001) {
    op = {"nop"};
  }

  //tst
  else if(opcode >> 8 == 0b0100'1010) {
    op = {"tst", suffix(opcode >> 6), " ", ea(opcode >> 6, opcode >> 3, opcode >> 0)};
  }

  else {
    op = {"???"};
  }

  s.append(op);
  return s;
}
