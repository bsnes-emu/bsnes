string ARM::disassemble_arm_opcode(uint32 pc) {
  static string conditions[] = {
    "eq", "ne", "cs", "cc",
    "mi", "pl", "vs", "vc",
    "hi", "ls", "ge", "lt",
    "gt", "le", "",   "nv",
  };

  static string opcodes[] = {
    "and", "eor", "sub", "rsb",
    "add", "adc", "sbc", "rsc",
    "tst", "teq", "cmp", "cmn",
    "orr", "mov", "bic", "mvn",
  };

  static string registers[] = {
    "r0",  "r1", "r2",  "r3",
    "r4",  "r5", "r6",  "r7",
    "r8",  "r9", "r10", "r11",
    "r12", "sp", "lr",  "pc",
  };

  static string indices[] = {
    "da", "ia", "db", "ib",
  };

  static auto is_move = [](uint4 opcode) { return opcode == 13 || opcode == 15; };
  static auto is_comp = [](uint4 opcode) { return opcode >= 8 && opcode <= 11; };
  static auto is_math = [](uint4 opcode) { return opcode < 8 || opcode == 12 || opcode == 14; };

  string output{hex<8>(pc), "  "};

  uint32 instruction = bus.read(pc, Word);
  output.append(hex<8>(instruction), "  ");

  //multiply()
  //mul{condition}{s} rd,rm,rs
  //mla{condition}{s} rd,rm,rs,rn
  if((instruction & 0x0fc000f0) == 0x00000090) {
    uint4 condition = instruction >> 28;
    uint1 accumulate = instruction >> 21;
    uint1 save = instruction >> 20;
    uint4 rd = instruction >> 16;
    uint4 rn = instruction >> 12;
    uint4 rs = instruction >> 8;
    uint4 rm = instruction >> 0;

    output.append(accumulate ? "mla" : "mul", conditions[condition], save ? "s " : " ");
    output.append(registers[rd], ",", registers[rm], ",", registers[rs]);
    if(accumulate) output.append(",", registers[rn]);

    return output;
  }

  //move_to_register_from_status()
  //mrs{condition} rd,(c,s)psr
  if((instruction & 0x0fb000f0) == 0x01000000) {
    uint4 condition = instruction >> 28;
    uint1 psr = instruction >> 22;
    uint4 rd = instruction >> 12;

    output.append("mrs", conditions[condition], " ");
    output.append(registers[rd], ",", psr ? "spsr" : "cpsr");

    return output;
  }

  //move_to_status_from_register()
  //msr{condition} (c,s)psr:{fields},rm
  if((instruction & 0x0fb000f0) == 0x01200000) {
    uint4 condition = instruction >> 28;
    uint1 psr = instruction >> 22;
    uint4 field = instruction >> 16;
    uint4 rm = instruction;

    output.append("msr", conditions[condition], " ");
    output.append(psr ? "spsr:" : "cpsr:");
    output.append(
      field & 1 ? "c" : "",
      field & 2 ? "x" : "",
      field & 4 ? "s" : "",
      field & 8 ? "f" : ""
    );
    output.append(",", registers[rm]);

    return output;
  }

  //branch_exchange_register()
  //bx{condition} rm
  if((instruction & 0x0ff000f0) == 0x01200010) {
    uint4 condition = instruction >> 28;
    uint4 rm = instruction;

    output.append("bx", conditions[condition], " ");
    output.append(registers[rm]);

    return output;
  }

  //data_immediate_shift()
  //{opcode}{condition}{s} rd,rm {shift} #immediate
  //{opcode}{condition} rn,rm {shift} #immediate
  //{opcode}{condition}{s} rd,rn,rm {shift} #immediate
  if((instruction & 0x0e000010) == 0x00000000) {
    uint4 condition = instruction >> 28;
    uint4 opcode = instruction >> 21;
    uint1 save = instruction >> 20;
    uint4 rn = instruction >> 16;
    uint4 rd = instruction >> 12;
    uint5 shift = instruction >> 7;
    uint2 op = instruction >> 5;
    uint4 rm = instruction;

    output.append(opcodes[opcode], conditions[condition]);
    if(is_move(opcode)) output.append(save ? "s " : " ", registers[rd]);
    if(is_comp(opcode)) output.append(" ", registers[rn]);
    if(is_math(opcode)) output.append(save ? "s " : " ", registers[rd], ",", registers[rn]);
    output.append(",", registers[rm]);
    if(op == 0 && shift != 0) output.append(" lsl #", shift);
    if(op == 1) output.append(" lsr #", shift == 0 ? 32u : (unsigned)shift);
    if(op == 2) output.append(" asr #", shift == 0 ? 32u : (unsigned)shift);
    if(op == 3 && shift != 0) output.append(" ror #", shift);
    if(op == 3 && shift == 0) output.append(" rrx");

    return output;
  }

  //data_register_shift()
  //{opcode}{condition}{s} rd,rm {shift} rs
  //{opcode}{condition} rn,rm {shift} rs
  //{opcode}{condition}{s} rd,rn,rm {shift} rs
  if((instruction & 0x0e000090) == 0x00000010) {
    uint4 condition = instruction >> 28;
    uint4 opcode = instruction >> 21;
    uint1 save = instruction >> 20;
    uint4 rn = instruction >> 16;
    uint4 rd = instruction >> 12;
    uint4 rs = instruction >> 8;
    uint2 mode = instruction >> 5;
    uint4 rm = instruction;

    output.append(opcodes[opcode], conditions[condition]);
    if(is_move(opcode)) output.append(save ? "s " : " ", registers[rd], ",");
    if(is_comp(opcode)) output.append(registers[rn], ",");
    if(is_math(opcode)) output.append(save ? "s " : " ", registers[rd], ",", registers[rn], ",");
    output.append(registers[rm]);
    if(mode == 0) output.append(" lsl ");
    if(mode == 1) output.append(" lsr ");
    if(mode == 2) output.append(" asr ");
    if(mode == 3) output.append(" ror ");
    output.append(registers[rs]);

    return output;
  }

  //data_immediate()
  //{opcode}{condition}{s} rd,#immediate
  //{opcode}{condition} rn,#immediate
  //{opcode}{condition}{s} rd,rn,#immediate
  if((instruction & 0x0e000000) == 0x02000000) {
    uint4 condition = instruction >> 28;
    uint4 opcode = instruction >> 21;
    uint1 save = instruction >> 20;
    uint4 rn = instruction >> 16;
    uint4 rd = instruction >> 12;
    uint4 rotate = instruction >> 8;
    uint8 immediate = instruction;

    uint32 rm = (immediate >> (rotate << 1)) | (immediate << (32 - (rotate << 1)));
    output.append(opcodes[opcode], conditions[condition]);
    if(is_move(opcode)) output.append(save ? "s " : " ", registers[rd]);
    if(is_comp(opcode)) output.append(" ", registers[rn]);
    if(is_math(opcode)) output.append(save ? "s " : " ", registers[rd], ",", registers[rn]);
    output.append(",#0x", hex<8>(rm));

    return output;
  }

  //move_immediate_offset()
  //(ldr,str){condition}{b} rd,[rn{,+/-offset}]{!}
  //(ldr,str){condition}{b} rd,[rn]{,+/-offset}
  if((instruction & 0x0e000000) == 0x04000000) {
    uint4 condition = instruction >> 28;
    uint1 pre = instruction >> 24;
    uint1 up = instruction >> 23;
    uint1 byte = instruction >> 22;
    uint1 writeback = instruction >> 21;
    uint1 load = instruction >> 20;
    uint4 rn = instruction >> 16;
    uint4 rd = instruction >> 12;
    uint12 immediate = instruction;

    output.append(load ? "ldr" : "str", conditions[condition], byte ? "b " : " ");
    output.append(registers[rd], ",[", registers[rn]);
    if(pre == 0) output.append("]");
    if(immediate) output.append(",", up ? "+" : "-", "0x", hex<3>(immediate));
    if(pre == 1) output.append("]");
    if(pre == 0 || writeback == 1) output.append("!");

    if(rn == 15) output.append(" =0x", hex<8>(bus.read(pc + 8 + (up ? +immediate : -immediate), byte ? Byte : Word)));
    return output;
  }

  //move_register_offset()
  //(ldr,str){condition}{b} rd,[rn,rm {mode} #immediate]{1}
  //(ldr,str){condition}{b} rd,[rn],rm {mode} #immediate
  if((instruction & 0x0e000010) == 0x06000000) {
    uint4 condition = instruction >> 28;
    uint1 pre = instruction >> 24;
    uint1 up = instruction >> 23;
    uint1 byte = instruction >> 22;
    uint1 writeback = instruction >> 21;
    uint1 load = instruction >> 20;
    uint4 rn = instruction >> 16;
    uint4 rd = instruction >> 12;
    uint5 shift = instruction >> 7;
    uint2 mode = instruction >> 5;
    uint4 rm = instruction;

    output.append(load ? "ldr" : "str", conditions[condition], byte ? "b " : " ");
    output.append(registers[rd], ",[", registers[rn]);
    if(pre == 0) output.append("]");
    output.append(",", up ? "+" : "-", registers[rm]);
    if(mode == 0 && shift != 0) output.append(" lsl #", shift);
    if(mode == 1) output.append(" lsr #", shift == 0 ? 32u : (unsigned)shift);
    if(mode == 2) output.append(" asr #", shift == 0 ? 32u : (unsigned)shift);
    if(mode == 3 && shift != 0) output.append(" ror #", shift);
    if(mode == 3 && shift == 0) output.append(" rrx");
    if(pre == 1) output.append("]");
    if(pre == 0 || writeback == 1) output.append("!");

    return output;
  }

  //move_multiple()
  //(ldm,stm) {condition}{mode} rn{!},{r...}{^}
  if((instruction & 0x0e000000) == 0x08000000) {
    uint4 condition = instruction >> 28;
    uint2 index = instruction >> 23;
    uint1 s = instruction >> 22;
    uint1 writeback = instruction >> 21;
    uint1 load = instruction >> 20;
    uint4 rn = instruction >> 16;
    uint16 list = instruction;

    output.append(load ? "ldm" : "stm", conditions[condition], indices[index], " ");
    output.append(registers[rn], writeback ? "!" : "", ",{");
    for(unsigned n = 0; n < 16; n++) if(list & (1 << n)) output.append(registers[n], ",");
    output.rtrim<1>(",");
    output.append("}", s ? "^" : "");

    return output;
  }

  //branch()
  //b{l}{condition} address
  if((instruction & 0x0e000000) == 0x0a000000) {
    uint4 condition = instruction >> 28;
    uint1 link = instruction >> 24;

    output.append("b", link ? "l" : "", conditions[condition], " ");
    output.append("0x", hex<8>(pc + 8 + (int24)instruction * 4));

    return output;
  }

  output.append("???");
  return output;
}

string ARM::disassemble_thumb_opcode(uint32 pc) {
  static string opcodes[] = { "mov", "cmp", "add", "sub" };

  static string registers[] = {
    "r0",  "r1", "r2",  "r3",
    "r4",  "r5", "r6",  "r7",
    "r8",  "r9", "r10", "r11",
    "r12", "sp", "lr",  "pc",
  };

  string output{hex<8>(pc), "  "};

  uint16 instruction = bus.read(pc, Half);
  output.append(hex<4>(instruction), "  ");

  //immediate()
  //(mov,cmp,add,sub) (rd,rn),#immediate
  if((instruction & 0xe000) == 0x2000) {
    uint2 opcode = instruction >> 11;
    uint3 rd = instruction >> 8;
    uint8 immediate = instruction;

    output.append(opcodes[opcode], " ", registers[rd], ",#0x", hex<2>(immediate));

    return output;
  }

  output.append("???");
  return output;
}

string ARM::disassemble_registers() {
  string output;
  output.append( "r0:", hex<8>(r( 0)), " r1:", hex<8>(r( 1)), "  r2:", hex<8>(r( 2)), "  r3:", hex<8>(r( 3)), "  ");
  output.append( "r4:", hex<8>(r( 4)), " r5:", hex<8>(r( 5)),  " r6:", hex<8>(r( 6)),  " r7:", hex<8>(r( 7)), " ");
  output.append("cpsr:", cpsr().n ? "N" : "n", cpsr().z ? "Z" : "z", cpsr().c ? "C" : "c", cpsr().v ? "V" : "v", "\n");
  output.append( "r8:", hex<8>(r( 8)), " r9:", hex<8>(r( 9)), " r10:", hex<8>(r(10)), " r11:", hex<8>(r(11)), " ");
  output.append("r12:", hex<8>(r(12)), " sp:", hex<8>(r(13)),  " lr:", hex<8>(r(14)),  " pc:", hex<8>(r(15)), " ");
  output.append("spsr:");
  if(mode() == Processor::Mode::USR || mode() == Processor::Mode::SYS) { output.append("----"); return output; }
  output.append(         spsr().n ? "N" : "n", spsr().z ? "Z" : "z", spsr().c ? "C" : "c", spsr().v ? "V" : "v");
  return output;
}
