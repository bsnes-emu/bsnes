#ifdef PROCESSOR_ARM_HPP

void ARM::arm_step() {
  if(pipeline.reload) {
    pipeline.reload = false;

    pipeline.fetch.address = r(15);
    pipeline.fetch.instruction = bus_read(r(15), Word);

    r(15).data += 4;
    pipeline.decode = pipeline.fetch;
    pipeline.fetch.address = r(15);
    pipeline.fetch.instruction = bus_read(r(15), Word);
    step(2);
  }

  r(15).data += 4;
  pipeline.execute = pipeline.decode;
  pipeline.decode = pipeline.fetch;
  pipeline.fetch.address = r(15);
  pipeline.fetch.instruction = bus_read(r(15), Word);
  step(2);

//print(disassemble_registers(), "\n");
//print(disassemble_arm_instruction(pipeline.execute.address), "\n");

  if(arm_condition() == false) return;

  if((instruction() & 0x0fc000f0) == 0x00000090) { arm_op_multiply(); return; }
  if((instruction() & 0x0fb000f0) == 0x01000000) { arm_op_move_to_register_from_status(); return; }
  if((instruction() & 0x0fb000f0) == 0x01000090) { arm_op_memory_swap(); return; }
  if((instruction() & 0x0fb000f0) == 0x01200000) { arm_op_move_to_status_from_register(); return; }
  if((instruction() & 0x0ff000f0) == 0x01200010) { arm_op_branch_exchange_register(); return; }  //ARMv4
  if((instruction() & 0x0fb00000) == 0x03200000) { arm_op_move_to_status_from_immediate(); return; }

  if((instruction() & 0x0e000010) == 0x00000000) { arm_op_data_immediate_shift(); return; }
  if((instruction() & 0x0e000090) == 0x00000010) { arm_op_data_register_shift(); return; }
  if((instruction() & 0x0e000000) == 0x02000000) { arm_op_data_immediate(); return; }
  if((instruction() & 0x0e000000) == 0x04000000) { arm_op_move_immediate_offset(); return; }
  if((instruction() & 0x0e000010) == 0x06000000) { arm_op_move_register_offset(); return; }
  if((instruction() & 0x0e000000) == 0x08000000) { arm_op_move_multiple(); return; }
  if((instruction() & 0x0e000000) == 0x0a000000) { arm_op_branch(); return; }
  if((instruction() & 0x0f000000) == 0x0f000000) { arm_op_software_interrupt(); return; }

  exception = true;
}

bool ARM::arm_condition() {
  uint4 condition = instruction() >> 28;

  switch(condition) {
  case  0: return cpsr().z == 1;                          //EQ (equal)
  case  1: return cpsr().z == 0;                          //NE (not equal)
  case  2: return cpsr().c == 1;                          //CS (carry set)
  case  3: return cpsr().c == 0;                          //CC (carry clear)
  case  4: return cpsr().n == 1;                          //MI (negative)
  case  5: return cpsr().n == 0;                          //PL (positive)
  case  6: return cpsr().v == 1;                          //VS (overflow)
  case  7: return cpsr().v == 0;                          //VC (no overflow)
  case  8: return cpsr().c == 1 && cpsr().z == 0;         //HI (unsigned higher)
  case  9: return cpsr().c == 0 || cpsr().z == 1;         //LS (unsigned lower or same)
  case 10: return cpsr().n == cpsr().v;                   //GE (signed greater than or equal)
  case 11: return cpsr().n != cpsr().v;                   //LT (signed less than)
  case 12: return cpsr().z == 0 && cpsr().n == cpsr().v;  //GT (signed greater than)
  case 13: return cpsr().z == 1 || cpsr().n != cpsr().v;  //LE (signed less than or equal)
  case 14: return true;                                   //AL (always)
  case 15: return false;                                  //NV (never)
  }
}

void ARM::arm_opcode(uint32 rm) {
  uint4 opcode = instruction() >> 21;
  uint1 save = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;

  uint32 rn = r(n);

  auto test = [&](uint32 result) {
    if(save) {
      cpsr().n = result >> 31;
      cpsr().z = result == 0;
      cpsr().c = carryout();
    }
    return result;
  };

  auto math = [&](uint32 source, uint32 modify, bool carry) {
    uint32 result = source + modify + carry;
    if(save) {
      uint32 overflow = ~(source ^ modify) & (source ^ result);
      cpsr().n = result >> 31;
      cpsr().z = result == 0;
      cpsr().c = (1u << 31) & (overflow ^ source ^ modify ^ result);
      cpsr().v = (1u << 31) & (overflow);
    }
    return result;
  };

  switch(opcode) {
  case  0: r(d) = test(rn & rm);            break;  //AND
  case  1: r(d) = test(rn ^ rm);            break;  //EOR
  case  2: r(d) = math(rn, ~rm, 1);         break;  //SUB
  case  3: r(d) = math(rm, ~rn, 1);         break;  //RSB
  case  4: r(d) = math(rn,  rm, 0);         break;  //ADD
  case  5: r(d) = math(rn,  rm, cpsr().c);  break;  //ADC
  case  6: r(d) = math(rn, ~rm, cpsr().c);  break;  //SBC
  case  7: r(d) = math(rm, ~rn, cpsr().c);  break;  //RSC
  case  8:        test(rn & rm);            break;  //TST
  case  9:        test(rn ^ rm);            break;  //TEQ
  case 10:        math(rn, ~rm, 1);         break;  //CMP
  case 11:        math(rn,  rm, 0);         break;  //CMN
  case 12: r(d) = test(rn | rm);            break;  //ORR
  case 13: r(d) = test(rm);                 break;  //MOV
  case 14: r(d) = test(rn &~rm);            break;  //BIC
  case 15: r(d) = test(~rm);                break;  //MVN
  }
}

void ARM::arm_move_to_status(uint32 rm) {
  uint1 source = instruction() >> 22;
  uint4 field = instruction() >> 16;

  if(source == 1) {
    if(mode() == Processor::Mode::USR) return;
    if(mode() == Processor::Mode::SYS) return;
  }

  PSR &psr = source ? spsr() : cpsr();

  if(field & 1) {
    if(source == 1 || (Processor::Mode)cpsr().m != Processor::Mode::USR) {
      psr.i = rm & 0x00000080;
      psr.f = rm & 0x00000040;
      psr.t = rm & 0x00000020;
      psr.m = rm & 0x0000001f;
      if(source == 0) processor.setMode((Processor::Mode)psr.m);
    }
  }

  if(field & 8) {
    psr.n = rm & 0x80000000;
    psr.z = rm & 0x40000000;
    psr.c = rm & 0x20000000;
    psr.v = rm & 0x10000000;
  }
}

//logical shift left
void ARM::lsl(bool &c, uint32 &rm, uint32 rs) {
  while(rs--) {
    c = rm >> 31;
    rm <<= 1;
  }
}

//logical shift right
void ARM::lsr(bool &c, uint32 &rm, uint32 rs) {
  while(rs--) {
    c = rm & 1;
    rm >>= 1;
  }
}

//arithmetic shift right
void ARM::asr(bool &c, uint32 &rm, uint32 rs) {
  while(rs--) {
    c = rm & 1;
    rm = (int32)rm >> 1;
  }
}

//rotate right
void ARM::ror(bool &c, uint32 &rm, uint32 rs) {
  while(rs--) {
    c = rm & 1;
    rm = (rm << 31) | (rm >> 1);
  }
}

//rotate right with extend
void ARM::rrx(bool &c, uint32 &rm) {
  bool carry = c;
  c = rm & 1;
  rm = (carry << 31) | (rm >> 1);
}

//mul{condition}{s} rd,rm,rs
//mla{condition}{s} rd,rm,rs,rn
//cccc 0000 00as dddd nnnn ssss 1001 mmmm
//c = condition
//a = accumulate
//s = save flags
//d = rd
//n = rn
//s = rs
//n = rm
void ARM::arm_op_multiply() {
  uint1 accumulate = instruction() >> 21;
  uint1 save = instruction() >> 20;
  uint4 d = instruction() >> 16;
  uint4 n = instruction() >> 12;
  uint4 s = instruction() >> 8;
  uint4 m = instruction() >> 0;

  auto &rd = r(d);
  uint32 rs = r(s);
  auto &rm = r(m);

  rd = accumulate ? r(n) : 0u;
  step(1);

  //Modified Booth Encoding
  bool carry = 0;
  unsigned place = 0;

  do {
    step(1);
    signed factor = (int2)rs + carry;

    if(factor == -2) rd -= rm << (place + 1);
    if(factor == -1) rd -= rm << (place + 0);
    if(factor == +1) rd += rm << (place + 0);
    if(factor == +2) rd += rm << (place + 1);

    carry = rs & 2;
    place += 2;
    rs >>= 2;
  } while(rs + carry && place < 32);

  if(save) {
    cpsr().n = r(d) >> 31;
    cpsr().z = r(d) == 0;
    cpsr().c = carry;
  }
}

//mrs{condition} rd,(c,s)psr
//cccc 0001 0r00 ++++ dddd ---- 0000 ----
//c = condition
//r = SPSR (0 = CPSR)
//d = rd
void ARM::arm_op_move_to_register_from_status() {
  uint1 source = instruction() >> 22;
  uint4 d = instruction() >> 12;

  if(source) {
    if(mode() == Processor::Mode::USR) return;
    if(mode() == Processor::Mode::SYS) return;
  }

  r(d) = source ? spsr() : cpsr();
}

//swp{condition}{b} rd,rm,[rn]
//cccc 0001 0b00 nnnn dddd ---- 1001 mmmm
//c = condition
//b = byte (0 = word)
//n = rn
//d = rd
//m = rm
void ARM::arm_op_memory_swap() {
  uint1 byte = instruction() >> 22;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint4 m = instruction();

  uint32 word = bus_read(r(n), byte ? Byte : Word);
  bus_write(r(n), byte ? Byte : Word, r(m));
  r(d) = word;
}

//msr{condition} (c,s)psr:{fields},rm
//cccc 0001 0r10 ffff ++++ ---- 0000 mmmm
//c = condition
//r = SPSR (0 = CPSR)
//f = field mask
//m = rm
void ARM::arm_op_move_to_status_from_register() {
  uint4 m = instruction();

  arm_move_to_status(r(m));
}

//bx{condition} rm
//cccc 0001 0010 ++++ ++++ ++++ 0001 mmmm
//c = condition
//m = rm
void ARM::arm_op_branch_exchange_register() {
  uint4 m = instruction();

  r(15) = r(m);
  cpsr().t = r(m) & 1;
}

//msr{condition} (c,s)psr:{fields},#immediate
//cccc 0011 0r10 ffff ++++ rrrr iiii iiii
//c = condition
//r = SPSR (0 = CPSR)
//f = field mask
//r = rotate
//i = immediate
void ARM::arm_op_move_to_status_from_immediate() {
  uint4 rotate = instruction() >> 8;
  uint8 immediate = instruction();

  uint32 rm = immediate;
  bool c = cpsr().c;
  if(rotate) ror(c, rm, 2 * rotate);

  arm_move_to_status(rm);
}

//{opcode}{condition}{s} rd,rm {shift} #immediate
//{opcode}{condition} rn,rm {shift} #immediate
//{opcode}{condition}{s} rd,rn,rm {shift} #immediate
//cccc 000o ooos nnnn dddd llll lss0 mmmm
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//l = shift immediate
//s = shift
//m = rm
void ARM::arm_op_data_immediate_shift() {
  uint1 save = instruction() >> 20;
  uint5 shift = instruction() >> 7;
  uint2 mode = instruction() >> 5;
  uint4 m = instruction();

  uint32 rs = shift;
  uint32 rm = r(m);
  bool c = cpsr().c;

  if(mode == 0) lsl(c, rm, rs);
  if(mode == 1) lsr(c, rm, rs ? rs : 32);
  if(mode == 2) asr(c, rm, rs ? rs : 32);
  if(mode == 3) rs ? ror(c, rm, rs) : rrx(c, rm);

  carryout() = c;
  arm_opcode(rm);
}

//{opcode}{condition}{s} rd,rm {shift} rs
//{opcode}{condition} rn,rm {shift} rs
//{opcode}{condition}{s} rd,rn,rm {shift} rs
//cccc 000o ooos nnnn dddd ssss 0ss1 mmmm
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//s = rs
//s = shift
//m = rm
void ARM::arm_op_data_register_shift() {
  uint1 save = instruction() >> 20;
  uint4 s = instruction() >> 8;
  uint2 mode = instruction() >> 5;
  uint4 m = instruction();

  uint8 rs = r(s);
  uint32 rm = r(m);
  bool c = cpsr().c;

  if(mode == 0) lsl(c, rm, rs < 33 ? rs : 33);
  if(mode == 1) lsr(c, rm, rs < 33 ? rs : 33);
  if(mode == 2) asr(c, rm, rs < 32 ? rs : 32);
  if(mode == 3 && rs) ror(c, rm, rs & 31 == 0 ? 32 : rs & 31);

  carryout() = c;
  arm_opcode(rm);
}

//{opcode}{condition}{s} rd,#immediate
//{opcode}{condition} rn,#immediate
//{opcode}{condition}{s} rd,rn,#immediate
//cccc 001o ooos nnnn dddd ssss iiii iiii
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//s = shift immediate
//i = immediate
void ARM::arm_op_data_immediate() {
  uint1 save = instruction() >> 20;
  uint4 shift = instruction() >> 8;
  uint8 immediate = instruction();

  uint32 rs = shift << 1;
  uint32 rm = (immediate >> rs) | (immediate << (32 - rs));
  if(rs) carryout() = immediate >> 31;

  arm_opcode(rm);
}

//(ldr,str){condition}{b} rd,[rn{,+/-offset}]{!}
//(ldr,str){condition}{b} rd,[rn]{,+/-offset}
//cccc 010p ubwl nnnn dddd iiii iiii iiii
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up (add/sub offset to base)
//b = byte (1 = word)
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//i = immediate
void ARM::arm_op_move_immediate_offset() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 byte = instruction() >> 22;
  uint1 writeback = instruction() >> 21;
  uint1 load = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint12 rm = instruction();

  uint32 rn = r(n);
  auto &rd = r(d);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  if(load) {
    rd = bus_read(rn, byte ? Byte : Word);
  } else {
    bus_write(rn, byte ? Byte : Word, rd);
  }
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//(ldr,str){condition}{b} rd,[rn,rm {mode} #immediate]{1}
//(ldr,str){condition}{b} rd,[rn],rm {mode} #immediate
//cccc 011p ubwl nnnn dddd llll lss0 mmmm
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up
//b = byte (1 = word)
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//l = shift immediate
//s = shift mode
//m = rm
void ARM::arm_op_move_register_offset() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 byte = instruction() >> 22;
  uint1 writeback = instruction() >> 21;
  uint1 load = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint4 d = instruction() >> 12;
  uint5 immediate = instruction() >> 7;
  uint2 mode = instruction() >> 5;
  uint4 m = instruction();

  uint32 rn = r(n);
  auto &rd = r(d);
  uint32 rs = immediate;
  uint32 rm = r(m);
  bool c = cpsr().c;

  if(mode == 0) lsl(c, rm, rs);
  if(mode == 1) lsr(c, rm, rs ? rs : 32);
  if(mode == 2) asr(c, rm, rs ? rs : 32);
  if(mode == 3) rs ? ror(c, rm, rs) : rrx(c, rm);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  if(load) {
    rd = bus_read(rn, byte ? Byte : Word);
  } else {
    bus_write(rn, byte ? Byte : Word, rd);
  }
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback == 1) r(n) = rn;
}

//(ldm,stm){condition}{mode} rn{!},{r...}
//cccc 100p uswl nnnn llll llll llll llll
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up (add/sub offset to base)
//s = spsr copy -or- usr register copy
//w = writeback
//l = load (0 = save)
//n = rn
//l = register list
void ARM::arm_op_move_multiple() {
  uint1 pre = instruction() >> 24;
  uint1 up = instruction() >> 23;
  uint1 s = instruction() >> 22;
  uint1 writeback = instruction() >> 21;
  uint1 load = instruction() >> 20;
  uint4 n = instruction() >> 16;
  uint16 list = instruction();

  uint32 rn = r(n);
  if(pre == 0 && up == 1) rn = rn + 0;  //IA
  if(pre == 1 && up == 1) rn = rn + 4;  //IB
  if(pre == 1 && up == 0) rn = rn - bit::count(list) * 4 + 0;  //DB
  if(pre == 0 && up == 0) rn = rn - bit::count(list) * 4 + 4;  //DA

  Processor::Mode pmode = mode();
  bool usr = false;
  if(s && load == 1 && (list & 0x8000) == 0) usr = true;
  if(s && load == 0) usr = true;

  if(usr) processor.setMode(Processor::Mode::USR);

  for(unsigned n = 0; n < 16; n++) {
    if(list & (1 << n)) {
      if(load) r(n) = bus_read(rn, Word);
      else bus_write(rn, Word, r(n));
      rn += 4;
    }
  }

  if(usr) processor.setMode(pmode);

  if(load == 1 && s && (list & 0x8000)) {
    if(mode() != Processor::Mode::USR && mode() != Processor::Mode::SYS) {
      cpsr() = spsr();
      processor.setMode((Processor::Mode)cpsr().m);
    }
  }

  if(writeback) {
    if(up == 1) r(n) = r(n) + bit::count(list) * 4;  //IA, IB
    if(up == 0) r(n) = r(n) - bit::count(list) * 4;  //DA, DB
  }
}

//b{l}{condition} address
//cccc 101l dddd dddd dddd dddd dddd dddd
//c = condition
//l = link
//d = displacement (24-bit signed)
void ARM::arm_op_branch() {
  uint1 link = instruction() >> 24;
  int24 displacement = instruction();

  if(link) r(14) = r(15) - 4;
  r(15) += displacement * 4;
}

//swi #immediate
//cccc 1111 iiii iiii iiii iiii iiii iiii
//c = condition
//i = immediate
void ARM::arm_op_software_interrupt() {
  uint24 immediate = instruction();

  vector(0x00000008, Processor::Mode::SVC);
}

#endif
