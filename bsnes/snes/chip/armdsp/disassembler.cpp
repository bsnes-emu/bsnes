#ifdef ARMDSP_CPP

string ArmDSP::disassemble_opcode(uint32 pc) {
  static string conditions[] = { "eq", "ne", "cs", "cc", "mi" ,"pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "al", "nv" };
  static string opcodes[] = { "and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc", "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn" };

  string output{hex<8>(pc), "  "};

  uint32 instruction = bus_read(pc);
  output.append(hex<8>(instruction), "  ");

  uint4 condition = instruction >> 28;
  uint4 opcode = instruction >> 21;
  uint4 rn = instruction >> 16;
  uint4 rd = instruction >> 12;
  uint4 rs = instruction >> 8;

  if((instruction & 0x0e000000) == 0x0a000000) {
    output.append("b");
    if(condition != 14) output.append(conditions[condition]);
    output.append(" 0x", hex<8>(pc + 8 + (int24)instruction * 4));
    return output;
  }

  if((instruction & 0x0e000000) == 0x02000000) {
    uint5 rotate = 2 * (uint4)(instruction >> 8);
    uint32 immediate = (uint8)instruction;
    immediate = (immediate >> rotate) | (immediate << (32 - rotate));
    output.append(opcodes[opcode], " r", rd, ",#0x", hex<8>(immediate));
    return output;
  }

  output.append("???");
  return output;
}

string ArmDSP::disassemble_registers() {
  return {
      "r0:", hex<8>(r[ 0]), "  r1:", hex<8>(r[ 1]), "  r2:", hex<8>(r[ 2]), "  r3:", hex<8>(r[ 3]),
    "  r4:", hex<8>(r[ 4]), "  r5:", hex<8>(r[ 5]), "  r6:", hex<8>(r[ 6]), "  r7:", hex<8>(r[ 7]), "\n",
      "r8:", hex<8>(r[ 8]), "  r9:", hex<8>(r[ 9]), " r10:", hex<8>(r[10]), " r11:", hex<8>(r[11]),
    " r12:", hex<8>(r[12]), " r13:", hex<8>(r[13]), " r14:", hex<8>(r[14]), " r15:", hex<8>(r[15]), "\n"
  };
}

#endif
