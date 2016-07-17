auto M68K::trap() -> void {
  instructionsExecuted--;
  r.pc -= 2;
  print("[M68K] unimplemented instruction: ", hex(r.pc, 6L), " = ", hex(opcode, 4L), "\n");
  print("[M68K] instructions executed: ", instructionsExecuted, "\n");
  while(true) step(5);
}

auto M68K::instruction() -> void {
  instructionsExecuted++;
  print(disassembleRegisters(), "\n", disassemble(r.pc), "\n\n");

  opcode = readPC();
  return instructionTable[opcode]();
}

M68K::M68K() {
  #define bind(id, name, ...) \
    assert(!instructionTable[id]); \
    instructionTable[id] = [=] { return instruction##name(__VA_ARGS__); }; \
    disassembleTable[id] = [=] { return disassemble##name(__VA_ARGS__); }; \

  //ADD
  for(uint3 d         : range(8))
  for(uint1 direction : range(2))
  for(uint3 mode      : range(8))
  for(uint3 reg       : range(8)) {
    auto opcode = 0b1101'0000'0000'0000 | d << 9 | direction << 8 | mode << 3 | reg << 0;
    if(direction == 1 && (mode == 0 || mode == 1 || (mode == 7 && reg >= 2))) continue;

    EA ea{mode, reg};
    bind(opcode | 0 << 6, ADD<Byte>, d, direction, ea);
    bind(opcode | 1 << 6, ADD<Word>, d, direction, ea);
    bind(opcode | 2 << 6, ADD<Long>, d, direction, ea);
  }

  #undef bind

  #define match(pattern) if( \
    (opcode & std::integral_constant<uint16_t, bit::mask(pattern)>::value) \
    == std::integral_constant<uint16_t, bit::test(pattern)>::value \
  )

  #define bind(name, ...) \
    assert(!instructionTable[opcode]); \
    instructionTable[opcode] = [=] { return instruction##name(__VA_ARGS__); }; \
    disassembleTable[opcode] = [=] { return disassemble##name(__VA_ARGS__); }; \

  #define bit(x) (uint)opcode.bit(x)
  #define bits(x, y) (uint)opcode.bits(x, y)

  for(uint16 opcode : range(65536)) {

/*
    //ADD
    match("1101 ---- ---- ----") {
      uint3 r = bits(11,9);
      uint1 direction = bit(8);
      uint2 size = bits(7,6);
      uint3 mode = bits(5,3);
      uint3 reg = bits(2,0);

      if(size != 3 && (direction == 0 || (mode == 2 || mode == 3 || mode == 4 || mode == 5 || mode == 6 || (mode == 7 && reg <= 1)))) {
        uint8 ea = mode << 5 | reg << 2 | size;
        bind(ADD, r, direction, ea);
      }
    }
*/

    //ANDI
    match("0000 0010 ---- ----") {
      uint2 size = bits(7,6);
      uint3 mode = bits(5,3);
      uint3 reg = bits(2,0);

      if(size != 3 && mode != 1) {
        uint8 ea = mode << 5 | reg << 2 | size;
        bind(ANDI, ea);
      }
    }

    //BCC
    match("0110 ---- ---- ----") {
      uint4 condition = bits(11,8);
      uint8 displacement = bits(7,0);

      if(true) {
        bind(BCC, condition, displacement);
      }
    }

    //LEA
    match("0100 ---1 11-- ----") {
      uint3 target = bits(11,9);
      uint3 mode = bits(5,3);
      uint3 reg = bits(2,0);

      if(mode == 2 || mode == 5 || mode == 6 || (mode == 7 && reg <= 4)) {
        uint8 ea = mode << 5 | reg << 2 | Long;
        bind(LEA, target, ea);
      }
    }

    //MOVE
    match("00-- ---- ---- ----") {
      uint2 size = bits(13,12) == 1 ? Byte : bits(13,12) == 3 ? Word : bits(13,12) == 2 ? Long : 3;
      uint3 targetReg = bits(11,9);
      uint3 targetMode = bits(8,6);
      uint3 sourceMode = bits(5,3);
      uint3 sourceReg = bits(2,0);

      if(size != 3 && targetMode != 1) {
        uint8 to = targetMode << 5 | targetReg << 2 | size;
        uint8 from = sourceMode << 5 | sourceReg << 2 | size;
        bind(MOVE, to, from);
      }
    }

    //MOVEA
    match("00-- ---0 01-- ----") {
      uint2 size = bits(13,12) == 3 ? Word : bits(13,12) == 2 ? Long : 3;
      uint3 to = bits(11,9);
      uint3 mode = bits(5,3);
      uint3 reg = bits(2,0);

      if(size != 3) {
        uint8 from = mode << 5 | reg << 2 | size;
        bind(MOVEA, to, from);
      }
    }

    //MOVEM
    match("0100 1-00 1--- ----") {
      uint1 direction = bit(10);
      uint2 size = 1 + bit(6);
      uint3 mode = bits(5,3);
      uint3 reg = bits(2,0);

      if((direction == 0 && (mode == 2 || mode == 4 || mode == 5 || mode == 6 || (mode == 7 && reg <= 3)))
      || (direction == 1 && (mode == 2 || mode == 3 || mode == 5 || mode == 6 || (mode == 7 && reg <= 3)))) {
        uint8 ea = mode << 5 | reg << 2 | size;
        bind(MOVEM, direction, ea);
      }
    }

    //MOVEQ
    match("0111 ---0 ---- ----") {
      uint3 target = bits(11,9);
      uint8 immediate = bits(7,0);

      if(true) {
        bind(MOVEQ, target, immediate);
      }
    }

    //MOVE_USP
    match("0100 1110 0110 ----") {
      uint1 direction = bit(3);
      uint3 reg = bits(2,0);

      if(true) {
        bind(MOVE_USP, direction, reg);
      }
    }

    //NOP
    match("0100 1110 0111 0001") {
      if(true) {
        bind(NOP);
      }
    }

    //TST
    match("0100 1010 ---- ----") {
      uint2 size = bits(7,6);
      uint3 mode = bits(5,3);
      uint3 reg = bits(2,0);

      if(size != 3) {
        uint8 ea = mode << 5 | reg << 2 | size << 0;
        bind(TST, ea);
      }
    }

  }

  #undef match
  #undef bind
  #undef bit
  #undef bits

  for(uint16 opcode : range(65536)) {
    if(instructionTable[opcode]) continue;
    instructionTable[opcode] = [=] { trap(); };
    disassembleTable[opcode] = [=] { return string{"???"}; };
  }
}
