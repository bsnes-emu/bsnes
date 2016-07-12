auto M68K::trap() -> void {
  instructionsExecuted--;
  r.pc -= 2;
  print("[M68K] unimplemented instruction: ", hex(r.pc, 6L), " = ", hex(opcode, 4L), "\n");
  print("[M68K] instructions executed: ", instructionsExecuted, "\n");
  while(true) step(5);
}

auto M68K::instruction() -> void {
  instructionsExecuted++;
  print(disassemble(r.pc), "\n");

  opcode = readWordPC();

  //bcc
  //0110 cccc dddd dddd
  if(opcode >> 12 == 0b0110) {
    return instructionBCC(opcode >> 8, opcode >> 0);
  }

  //nop
  //0100 1110 0111 0001
  if(opcode == 0b0100'1110'0111'0001) {
    return instructionNOP();
  }

  //tst
  //0100 1010 ssmm mrrr
  if(opcode >> 8 == 0b0100'1010) {
    return instructionTST(opcode >> 6, opcode >> 3, opcode >> 0);
  }

  trap();
}
