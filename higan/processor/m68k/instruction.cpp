auto M68K::trap() -> void {
  instructionsExecuted--;
  r.pc -= 2;
  print("[M68K] unimplemented instruction: ", hex(r.pc, 6L), " = ", hex(opcode, 4L), "\n");
  print("[M68K] instructions executed: ", instructionsExecuted, "\n");
  while(true) step(5);
}

auto M68K::instruction() -> void {
  instructionsExecuted++;

  if(instructionsExecuted >= 20) trap();

  print(disassembleRegisters(), "\n");
  print(disassemble(r.pc), "\n");
  print("\n");

  opcode = readPC();
  return instructionTable[opcode]();
}

M68K::M68K() {
  #define bind(id, name, ...) { \
    assert(!instructionTable[id]); \
    instructionTable[id] = [=] { return instruction##name(__VA_ARGS__); }; \
    disassembleTable[id] = [=] { return disassemble##name(__VA_ARGS__); }; \
  }

  #define unbind(id) { \
    instructionTable[id].reset(); \
    disassembleTable[id].reset(); \
  }

  #define pattern(s) \
    std::integral_constant<uint16_t, bit::test(s)>::value

  //ADD
  for(uint3 dreg      : range(8))
  for(uint1 direction : range(2))
  for(uint3 mode      : range(8))
  for(uint3 reg       : range(8)) {
    auto opcode = pattern("1101 ---- ++-- ----") | dreg << 9 | direction << 8 | mode << 3 | reg << 0;
    if(direction == 1 && (mode == 0 || mode == 1 || (mode == 7 && reg >= 2))) continue;

    DataRegister dr{dreg};
    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, ADD<Byte>, dr, direction, ea);
    bind(opcode | 1 << 6, ADD<Word>, dr, direction, ea);
    bind(opcode | 2 << 6, ADD<Long>, dr, direction, ea);

    if(direction == 0 && mode == 1) unbind(opcode | 0 << 6);
  }

  //ANDI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 0010 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, ANDI<Byte>, ea);
    bind(opcode | 1 << 6, ANDI<Word>, ea);
    bind(opcode | 2 << 6, ANDI<Long>, ea);
  }

  //ANDI_TO_CCR
  { auto opcode = pattern("0000 0010 0011 1100");

    bind(opcode, ANDI_TO_CCR);
  }

  //ANDI_TO_SR
  { auto opcode = pattern("0000 0010 0111 1100");

    bind(opcode, ANDI_TO_SR);
  }

  //BCC
  for(uint4 condition    : range( 16))
  for(uint8 displacement : range(256)) {
    auto opcode = pattern("0110 ---- ---- ----") | condition << 8 | displacement << 0;

    bind(opcode, BCC, condition, displacement);
  }

  //BTST (register)
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 ---1 00-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode == 1) continue;

    DataRegister dr{dreg};
    EffectiveAddress ea{mode, reg};
    if(mode == 0) bind(opcode, BTST<Long>, dr, ea);
    if(mode != 0) bind(opcode, BTST<Byte>, dr, ea);
  }

  //BTST (immediate)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 1000 00-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg == 2)) continue;

    EffectiveAddress ea{mode, reg};
    if(mode == 0) bind(opcode, BTST<Long>, ea);
    if(mode != 0) bind(opcode, BTST<Byte>, ea);
  }

  //CLR
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 0010 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, CLR<Byte>, ea);
    bind(opcode | 1 << 6, CLR<Word>, ea);
    bind(opcode | 2 << 6, CLR<Long>, ea);
  }

  //CMP
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1011 ---0 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;

    DataRegister dr{dreg};
    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, CMP<Byte>, dr, ea);
    bind(opcode | 1 << 6, CMP<Word>, dr, ea);
    bind(opcode | 2 << 6, CMP<Long>, dr, ea);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  //DBCC
  for(uint4 condition : range(16))
  for(uint3 dreg      : range( 8)) {
    auto opcode = pattern("0101 ---- 1100 1---") | condition << 8 | dreg << 0;

    DataRegister dr{dreg};
    bind(opcode, DBCC, condition, dr);
  }

  //EORI_TO_CCR
  { auto opcode = pattern("0000 1010 0011 1100");

    bind(opcode, EORI_TO_CCR);
  }

  //EORI_TO_SR
  { auto opcode = pattern("0000 1010 0111 1100");

    bind(opcode, EORI_TO_SR);
  }

  //LEA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 ---1 11-- ----") | areg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || mode == 3 || mode == 4 || (mode == 7 && reg == 4)) continue;

    AddressRegister ar{areg};
    EffectiveAddress ea{mode, reg};
    bind(opcode, LEA, ar, ea);
  }

  //MOVE
  for(uint3 toReg    : range(8))
  for(uint3 toMode   : range(8))
  for(uint3 fromMode : range(8))
  for(uint3 fromReg  : range(8)) {
    auto opcode = pattern("00++ ---- ---- ----") | toReg << 9 | toMode << 6 | fromMode << 3 | fromReg << 0;
    if(toMode == 1 || (toMode == 7 && toReg >= 2)) continue;

    EffectiveAddress to{toMode, toReg};
    EffectiveAddress from{fromMode, fromReg};
    bind(opcode | 1 << 12, MOVE<Byte>, to, from);
    bind(opcode | 3 << 12, MOVE<Word>, to, from);
    bind(opcode | 2 << 12, MOVE<Long>, to, from);

    if(fromMode == 1) unbind(opcode | 1 << 12);
  }

  //MOVEA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("00++ ---0 01-- ----") | areg << 9 | mode << 3 | reg << 0;

    AddressRegister ar{areg};
    EffectiveAddress ea{mode, reg};
    bind(opcode | 3 << 12, MOVEA<Word>, ar, ea);
    bind(opcode | 2 << 12, MOVEA<Long>, ar, ea);
  }

  //MOVEM
  for(uint1 direction : range(2))
  for(uint3 mode      : range(8))
  for(uint3 reg       : range(8)) {
    auto opcode = pattern("0100 1-00 1+-- ----") | direction << 10 | mode << 3 | reg << 0;
    if(direction == 0 && (mode <= 1 || mode == 3 || (mode == 7 && reg >= 2)));
    if(direction == 1 && (mode <= 1 || mode == 4 || (mode == 7 && reg == 4)));

    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, MOVEM<Word>, direction, ea);
    bind(opcode | 1 << 6, MOVEM<Long>, direction, ea);
  }

  //MOVEQ
  for(uint3 dreg      : range(  8))
  for(uint8 immediate : range(256)) {
    auto opcode = pattern("0111 ---0 ---- ----") | dreg << 9 | immediate << 0;

    DataRegister dr{dreg};
    bind(opcode, MOVEQ, dr, immediate);
  }

  //MOVE_FROM_SR
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 0000 11-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode, MOVE_FROM_SR, ea);
  }

  //MOVE_TO_CCR
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 0100 11-- ----") | mode << 3 | reg << 0;
    if(mode == 1) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode, MOVE_TO_CCR, ea);
  }

  //MOVE_TO_SR
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 0110 11-- ----") | mode << 3 | reg << 0;
    if(mode == 1) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode, MOVE_TO_SR, ea);
  }

  //MOVE_USP
  for(uint1 direction : range(2))
  for(uint3 areg      : range(8)) {
    auto opcode = pattern("0100 1110 0110 ----") | direction << 3 | areg << 0;

    AddressRegister ar{areg};
    bind(opcode, MOVE_USP, direction, ar);
  }

  //NOP
  { auto opcode = pattern("0100 1110 0111 0001");

    bind(opcode, NOP);
  }

  //ORI_TO_CCR
  { auto opcode = pattern("0000 0000 0011 1100");

    bind(opcode, ORI_TO_CCR);
  }

  //ORI_TO_SR
  { auto opcode = pattern("0000 0000 0111 1100");

    bind(opcode, ORI_TO_SR);
  }

  //RTS
  { auto opcode = pattern("0100 1110 0111 0101");

    bind(opcode, RTS);
  }

  //TST
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 1010 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 2) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, TST<Byte>, ea);
    bind(opcode | 1 << 6, TST<Word>, ea);
    bind(opcode | 2 << 6, TST<Long>, ea);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  #undef bind
  #undef unbind
  #undef pattern

  uint unimplemented = 0;
  for(uint16 opcode : range(65536)) {
    if(instructionTable[opcode]) continue;
    instructionTable[opcode] = [=] { trap(); };
    disassembleTable[opcode] = [=] { return string{"???"}; };
    unimplemented++;
  }
  print("[M68K] unimplemented opcodes: ", unimplemented, "\n");
}
