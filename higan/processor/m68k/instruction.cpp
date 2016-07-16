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
  #define match(pattern) if( \
    (opcode & std::integral_constant<uint16_t, bit::mask(pattern)>::value) \
    == std::integral_constant<uint16_t, bit::test(pattern)>::value \
  )

  #define bind(name, ...) \
    instructionTable[opcode] = [=] { return instruction##name(__VA_ARGS__); }; \
    disassembleTable[opcode] = [=] { return disassemble##name(__VA_ARGS__); }; \

  #define bit(x) (uint)opcode.bit(x)
  #define bits(x, y) (uint)opcode.bits(x, y)

  for(uint16 opcode : range(65536)) {

    //ANDI
    match("0000 0010 ---- ----") {
      auto size = bits(7,6);
      auto mode = bits(5,3);
      auto reg = bits(2,0);

      size = size == 0 ? Byte : size == 1 ? Word : size == 2 ? Long : 0;
      if(size && mode != 1) {
        bind(ANDI, size, {mode, reg});
      }
    }

    //BCC
    match("0110 ---- ---- ----") {
      auto condition = bits(11,8);
      auto displacement = bits(7,0);

      if(true) {
        bind(BCC, condition, displacement);
      }
    }

    //LEA
    match("0100 ---1 11-- ----") {
      auto target = bits(11,9);
      auto mode = bits(5,3);
      auto reg = bits(2,0);

      if(mode == 2 || mode == 5 || mode == 6 || (mode == 7 && reg <= 4)) {
        bind(LEA, target, {mode, reg});
      }
    }

    //MOVE
    match("00-- ---- ---- ----") {
      auto size = bits(13,12);
      auto targetReg = bits(11,9);
      auto targetMode = bits(8,6);
      auto sourceMode = bits(5,3);
      auto sourceReg = bits(2,0);

      size = size == 1 ? Byte : size == 3 ? Word : size == 2 ? Long : 0;
      if(size && targetMode != 1) {
        bind(MOVE, size, {targetMode, targetReg}, {sourceMode, sourceReg});
      }
    }

    //MOVEA
    match("00-- ---0 01-- ----") {
      auto size = bits(13,12);
      auto target = bits(11,9);
      auto sourceMode = bits(5,3);
      auto sourceReg = bits(2,0);

      size = size == 3 ? Word : size == 2 ? Long : 0;
      if(size) {
        bind(MOVEA, size, target, {sourceMode, sourceReg});
      }
    }

    //MOVEM
    match("0100 1-00 1--- ----") {
      auto direction = bit(10);
      auto size = bit(6);
      auto mode = bits(5,3);
      auto reg = bits(2,0);

      size = size == 0 ? Word : size == 1 ? Long : 0;
      if((direction == 0 && (mode == 2 || mode == 4 || mode == 5 || mode == 6 || (mode == 7 && reg <= 3)))
      || (direction == 1 && (mode == 2 || mode == 3 || mode == 5 || mode == 6 || (mode == 7 && reg <= 3)))) {
        bind(MOVEM, direction, size, {mode, reg});
      }
    }

    //MOVEQ
    match("0111 ---0 ---- ----") {
      auto target = bits(11,9);
      auto immediate = bits(7,0);

      if(true) {
        bind(MOVEQ, target, immediate);
      }
    }

    //MOVE_USP
    match("0100 1110 0110 ----") {
      auto direction = bit(3);
      auto reg = bits(2,0);

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
      auto size = bits(7,6);
      auto mode = bits(5,3);
      auto reg = bits(2,0);

      size = size == 0 ? Byte : size == 1 ? Word : size == 2 ? Long : 0;
      if(size) {
        bind(TST, size, {mode, reg});
      }
    }

  }

  for(uint16 opcode : range(65536)) {
    if(instructionTable[opcode]) continue;
    instructionTable[opcode] = [=] { trap(); };
    disassembleTable[opcode] = [=] { return string{"???"}; };
  }

  #undef match
  #undef bind
  #undef bit
  #undef bits
}
