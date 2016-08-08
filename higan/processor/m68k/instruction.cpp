auto M68K::trap() -> void {
  instructionsExecuted--;
  r.pc -= 2;
  print("[M68K] unimplemented instruction: ", hex(r.pc, 6L), " = ", hex(opcode, 4L), "\n");
  print("[M68K] instructions executed: ", instructionsExecuted, "\n");
  while(true) step(5);
}

auto M68K::instruction() -> void {
  instructionsExecuted++;

//if(instructionsExecuted >= 2000010) trap();

//if(instructionsExecuted >= 2000000) {
//  print(disassembleRegisters(), "\n");
//  print(disassemble(r.pc), "\n");
//  print("\n");
//}

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
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1101 ---0 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 5) continue;

    EffectiveAddress from{mode, reg};
    DataRegister with{dreg};
    bind(opcode | 0 << 6, ADD<Byte>, from, with);
    bind(opcode | 1 << 6, ADD<Word>, from, with);
    bind(opcode | 2 << 6, ADD<Long>, from, with);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  //ADD
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1101 ---1 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    DataRegister from{dreg};
    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, ADD<Byte>, from, with);
    bind(opcode | 1 << 6, ADD<Word>, from, with);
    bind(opcode | 2 << 6, ADD<Long>, from, with);
  }

  //ADDA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1101 ---+ 11-- ----") | areg << 9 | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 5) continue;

    AddressRegister ar{areg};
    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 8, ADDA<Word>, ar, ea);
    bind(opcode | 1 << 8, ADDA<Long>, ar, ea);
  }

  //ADDI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 0110 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode | 0 << 6, ADDI<Byte>, modify);
    bind(opcode | 1 << 6, ADDI<Word>, modify);
    bind(opcode | 2 << 6, ADDI<Long>, modify);
  }

  //ADDQ
  for(uint3 data : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0101 ---0 ++-- ----") | data << 9 | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 2) continue;

    uint4 immediate = data ? (uint4)data : (uint4)8;
    EffectiveAddress modify{mode, reg};
    bind(opcode | 0 << 6, ADDQ<Byte>, immediate, modify);
    bind(opcode | 1 << 6, ADDQ<Word>, immediate, modify);
    bind(opcode | 2 << 6, ADDQ<Long>, immediate, modify);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  //ADDX
  for(uint3 treg : range(8))
  for(uint3 sreg : range(8)) {
    auto opcode = pattern("1101 ---1 ++00 ----") | treg << 9 | sreg << 0;

    EffectiveAddress dataTarget{DataRegisterDirect, treg};
    EffectiveAddress dataSource{DataRegisterDirect, sreg};
    bind(opcode | 0 << 6 | 0 << 3, ADDX<Byte>, dataTarget, dataSource);
    bind(opcode | 1 << 6 | 0 << 3, ADDX<Word>, dataTarget, dataSource);
    bind(opcode | 2 << 6 | 0 << 3, ADDX<Long>, dataTarget, dataSource);

    EffectiveAddress addressTarget{AddressRegisterIndirectWithPreDecrement, treg};
    EffectiveAddress addressSource{AddressRegisterIndirectWithPreDecrement, sreg};
    bind(opcode | 0 << 6 | 1 << 3, ADDX<Byte>, addressTarget, addressSource);
    bind(opcode | 1 << 6 | 1 << 3, ADDX<Word>, addressTarget, addressSource);
    bind(opcode | 2 << 6 | 1 << 3, ADDX<Long>, addressTarget, addressSource);
  }

  //AND
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1100 ---0 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 5)) continue;

    EffectiveAddress from{mode, reg};
    DataRegister with{dreg};
    bind(opcode | 0 << 6, AND<Byte>, from, with);
    bind(opcode | 1 << 6, AND<Word>, from, with);
    bind(opcode | 2 << 6, AND<Long>, from, with);
  }

  //AND
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1100 ---1 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    DataRegister from{dreg};
    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, AND<Byte>, from, with);
    bind(opcode | 1 << 6, AND<Word>, from, with);
    bind(opcode | 2 << 6, AND<Long>, from, with);
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

  //ASL (immediate)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---1 ++00 0---") | count << 9 | dreg << 0;

    auto shift = count ? (uint4)count : (uint4)8;
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ASL<Byte>, shift, modify);
    bind(opcode | 1 << 6, ASL<Word>, shift, modify);
    bind(opcode | 2 << 6, ASL<Long>, shift, modify);
  }

  //ASL (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---1 ++10 0---") | sreg << 9 | dreg << 0;

    DataRegister shift{sreg};
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ASL<Byte>, shift, modify);
    bind(opcode | 1 << 6, ASL<Word>, shift, modify);
    bind(opcode | 2 << 6, ASL<Long>, shift, modify);
  }

  //ASL (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0001 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode, ASL, modify);
  }

  //ASR (immediate)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---0 ++00 0---") | count << 9 | dreg << 0;

    auto shift = count ? (uint4)count : (uint4)8;
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ASR<Byte>, shift, modify);
    bind(opcode | 1 << 6, ASR<Word>, shift, modify);
    bind(opcode | 2 << 6, ASR<Long>, shift, modify);
  }

  //ASR (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---0 ++10 0---") | sreg << 9 | dreg << 0;

    DataRegister shift{sreg};
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ASR<Byte>, shift, modify);
    bind(opcode | 1 << 6, ASR<Word>, shift, modify);
    bind(opcode | 2 << 6, ASR<Long>, shift, modify);
  }

  //ASR (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0000 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode, ASR, modify);
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
    if(mode == 1 || (mode == 7 && reg >= 5)) continue;

    DataRegister dr{dreg};
    EffectiveAddress ea{mode, reg};
    if(mode == 0) bind(opcode, BTST<Long>, dr, ea);
    if(mode != 0) bind(opcode, BTST<Byte>, dr, ea);
  }

  //BTST (immediate)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 1000 00-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && (reg == 2 || reg >= 5))) continue;

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
    if(mode == 7 && reg >= 5) continue;

    DataRegister dr{dreg};
    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, CMP<Byte>, dr, ea);
    bind(opcode | 1 << 6, CMP<Word>, dr, ea);
    bind(opcode | 2 << 6, CMP<Long>, dr, ea);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  //CMPA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1011 ---+ 11-- ----") | areg << 9 | mode << 3 | reg << 0;

    AddressRegister ar{areg};
    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 8, CMPA<Word>, ar, ea);
    bind(opcode | 1 << 8, CMPA<Long>, ar, ea);
  }

  //CMPI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 1100 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, CMPI<Byte>, ea);
    bind(opcode | 1 << 6, CMPI<Word>, ea);
    bind(opcode | 2 << 6, CMPI<Long>, ea);
  }

  //CMPM
  for(uint3 xreg : range(8))
  for(uint3 yreg : range(8)) {
    auto opcode = pattern("1011 ---1 ++00 1---") | xreg << 9 | yreg << 0;

    EffectiveAddress ax{AddressRegisterIndirectWithPostIncrement, xreg};
    EffectiveAddress ay{AddressRegisterIndirectWithPostIncrement, yreg};
    bind(opcode | 0 << 6, CMPM<Byte>, ax, ay);
    bind(opcode | 1 << 6, CMPM<Word>, ax, ay);
    bind(opcode | 2 << 6, CMPM<Long>, ax, ay);
  }

  //DBCC
  for(uint4 condition : range(16))
  for(uint3 dreg      : range( 8)) {
    auto opcode = pattern("0101 ---- 1100 1---") | condition << 8 | dreg << 0;

    DataRegister dr{dreg};
    bind(opcode, DBCC, condition, dr);
  }

  //EOR
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1011 ---1 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    DataRegister from{dreg};
    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, EOR<Byte>, from, with);
    bind(opcode | 1 << 6, EOR<Word>, from, with);
    bind(opcode | 2 << 6, EOR<Long>, from, with);
  }

  //EORI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 1010 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, EORI<Byte>, with);
    bind(opcode | 1 << 6, EORI<Word>, with);
    bind(opcode | 2 << 6, EORI<Long>, with);
  }

  //EORI_TO_CCR
  { auto opcode = pattern("0000 1010 0011 1100");

    bind(opcode, EORI_TO_CCR);
  }

  //EORI_TO_SR
  { auto opcode = pattern("0000 1010 0111 1100");

    bind(opcode, EORI_TO_SR);
  }

  //JSR
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 1110 10-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || mode == 3 || mode == 4 || (mode == 7 && reg >= 4)) continue;

    EffectiveAddress target{mode, reg};
    bind(opcode, JSR, target);
  }

  //LEA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 ---1 11-- ----") | areg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || mode == 3 || mode == 4 || (mode == 7 && reg >= 4)) continue;

    AddressRegister ar{areg};
    EffectiveAddress ea{mode, reg};
    bind(opcode, LEA, ar, ea);
  }

  //LSL (immediate)
  for(uint3 data : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---1 ++00 1---") | data << 9 | dreg << 0;

    auto immediate = data ? (uint4)data : (uint4)8;
    DataRegister dr{dreg};
    bind(opcode | 0 << 6, LSL<Byte>, immediate, dr);
    bind(opcode | 1 << 6, LSL<Word>, immediate, dr);
    bind(opcode | 2 << 6, LSL<Long>, immediate, dr);
  }

  //LSL (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---1 ++10 1---") | sreg << 9 | dreg << 0;

    DataRegister sr{sreg};
    DataRegister dr{dreg};
    bind(opcode | 0 << 6, LSL<Byte>, sr, dr);
    bind(opcode | 1 << 6, LSL<Word>, sr, dr);
    bind(opcode | 2 << 6, LSL<Long>, sr, dr);
  }

  //LSL (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0011 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode, LSL, ea);
  }

  //LSR (immediate)
  for(uint3 data : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---0 ++00 1---") | data << 9 | dreg << 0;

    auto immediate = data ? (uint4)data : (uint4)8;
    DataRegister dr{dreg};
    bind(opcode | 0 << 6, LSR<Byte>, immediate, dr);
    bind(opcode | 1 << 6, LSR<Word>, immediate, dr);
    bind(opcode | 2 << 6, LSR<Long>, immediate, dr);
  }

  //LSR (register)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---0 ++10 1---") | count << 9 | dreg << 0;

    DataRegister shift{count};
    DataRegister dr{dreg};
    bind(opcode | 0 << 6, LSR<Byte>, shift, dr);
    bind(opcode | 1 << 6, LSR<Word>, shift, dr);
    bind(opcode | 2 << 6, LSR<Long>, shift, dr);
  }

  //LSR (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0010 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode, LSR, ea);
  }

  //MOVE
  for(uint3 toReg    : range(8))
  for(uint3 toMode   : range(8))
  for(uint3 fromMode : range(8))
  for(uint3 fromReg  : range(8)) {
    auto opcode = pattern("00++ ---- ---- ----") | toReg << 9 | toMode << 6 | fromMode << 3 | fromReg << 0;
    if(toMode == 1 || (toMode == 7 && toReg >= 2)) continue;
    if(fromMode == 7 && fromReg >= 5) continue;

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
    if(mode == 7 && reg >= 5) continue;

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
    if(direction == 1 && (mode <= 1 || mode == 4 || (mode == 7 && reg >= 4)));

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
    if(mode == 1 || (mode == 7 && reg >= 5)) continue;

    EffectiveAddress ea{mode, reg};
    bind(opcode, MOVE_TO_CCR, ea);
  }

  //MOVE_TO_SR
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0100 0110 11-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 5)) continue;

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

  //OR
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1000 ---0 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 5)) continue;

    EffectiveAddress from{mode, reg};
    DataRegister with{dreg};
    bind(opcode | 0 << 6, OR<Byte>, from, with);
    bind(opcode | 1 << 6, OR<Word>, from, with);
    bind(opcode | 2 << 6, OR<Long>, from, with);
  }

  //OR
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1000 ---1 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    DataRegister from{dreg};
    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, OR<Byte>, from, with);
    bind(opcode | 1 << 6, OR<Word>, from, with);
    bind(opcode | 2 << 6, OR<Long>, from, with);
  }

  //ORI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 0000 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, ORI<Byte>, with);
    bind(opcode | 1 << 6, ORI<Word>, with);
    bind(opcode | 2 << 6, ORI<Long>, with);
  }

  //ORI_TO_CCR
  { auto opcode = pattern("0000 0000 0011 1100");

    bind(opcode, ORI_TO_CCR);
  }

  //ORI_TO_SR
  { auto opcode = pattern("0000 0000 0111 1100");

    bind(opcode, ORI_TO_SR);
  }

  //ROL (immediate)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---1 ++01 1---") | count << 9 | dreg << 0;

    auto shift = count ? (uint4)count : (uint4)8;
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROL<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROL<Word>, shift, modify);
    bind(opcode | 2 << 6, ROL<Long>, shift, modify);
  }

  //ROL (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---1 ++11 1---") | sreg << 9 | dreg << 0;

    DataRegister shift{sreg};
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROL<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROL<Word>, shift, modify);
    bind(opcode | 2 << 6, ROL<Long>, shift, modify);
  }

  //ROL (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0111 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode, ROL, modify);
  }

  //ROR (immediate)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---0 ++01 1---") | count << 9 | dreg << 0;

    auto shift = count ? (uint4)count : (uint4)8;
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROR<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROR<Word>, shift, modify);
    bind(opcode | 2 << 6, ROR<Long>, shift, modify);
  }

  //ROR (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---0 ++11 1---") | sreg << 9 | dreg << 0;

    DataRegister shift{sreg};
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROR<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROR<Word>, shift, modify);
    bind(opcode | 2 << 6, ROR<Long>, shift, modify);
  }

  //ROR (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0110 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode, ROR, modify);
  }

  //ROXL (immediate)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---1 ++01 0---") | count << 9 | dreg << 0;

    auto shift = count ? (uint4)count : (uint4)8;
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROXL<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROXL<Word>, shift, modify);
    bind(opcode | 2 << 6, ROXL<Long>, shift, modify);
  }

  //ROXL (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---1 ++11 0---") | sreg << 9 | dreg << 0;

    DataRegister shift{sreg};
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROXL<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROXL<Word>, shift, modify);
    bind(opcode | 2 << 6, ROXL<Long>, shift, modify);
  }

  //ROXL (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0101 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode, ROXL, modify);
  }

  //ROXR (immediate)
  for(uint3 count : range(8))
  for(uint3 dreg  : range(8)) {
    auto opcode = pattern("1110 ---0 ++01 0---") | count << 9 | dreg << 0;

    auto shift = count ? (uint4)count : (uint4)8;
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROXR<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROXR<Word>, shift, modify);
    bind(opcode | 2 << 6, ROXR<Long>, shift, modify);
  }

  //ROXR (register)
  for(uint3 sreg : range(8))
  for(uint3 dreg : range(8)) {
    auto opcode = pattern("1110 ---0 ++11 0---") | sreg << 9 | dreg << 0;

    DataRegister shift{sreg};
    DataRegister modify{dreg};
    bind(opcode | 0 << 6, ROXR<Byte>, shift, modify);
    bind(opcode | 1 << 6, ROXR<Word>, shift, modify);
    bind(opcode | 2 << 6, ROXR<Long>, shift, modify);
  }

  //ROXR (effective address)
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1110 0100 11-- ----") | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress modify{mode, reg};
    bind(opcode, ROXR, modify);
  }

  //RTS
  { auto opcode = pattern("0100 1110 0111 0101");

    bind(opcode, RTS);
  }

  //SUB
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1001 ---0 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 5) continue;

    EffectiveAddress source{mode, reg};
    DataRegister target{dreg};
    bind(opcode | 0 << 6, SUB<Byte>, source, target);
    bind(opcode | 1 << 6, SUB<Word>, source, target);
    bind(opcode | 2 << 6, SUB<Long>, source, target);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  //SUB
  for(uint3 dreg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1001 ---1 ++-- ----") | dreg << 9 | mode << 3 | reg << 0;
    if(mode <= 1 || (mode == 7 && reg >= 2)) continue;

    DataRegister source{dreg};
    EffectiveAddress target{mode, reg};
    bind(opcode | 0 << 6, SUB<Byte>, source, target);
    bind(opcode | 1 << 6, SUB<Word>, source, target);
    bind(opcode | 2 << 6, SUB<Long>, source, target);
  }

  //SUBA
  for(uint3 areg : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("1001 ---+ 11-- ----") | areg << 9 | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 5) continue;

    AddressRegister to{areg};
    EffectiveAddress from{mode, reg};
    bind(opcode | 0 << 8, SUBA<Word>, to, from);
    bind(opcode | 1 << 8, SUBA<Long>, to, from);
  }

  //SUBI
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0000 0100 ++-- ----") | mode << 3 | reg << 0;
    if(mode == 1 || (mode == 7 && reg >= 2)) continue;

    EffectiveAddress with{mode, reg};
    bind(opcode | 0 << 6, SUBI<Byte>, with);
    bind(opcode | 1 << 6, SUBI<Word>, with);
    bind(opcode | 2 << 6, SUBI<Long>, with);
  }

  //SUBQ
  for(uint3 data : range(8))
  for(uint3 mode : range(8))
  for(uint3 reg  : range(8)) {
    auto opcode = pattern("0101 ---1 ++-- ----") | data << 9 | mode << 3 | reg << 0;
    if(mode == 7 && reg >= 2) continue;

    auto immediate = data ? (uint4)data : (uint4)8;
    EffectiveAddress ea{mode, reg};
    bind(opcode | 0 << 6, SUBQ<Byte>, immediate, ea);
    bind(opcode | 1 << 6, SUBQ<Word>, immediate, ea);
    bind(opcode | 2 << 6, SUBQ<Long>, immediate, ea);

    if(mode == 1) unbind(opcode | 0 << 6);
  }

  //SUBX
  for(uint3 treg : range(8))
  for(uint3 sreg : range(8)) {
    auto opcode = pattern("1001 ---1 ++00 ----") | treg << 9 | sreg << 0;

    EffectiveAddress dataTarget{DataRegisterDirect, treg};
    EffectiveAddress dataSource{DataRegisterDirect, sreg};
    bind(opcode | 0 << 6 | 0 << 3, SUBX<Byte>, dataTarget, dataSource);
    bind(opcode | 1 << 6 | 0 << 3, SUBX<Word>, dataTarget, dataSource);
    bind(opcode | 2 << 6 | 0 << 3, SUBX<Long>, dataTarget, dataSource);

    EffectiveAddress addressTarget{AddressRegisterIndirectWithPreDecrement, treg};
    EffectiveAddress addressSource{AddressRegisterIndirectWithPreDecrement, sreg};
    bind(opcode | 0 << 6 | 1 << 3, SUBX<Byte>, addressTarget, addressSource);
    bind(opcode | 1 << 6 | 1 << 3, SUBX<Word>, addressTarget, addressSource);
    bind(opcode | 2 << 6 | 1 << 3, SUBX<Long>, addressTarget, addressSource);
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
