auto M68K::trap() -> void {
  instructionsExecuted--;
  r.pc -= 2;
  print("[M68K] unimplemented instruction: ", hex(r.pc, 6L), " = ", hex(opcode, 4L), "\n");
  print("[M68K] instructions executed: ", instructionsExecuted, "\n");
  while(true) step(5);
}

auto M68K::instruction() -> void {
  instructionsExecuted++;
  print(disassembleRegisters(), "\n");
  print(disassemble(r.pc), "\n");
  print("\n");

  opcode = readPC();
  return instructionTable[opcode]();
}

M68K::M68K() {
  #define bind(id, name, ...) \
    assert(!instructionTable[id]); \
    instructionTable[id] = [=] { return instruction##name(__VA_ARGS__); }; \
    disassembleTable[id] = [=] { return disassemble##name(__VA_ARGS__); };

  #define pattern(s) \
    std::integral_constant<uint16_t, bit::test(s)>::value

  //ADD
  for(uint3 dreg      : range(8))
  for(uint1 direction : range(2))
  for(uint3 mode      : range(8))
  for(uint3 reg       : range(8)) {
    auto opcode = pattern("1101 ---- ++-- ----") | dreg << 9 | direction << 8 | mode << 3 | reg << 0;
    if(direction == 1 && (mode == 0 || mode == 1 || (mode == 7 && reg >= 2))) continue;

    Register rd{0u + dreg};
    EA ea{mode, reg};
    bind(opcode | 0 << 6, ADD<Byte>, rd, direction, ea);
    bind(opcode | 1 << 6, ADD<Word>, rd, direction, ea);
    bind(opcode | 2 << 6, ADD<Long>, rd, direction, ea);
  }

  //ANDI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 0010 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 4)) continue;

    EA ea{mode, reg};
    bind(opcode | 0 << 6, ANDI<Byte>, ea);
    bind(opcode | 1 << 6, ANDI<Word>, ea);
    bind(opcode | 2 << 6, ANDI<Long>, ea);
  }

  //BCC
  for(uint4 condition    : range( 16))
  for(uint8 displacement : range(256)) {
    auto opcode = pattern("0110 ---- ---- ----") | condition << 8 | displacement << 0;

    bind(opcode, BCC, condition, displacement);
  }

  //LEA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 ---1 11-- ----") | areg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || mode == 3 || mode == 4 || (mode == 7 && reg >= 4)) continue;

    Register ra{8u + areg};
    EA ea{mode, reg};
    bind(opcode, LEA, ra, ea);
  }

  //MOVE
  for(uint3 toReg    : range(8))
  for(uint3 toMode   : range(8))
  for(uint3 fromMode : range(8))
  for(uint3 fromReg  : range(8)) {
    auto opcode = pattern("00++ ---- ---- ----") | toReg << 9 | toMode << 6 | fromMode << 3 | fromReg << 0;
    if(toMode == 1 || (toMode == 7 && toReg >= 4)) continue;

    EA to{toMode, toReg};
    EA from{fromMode, fromReg};
    bind(opcode | 1 << 12, MOVE<Byte>, to, from);
    bind(opcode | 3 << 12, MOVE<Word>, to, from);
    bind(opcode | 2 << 12, MOVE<Long>, to, from);
  }

  //MOVEA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("00++ ---0 01-- ----") | areg << 9 | mode << 3 | reg << 0;

    Register ra{8u + areg};
    EA ea{mode, reg};
    bind(opcode | 3 << 12, MOVEA<Word>, ra, ea);
    bind(opcode | 2 << 12, MOVEA<Long>, ra, ea);
  }

  //MOVEM
  for(uint1 direction : range(2))
  for(uint3 mode      : range(8))
  for(uint3 reg       : range(8)) {
    auto opcode = pattern("0100 1-00 1+-- ----") | direction << 10 | mode << 3 | reg << 0;
    if(mode <= 1 || mode == 3 || (mode == 7 && reg >= 4));

    EA ea{mode, reg};
    bind(opcode | 0 << 6, MOVEM<Word>, direction, ea);
    bind(opcode | 1 << 6, MOVEM<Long>, direction, ea);
  }

  //MOVEQ
  for(uint3 dreg      : range(  8))
  for(uint8 immediate : range(256)) {
    auto opcode = pattern("0111 ---0 ---- ----") | dreg << 9 | immediate << 0;

    Register rd{0u + dreg};
    bind(opcode, MOVEQ, rd, immediate);
  }

  //MOVE_USP
  for(uint1 direction : range(2))
  for(uint3 areg      : range(8)) {
    auto opcode = pattern("0100 1110 0110 ----") | direction << 3 | areg << 0;

    Register ra{8u + areg};
    bind(opcode, MOVE_USP, direction, ra);
  }

  //NOP
  { auto opcode = pattern("0100 1110 0111 0001");

    bind(opcode, NOP);
  }

  //TST
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 1010 ++-- ----") | mode << 3 | reg << 0;

    EA ea{mode, reg};
    bind(opcode | 0 << 6, TST<Byte>, ea);
    bind(opcode | 1 << 6, TST<Word>, ea);
    bind(opcode | 2 << 6, TST<Long>, ea);
  }

  #undef bind
  #undef pattern

  uint unimplemented = 0;
  for(uint16 opcode : range(65536)) {
    if(instructionTable[opcode]) continue;
    instructionTable[opcode] = [=] { trap(); };
    disassembleTable[opcode] = [=] { return string{"???"}; };
    unimplemented++;
  }
//print("[M68K] unimplemented opcodes: ", unimplemented, "\n");
}
