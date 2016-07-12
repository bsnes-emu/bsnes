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

  opcode = readWordPC();
  return instructionTable[opcode]();
}

M68K::M68K() {
  for(uint opcode : range(65536)) instructionTable[opcode] = [=] { trap(); };

  #define match(pattern) else if( \
    (opcode & std::integral_constant<uint16_t, bit::mask(pattern)>::value) \
    == std::integral_constant<uint16_t, bit::test(pattern)>::value \
  ) instructionTable[opcode] = [=]
  #define bit(n) (opcode >> n & 1)
  #define bits(hi, lo) ((opcode >> lo) & ((1 << (hi - lo + 1)) - 1))

  for(uint opcode : range(65536)) {
    if(0);

    //NOP
    match("0100 1110 0111 0001") {
      instructionNOP();
    };

    //MOVEM (direction, size, mode, register)
    match("0100 1-00 1--- ----") {
      instructionMOVEM(bit(10), EA{1 + bit(6), bits(5,3), bits(2,0)});
    };

    //TST (size, mode, register)
    match("0100 1010 ---- ----") {
      instructionTST(EA{bits(7,6), bits(5,3), bits(2,0)});
    };

    //LEA (An, mode, register)
    match("0100 ---1 11-- ----") {
      instructionLEA(bits(11,9), EA{Long, bits(5,3), bits(2,0)});
    };

    //BCC (condition, displacement)
    match("0110 ---- ---- ----") {
      instructionBCC(bits(11,8), bits(7,0));
    };
  }

  #undef match
  #undef bit
  #undef bits
}
